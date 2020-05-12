#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>

typedef uint64_t u64;

u64 good_fib (u64 n);

int main (int argc, char *argv[])
{
	if (argc != 2)
		errx (EX_USAGE, "usage: bad_fib <num>");

	long n = strtol (argv[1], NULL, 10);

	printf ("fib(%ld) = %" PRIu64 "\n", n, good_fib (n));

	return EXIT_SUCCESS;
}

// Code to return the nth fibonacci number
// 0 1 1 2 3 5 8 13 etc
//
// This version caches intermediate results, an approach known as
// "dynamic programming", leading to increased efficiency.
u64 good_fib (u64 n)
{
	u64 tmp;
	u64 prev1 = 0;
	u64 prev2 = 1;
	for (u64 i = 0; i < n; i++) {
		tmp = prev1;
		prev1 = prev2;
		prev2 = tmp + prev2;
	}
	return prev1;
}
