#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>

typedef uint64_t u64;

u64 bad_fib (u64 n);

int main (int argc, char *argv[])
{
	if (argc != 2)
		errx (EX_USAGE, "usage: bad_fib <num>");

	long n = strtol (argv[1], NULL, 10);

	printf ("fib(%ld) = %" PRIu64 "\n", n, bad_fib (n));

	return EXIT_SUCCESS;
}

// Code to return the n'th fibonacci number
// 0 1 1 2 3 5 8 13 etc
//
// This code is very inefficient! It is exponential.
// Time it with input of say 30 vs 45 vs 46 vs 47 etc
u64 bad_fib (u64 n)
{
	switch (n) {
	case 0:  return 0;
	case 1:  return 1;
	default: return bad_fib (n-1) + bad_fib (n-2);
	}
}
