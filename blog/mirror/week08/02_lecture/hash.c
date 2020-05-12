
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <err.h>
#include <sysexits.h>

#ifndef SIZE_T_MAX
#define SIZE_T_MAX 1ULL<<31
#endif

enum approach {
	HASH_PHP,
	HASH_PLUS,
	HASH_PLUSTIMES,
	HASH_PLUSPLUS,
	HASH_UNIVERSAL
};

static size_t key_hash (char *k, size_t N, enum approach approach);

#define wrapping_add(x,y,max) \
	(((x) + (y)) % (max))    
    
int main (int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
		errx (EX_USAGE, "usage: hash <size> [<approach>]");

	size_t ht_size = (size_t) strtol (argv[1], NULL, 10);

	enum approach app = HASH_UNIVERSAL;
	if (argc == 3) {
		long x = strtol (argv[2], NULL, 10);
		assert (HASH_PHP <= x && x <= HASH_UNIVERSAL);
		app = (enum approach) x;
	}

	char buf[BUFSIZ];
	while (fgets (buf, BUFSIZ, stdin) != NULL) {
		*(strrchr (buf, '\n')) = '\0';
		printf ("%zu\t%s\n", key_hash (buf, ht_size, app), buf);
	}
}

static size_t key_hash (char *k, size_t N, enum approach approach)
{
	switch (approach) {
	case HASH_PHP:
		return strlen (k) % N;

	case HASH_PLUS: {
		size_t h = 0;
		for (size_t i = 0; k[i] != '\0'; i++) // h += k[i];
			h = wrapping_add (h, (unsigned) k[i], SIZE_T_MAX);
		return h % N;
	}

	case HASH_PLUSTIMES: {
		size_t h = 0;
		for (size_t i = 0; k[i] != '\0'; i++) // h += k[i] * i;
			h = wrapping_add (h, i * (unsigned) k[i], SIZE_T_MAX);
		return h % N;
	}

	case HASH_PLUSPLUS: {
		size_t h = 0;
		unsigned a = 127; // prime!
		for (size_t i = 0; k[i] != '\0'; i++) {
#if 0
			printf ("at [%zu], a * h + k[i] = %u * %zu + %hhu\n",
					i, a, h, k[i]);
#endif
			h = wrapping_add (a * h, (unsigned) k[i], SIZE_T_MAX) % N;
		}
		return h;
	}

	case HASH_UNIVERSAL: {
		size_t h = 0;
		unsigned a = 31415, b = 21783;
		for (size_t i = 0; k[i] != '\0'; i++) {
			a = (a * b) % (N - 1);
#if 0
			printf ("at [%zu], a * h + k[i] ="
					" %u * %zu + %hhu ="
					" %zu\n",
					i, a, h, k[i], ((a * h) + k[i]) % N);
#endif
			h = wrapping_add (a * h, (unsigned)k[i], SIZE_T_MAX) % N;
		}
		return h;
	}
	}
}
