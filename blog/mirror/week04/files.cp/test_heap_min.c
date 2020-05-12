#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap_min.h"

static void test_valid_minheaps (void);
static void test_invalid_minheaps (void);

int main (void)
{
	// Stops output being buffered;
	// we get all printf output before crashes/aborts
	setbuf (stdout, NULL);

	test_valid_minheaps ();
	test_invalid_minheaps ();

	puts ("\nAll tests passed. You are awesome!");
	return EXIT_SUCCESS;
}


static void test_valid_minheaps (void)
{
	int heap1[] = { 0, 2, 10 };

	puts ("valid minheaps: test 1: 2,10");
	assert (heap_min_p (heap1, 2));

	// ... add more tests! ...
}

static void test_invalid_minheaps (void)
{
	int heap1[] = { 0, 10, 7, 6 };

	puts ("invalid minheaps: test 1: 10,7,6");
	assert (! heap_min_p (heap1, 3));

	// ... add more tests! ...
}
