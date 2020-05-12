// Calculate the factorial of an integer n
// Note that factorials grow big very quickly

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef uint64_t u64;

static bool prompt (int *);
static u64 factorial_rec (int);
static u64 factorial_iter (int);

int main (void)
{
	int n;
	while (prompt (&n)) {
		printf ("%d! is %" PRIu64 "\n", n, factorial_iter (n));
		printf ("%d! is %" PRIu64 "\n", n, factorial_rec (n));
	}

	return 0;
}

static bool prompt (int *n)
{
	printf ("Enter n: ");
	return scanf ("%d", n) == 1;
}

// Really large n can cause program to crash for recursive values.
// Try 999999 ... why?
static u64 factorial_rec (int n)
{
	if (n <= 0) return 1;
	return n * factorial_rec (n - 1);
}

static u64 factorial_iter (int n)
{
	u64 result = 1;
	while (n >= 1) {
		result *= n;
		n--;
	}
	return result;
}
