#include <stdlib.h>
#include <limits.h>

#ifndef SIZE_T_MAX
#define SIZE_T_MAX 1ULL<<31
#endif

typedef struct ht ht;

typedef char *key;
#define key_clone(x) (strdup(x))
#define key_drop(x) (free((x)))
#define key_eq(a,b) (strcmp((a), (b)) == 0)

typedef char *value;
#define value_clone(x) (strdup(x))
#define value_drop(x) (free(x))

ht *ht_new (size_t size);
value ht_select (ht *, key);
void ht_insert (ht *, key, value);
void ht_update (ht *, key, value);
void ht_upsert (ht *, key, value);
void ht_delete (ht *, key);
void ht_keys (ht *, key *);
size_t ht_count (ht *);
void ht_drop (ht *);

void ht_show (ht *);

#define HASH_UNIVERSAL

#define wrapping_add(x,y,max) \
	(((x) + (y)) % (max))

#ifdef HASH_PHP
#define key_hash(k,N) \
	({ \
		strlen ((k)) % (N); \
	})
#endif

#ifdef HASH_PLUS
#define key_hash(k,N) \
	({ \
		size_t h = 0; \
		for (size_t i = 0; k[i] != '\0'; i++) \
			h = wrapping_add (h, (unsigned)k[i], SIZE_T_MAX); \
		h % N; \
	})
#endif

#ifdef HASH_PLUSPLUS
#define key_hash(k,N) \
	({ \
		size_t h = 0; \
		unsigned a = 127; \
		for (size_t i = 0; k[i] != '\0'; i++) \
			h = wrapping_add (a * h, (unsigned)k[i], SIZE_T_MAX) % N; \
		h; \
	})
#endif

#ifdef HASH_UNIVERSAL
#define key_hash(k,N) \
	({ \
		size_t h = 0; \
		unsigned a = 31415, b = 21783; \
		for (size_t i = 0; k[i] != '\0'; i++) { \
			a = (a * b) % (N - 1); \
			h = wrapping_add (a * h, (unsigned)k[i], SIZE_T_MAX) % N; \
		} \
		h; \
	})
#endif
