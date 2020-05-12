#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t binary_search (int a[], size_t n, int key);
static bool prompt_int (char *, int *);

int main (void)
{
	int size;
	prompt_int ("Enter number of elements: ", &size);

	int key;
	// prompt_int ("Enter key to search for: ", &key);
	key = -99;

	int *a = calloc (size, sizeof(int));
	puts ("Enter elements...");

	for (size_t i = 0; i < (unsigned int)size; i++)
		scanf ("%d",&a[i]);

	printf ("binary_search(%d) ... ", key);
	ssize_t result = binary_search(a,size,key);
	if (result != -1)
		printf ("found at index %zu\n", result);
	else
		printf ("not found\n");

	return EXIT_SUCCESS;
}

static bool prompt_int (char *prompt, int *n)
{
	fputs (prompt, stdout);
	return scanf ("%d", n) == 1;
}

//         { 1, 2, 3, 4, 5, 6, 8, 9, 10 } (n = 9)
//                          ^ == 7
// step 3:              lo |  | hi
// step 2:                 | lo      hi |
// step 1: | lo                      hi |
//
// Binary search takes log2(n) steps ... n = 9 => log2(9) ~= 3

ssize_t binary_search (int a[], size_t n, int key)
{
	size_t lo = 0, hi = n - 1;
	while (hi >= lo) {
		size_t mid = (lo + hi) / 2;
		if (a[mid] == key) return mid;
		if (a[mid]  > key) hi = mid - 1;
		if (a[mid] <  key) lo = mid + 1;
	}
	return -1;
}
