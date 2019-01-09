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

	{
		int heap[] = { 0, 2, 7, 10 };
		puts ("valid minheaps: test 2: 2, 7, 10");
		assert (heap_min_p (heap, 3));
	}

	{
		int heap[] = { 0, 2, 7, 10, 9, 8};
		puts ("valid minheaps: test 3: 2, 7, 10, 9, 8");
		assert (heap_min_p (heap, 5));
	}
}

static void test_invalid_minheaps (void)
{
	int heap1[] = { 0, 10, 7, 6 };
	puts ("invalid minheaps: test 1: 10, 7, 6");
	assert (! heap_min_p (heap1, 3));

	{
		int heap[] = { 0, 1, 0};
		puts ("invalid minheaps: test 2: 1, 0");
		assert (!heap_min_p (heap, 2));
	}

	{
		int heap[] = { 0, 2, 7, 10, 6, 9};
		/////////////////////////////////   1  2  a   b
		puts ("invalid minheaps: test 3: 0, 2, 7, 10, 6, 9");
		assert (! heap_min_p (heap, 5));
	}
}

/*

def genCode(items:[int], type: bool, testN: int):
  s = ", ".join(map(str, items))
  return """
{
  int heap[] = {0, %s};
  puts("%s minheaps: test %d: %s");
  assert(%s heap_min_p(heap, %d));
}
""" % (s, "valid" if type else "invalid", testN, s, '' if type else '!', len(items))


print("\n".join([genCode([1,2,3], True, 1), genCode([4,5,6], True, 1), genCode([1,7,2], True, 1)]))

 */