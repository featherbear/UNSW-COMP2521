////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... leak analysis
//
// Curtis Millar suggested a shim of `malloc` and `calloc` (and `free`)
// to check that all allocated memory is allocated and free'd, with our
// own headers.
//
// TODO:
//  * address-to-line translation of free'd frame values; currently, we
//    just report `symbol+offset`, which is probably not all that useful
//    for students.  this will involve DWARF mangling.
//  * balancing the tree better. it's probably close to degenerate.
//
// 2018-11-24	Jashank Jeremy <jashank.jeremy@unsw.edu.au>

#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <execinfo.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral" //!!!
#pragma clang diagnostic ignored "-Wunused-macros"

#include <sanitizer/asan_interface.h>

#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#define ASAN_REGION_IS_POISONED(addr, size) \
	__asan_region_is_poisoned((addr), (size))
#else
#define ASAN_REGION_IS_POISONED(addr, size) false
#endif

////////////////////////////////////////////////////////////////////////

#define __dead2				__attribute__((__noreturn__))
#define __malloc_like		__attribute__((__malloc__))
#define __alloc_size(x)		__attribute__((__alloc_size__(x)))
#define __alloc_size2(n, x)	__attribute__((__alloc_size__(n, x)))
#define __result_use_check	__attribute__((__warn_unused_result__))
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

typedef struct alloc  alloc;
static struct alloc {
	enum alloc_magic {
		ALLOC_MAGIC_INUSE = 0x616c6c6f,	// 'allo'
		ALLOC_MAGIC_FREE  = 0x66726565,	// 'free'
	} magic; /**< magic markers for allocation */

	uintptr_t key;	/**< the pointer to the user allocation */
	alloc *l;		/**< the left subtree of allocations */
	alloc *r;		/**< the left subtree of allocations */

	size_t alloc_frames;
	void *alloc_by[8];	/**< pointer to allocating function */
	size_t free_frames;
	void *free_by[8];	/**< pointer to freeing function */

	size_t size;
	char real[];	/**< flexible array member; the data went data way */
} *root = NULL;

static enum err_reason {
	ERR_NONE = 0,
	ERR_FREE_WTF,
	ERR_FREE_WTAF,
	ERR_ALLOC_FAIL,
	ERR_TOO_FREE,
	NUM_ERR_REASONS
} reason = ERR_NONE;

////////////////////////////////////////////////////////////////////////

#define REAL(sym)	__real_##sym
#define WRAP(sym)	__wrap_##sym

extern void *REAL(malloc)  (size_t);
extern void *REAL(calloc)  (size_t, size_t);
extern void *REAL(realloc) (void *, size_t);
extern void  REAL(free)    (void *);
extern char *REAL(strdup)  (const char *);
extern char *REAL(strndup) (const char *, size_t);

void *WRAP(malloc) (size_t)
	__malloc_like __alloc_size(1) __result_use_check;
void *WRAP(calloc) (size_t, size_t)
	__malloc_like __alloc_size2(1, 2) __result_use_check;
void *WRAP(realloc) (void *, size_t)
	__alloc_size(2) __result_use_check;
char *WRAP(strdup) (const char *)
	__malloc_like __result_use_check;
char *WRAP(strndup) (const char *, size_t)
	__malloc_like __result_use_check;
void  WRAP(free) (void *);

static void a_new (size_t, alloc **, void **);
static alloc *a_insert (alloc *, alloc *);
static alloc *a_search (uintptr_t, alloc *);
static inline void *alloc_to_user_ptr (alloc *);
static inline alloc *user_to_alloc_ptr (void *);

static void init_leak2521 (void) __attribute__((constructor));
static void fini_leak2521 (void) __attribute__((destructor));

static void check_freed_alloc (alloc *, size_t *, size_t *);

static void catastrophe (enum err_reason, alloc *, const char *, ...)
	__dead2;

////////////////////////////////////////////////////////////////////////

#ifdef __FreeBSD__
static bool broken_asan_backtrace_warn = false;
#define do_traceback(n,f) \
	({ if (!broken_asan_backtrace_warn) \
			warnx("`backtrace(3)' is broken on FreeBSD; " \
				  "stack traces won't be available"); \
		broken_asan_backtrace_warn = true; \
	})
#else
#define do_traceback(n_frames, frames) \
	({ \
		void *call_fp[9] = {0}; \
		size_t _frames = (size_t) backtrace (call_fp, 9); \
		for (size_t i = 0; i < _frames && i < 8; i++) \
			(frames)[i] = call_fp[i + 1]; \
		(n_frames) = _frames - 1; \
	})
#endif

static alloc *
do_alloc (size_t size)
{
	alloc *aptr;
	void  *uptr;

	a_new (size, &aptr, &uptr);
	root = a_insert (root, aptr);

	return aptr;
}

void *
WRAP(malloc) (size_t size)
{
	alloc *aptr = do_alloc (size);
	void  *uptr = alloc_to_user_ptr (aptr);
	do_traceback (aptr->alloc_frames, aptr->alloc_by);
	memset (uptr, 0xbe, size);
	ASAN_POISON_MEMORY_REGION (aptr, sizeof (alloc));
	return uptr;
}

char *
WRAP(strdup) (const char *str)
{
	if (str == NULL) return NULL;

	// "@(#)strdup.c    8.1 (Berkeley) 6/4/93"
	size_t len = strlen (str) + 1;
	char *copy;
	if ((copy = WRAP(malloc) (len)) == NULL)
		return (NULL);
	memcpy (copy, str, len);
	return (copy);
}

char *
WRAP(strndup) (const char *str, size_t maxlen)
{
	if (str == NULL) return NULL;

	// $OpenBSD: strndup.c,v 1.1 2010/05/18 22:24:55 tedu Exp $
	size_t len = strnlen(str, maxlen);
	char *copy = WRAP(malloc) (len + 1);
	if (copy != NULL) {
		(void) memcpy (copy, str, len);
		copy[len] = '\0';
	}
	return copy;
}

void *
WRAP(calloc) (size_t number, size_t size)
{
	size_t num_size;
	// checked arithmetic builtins
	// <https://clang.llvm.org/docs/LanguageExtensions.html#checked-arithmetic-builtins>
	if (__builtin_mul_overflow (number, size, &num_size)) {
		errno = ENOMEM;
		return NULL;
	}

	alloc *aptr = do_alloc (num_size);
	void  *uptr = alloc_to_user_ptr (aptr);
	do_traceback (aptr->alloc_frames, aptr->alloc_by);
	memset (uptr, 0x00, size);
	ASAN_POISON_MEMORY_REGION (aptr, sizeof (alloc));
	return uptr;
}

void *
WRAP(realloc) (void *user_ptr, size_t size)
{
	if (user_ptr == NULL) return WRAP(malloc) (size);

	alloc *old_aptr = user_to_alloc_ptr (user_ptr);
	ASAN_UNPOISON_MEMORY_REGION (old_aptr, sizeof (alloc));
	if (old_aptr->magic == ALLOC_MAGIC_FREE)
		catastrophe (ERR_TOO_FREE, old_aptr, "realloc %p", user_ptr);

	alloc *new_aptr = do_alloc (size);
	void  *new_uptr = alloc_to_user_ptr (new_aptr);
	memset (new_uptr, 0xbe, size);
	memcpy (new_uptr, user_ptr, min (old_aptr->size, size));

	do_traceback (old_aptr->free_frames,  old_aptr->free_by);
	do_traceback (new_aptr->alloc_frames, new_aptr->alloc_by);

	old_aptr->magic = ALLOC_MAGIC_FREE;
	ASAN_POISON_MEMORY_REGION (user_ptr, old_aptr->size);
	ASAN_POISON_MEMORY_REGION (old_aptr, sizeof (alloc));
	ASAN_POISON_MEMORY_REGION (new_aptr, sizeof (alloc));
	return new_uptr;
}

void
WRAP(free) (void *user_ptr)
{
	if (user_ptr == NULL) return;

	alloc *our_ptr_maybe = user_to_alloc_ptr (user_ptr);
	alloc *a = a_search ((uintptr_t) user_ptr, root);
	if (a == NULL)
		catastrophe (ERR_FREE_WTF, our_ptr_maybe, "free %p", user_ptr);
	if (a != our_ptr_maybe)
		catastrophe (ERR_FREE_WTAF, a, "free %p", user_ptr);

	ASAN_UNPOISON_MEMORY_REGION (a, sizeof (alloc));
	if (a->magic == ALLOC_MAGIC_FREE)
		catastrophe (ERR_TOO_FREE, a, "free %p", user_ptr);

	a->magic = ALLOC_MAGIC_FREE;
	ASAN_POISON_MEMORY_REGION (user_ptr, a->size);

	do_traceback (a->free_frames, a->free_by);

	ASAN_POISON_MEMORY_REGION (a, sizeof (alloc));
	return;
}

////////////////////////////////////////////////////////////////////////

static void
a_new (size_t n_bytes, alloc **aptr, void **uptr)
{
	*aptr = REAL(malloc) (sizeof (alloc) + n_bytes);
	*uptr = alloc_to_user_ptr (*aptr);

	(**aptr) = (alloc) {
		.magic = ALLOC_MAGIC_INUSE,
		.key = (uintptr_t) *uptr,
		.l = NULL, .r = NULL,
		.alloc_by = {}, .alloc_frames = 0,
		.free_by  = {}, .free_frames  = 0,
		.size = n_bytes,
	};
}

static alloc *
a_insert (alloc *as, alloc *a)
{
	if (a == NULL)
		catastrophe (ERR_ALLOC_FAIL, a, "alloc insert %p", (void *)as);

	bool was_poisoned =
		ASAN_REGION_IS_POISONED (as, sizeof (alloc)) != NULL;

	if (was_poisoned) ASAN_UNPOISON_MEMORY_REGION (as, sizeof (alloc));

	if (as == NULL)             as    = a;
	else if (a->key <  as->key) as->l = a_insert (as->l, a);
	else if (a->key  > as->key) as->r = a_insert (as->r, a);
	else // if (a->key == as->key) or something else
		catastrophe (
			ERR_ALLOC_FAIL, a,
			"alloc insert %p at %p",
			(void *)a, (void *)as
		);

	if (was_poisoned) ASAN_POISON_MEMORY_REGION (as, sizeof (alloc));

	return as;
}

static alloc *
a_search (uintptr_t key, alloc *a)
{
	if (a == NULL)          return NULL;

	ASAN_UNPOISON_MEMORY_REGION (a, sizeof (alloc));
	alloc *ret;

	     if (a->key <  key) ret = a_search (key, a->r);
	else if (a->key  > key) ret = a_search (key, a->l);
	else if (a->key == key) ret = a;
	else                    ret = NULL;

	ASAN_POISON_MEMORY_REGION (a, sizeof (alloc));
	return ret;
}

static inline void *
alloc_to_user_ptr (alloc *aptr)
{
	return (void *)(((uintptr_t) aptr) + sizeof (alloc));
}

static inline alloc *
user_to_alloc_ptr (void *uptr)
{
	return (alloc *)(((uintptr_t) uptr) - sizeof (alloc));
}

////////////////////////////////////////////////////////////////////////

static void
init_leak2521 (void)
{
}

static void
fini_leak2521 (void)
{
	// don't bother doing leak analysis on erroneous exits.
	if (reason != ERR_NONE) return;

	size_t lost_allocs = 0, lost_bytes = 0;
	check_freed_alloc (root, &lost_allocs, &lost_bytes);

	if (lost_allocs > 0 || lost_bytes > 0)
		fprintf (
			stderr,
			"Lost %zu bytes in %zu allocations.\n",
			lost_bytes, lost_allocs
		);
}

static void
show_frames (size_t n_frames, void **frames)
{
	for (size_t i = 0; i < n_frames; i++) {
		Dl_info dli;
		dladdr (frames[i], &dli);
		if (dli.dli_sname == NULL || dli.dli_saddr == NULL)
			fprintf (stderr, "\t[ %p ]\n", frames[i]);
		else
			fprintf (stderr, "\t%s+%" PRIxPTR "\n",
				dli.dli_sname,
				((uintptr_t) frames[i]) - ((uintptr_t) dli.dli_saddr)
			);
	}
}

static void
check_one_freed_alloc (alloc *a, size_t *lost_allocs, size_t *lost_bytes)
{
	if (a == NULL) return;

	if (a->magic == ALLOC_MAGIC_FREE) return;
	fprintf (
		stderr,
		"Leak of %zu bytes at %p\n",
		a->size, (void *)a->real
	);

	if (a->alloc_frames > 0) {
		fprintf (stderr, "    allocated in\n");
		show_frames (a->alloc_frames, a->alloc_by);
	}

	if (a->free_frames > 0) {
		fprintf (stderr, "    freed in\n");
		show_frames (a->free_frames, a->free_by);
	}

	fprintf (stderr, "\n");

	if (lost_allocs) *lost_allocs += 1;
	if (lost_bytes)  *lost_bytes += a->size;
}

static void
check_freed_alloc (alloc *a, size_t *lost_allocs, size_t *lost_bytes)
{
	if (a == NULL) return;
	ASAN_UNPOISON_MEMORY_REGION (a, sizeof (alloc));

	check_freed_alloc (a->l, lost_allocs, lost_bytes);
	check_one_freed_alloc (a, lost_allocs, lost_bytes);
	check_freed_alloc (a->r, lost_allocs, lost_bytes);

	REAL(free) (a);
}

////////////////////////////////////////////////////////////////////////

#ifdef __LEAK2521_DEBUG_HEAP
static void
describe_heap (alloc *a)
{
	if (a == NULL) return;
	ASAN_UNPOISON_MEMORY_REGION (a, sizeof (alloc));

	fprintf (
		stderr,
		"%p: %s, %zu bytes at %p\n",
		(void *)a,
		(a->magic == ALLOC_MAGIC_INUSE ? "inuse" :
		 a->magic == ALLOC_MAGIC_FREE  ? "free " : "?????"),
		a->size,
		(void *)a->real
	);

	describe_heap (a->l);
	describe_heap (a->r);
}
#endif

////////////////////////////////////////////////////////////////////////

static const char *err_reasons[NUM_ERR_REASONS] = {
	[ERR_NONE]		= "no reason?",
	[ERR_FREE_WTF]	= "the pointer passed was not allocated",
	[ERR_FREE_WTAF]	= "alloc* header is not in the chunk tree",
	[ERR_TOO_FREE]	= "the pointer passed has already been free'd",
	[ERR_ALLOC_FAIL]= "unable to allocate memory",
};

static void
catastrophe (enum err_reason rx, alloc *a, const char *fmt, ...)
{
	reason = rx;

	va_list ap;
	va_start (ap, fmt);

	vwarnx (fmt, ap);
	fprintf (stderr, "... %s\n", err_reasons[rx]);

	if (rx == ERR_TOO_FREE && a != NULL && a->free_frames > 0) {
		fprintf (stderr, "    freed in\n");
		show_frames (a->free_frames, a->free_by);
	}

	va_end (ap);

#ifdef __LEAK2521_DEBUG_HEAP
	describe_heap (root);
#endif

	// skip the destructors, please
	exit (10 + (int) rx);
}

#pragma clang diagnostic pop
