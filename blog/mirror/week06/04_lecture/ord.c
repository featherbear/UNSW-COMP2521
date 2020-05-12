#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

typedef int Item;
#define key(A) (A)
#define less(A,B) (key(A) < key(B))

static inline void swap_idx (Item items[], size_t a, size_t b);

void sort_bubble (Item items[], size_t lo, size_t hi);
void sort_selection (Item items[], size_t lo, size_t hi);
void sort_insertion (Item items[], size_t lo, size_t hi);
void sort_shell (Item items[], size_t lo, size_t hi);
size_t shell_h (size_t lo, size_t hi);

void sort_merge (Item items[], size_t lo, size_t hi);
static void merge (Item items[], size_t lo, size_t mid, size_t hi);
static int merge_calls = 0;

void sort_quick (Item items[], size_t lo, size_t hi);
static size_t partition (Item items[], size_t lo, size_t hi);

void show_array (Item *items, size_t n);
void print_array (Item *items, size_t n);

int main (void)
{
	size_t n_items;
	scanf ("%zu", &n_items);

	Item *items = calloc (n_items, sizeof (Item));
	if (items == NULL)
		err (EX_OSERR, "couldn't allocate %zu items", n_items);

	for (size_t i = 0; i < n_items; i++)
		scanf ("%d", &items[i]);

	// print_array (items, n_items);
	// sort_bubble (items, 0, n_items - 1);
	// sort_selection (items, 0, n_items - 1);
	// sort_insertion (items, 0, n_items - 1);
	// sort_shell (items, 0, n_items - 1);
	// sort_merge (items, 0, n_items - 1);
	// printf ("sort_merge called %d times\n", merge_calls);
	// int merge_recursion = log2 (n_items);
	// printf ("sort_merge recursed %d levels deep\n", merge_recursion);

	sort_quick (items, 0, n_items - 1);
	print_array (items, n_items);

#if 0
	for (size_t h = shell_h (0, 10000); h > 0; h /= 3)
		printf ("h is now %zu\n", h);
#endif

	return EXIT_SUCCESS;
}

//  { 4, 1, 7, 3, 8, 6, 5, 2 }
//       ^ j               ^ i
//    ^~~~
void sort_bubble (Item items[], size_t lo, size_t hi)
{
	for (size_t i = hi; i > lo; i--) {
		printf ("%d\n", i);
		for (size_t j = lo + 1; j <= i; j++) {
			if (less (items[j], items[j - 1])) {
				swap_idx (items, j, j-1);
			}
			show_array (&items[lo], hi + 1);
		}
	}

	// #define less(A,B) (key(A) < key(B))
	// #define key(A) (A)
	//      if (key (items[j]) < key (items[j - 1]))
	//      if ((items[j]) < (items[j - 1]))
}

void sort_selection (Item items[], size_t lo, size_t hi)
{
	show_array (&items[lo], hi + 1);

	for (size_t i = lo; i < hi; i++) {
		size_t min = i;
		for (size_t j = i + 1; j <= hi; j++)
			if (less (items[j], items[min]))
				min = j;
		swap_idx (items, i, min);
		show_array (&items[lo], hi + 1);
	}
}

void sort_insertion (Item items[], size_t lo, size_t hi)
{
	show_array (&items[lo], hi + 1);
	for (size_t i = lo + 1; i <= hi; i++) {
		Item item = items[i];
		size_t j = i;
		for (; j > lo; j--) {
			if (! less (item, items[j - 1])) break;
			else items[j] = items[j - 1];
			printf ("\t"); show_array (&items[lo], hi + 1);
		}
		items[j] = item;

		show_array (&items[lo], hi + 1);
	}	
}

size_t shell_h (size_t lo, size_t hi)
{
	size_t n = hi - lo + 1;
	size_t h = 1;
	for (; h <= (n - 1) / 9; h = (3 * h) + 1) {
		printf ("h = %zu\n", h);
	}
	return h;
}

void sort_shell (Item items[], size_t lo, size_t hi)
{
	size_t n = hi - lo + 1;
	size_t h = 1;
	for (; h <= (n - 1) / 9; h = (3 * h) + 1) {
		printf ("h = %zu\n", h);
	}

	puts ("");
	for (/* h */; h > 0; h /= 3) {
		printf ("h = %zu\n", h);
		show_array (&items[lo], hi + 1);

		for (size_t i = h; i < n; i++) {
			Item item = items[i];
			size_t j = i;
			for (/* j */; j >= h && item < items[j - h]; j -= h)
				items[j] = items[j - h];
			items[j] = item;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void sort_merge (Item items[], size_t lo, size_t hi)
{
	merge_calls++;
	if (hi <= lo) return;
	size_t mid = (lo + hi) / 2;
	sort_merge (items, lo, mid);
	sort_merge (items, mid + 1, hi);
	merge (items, lo, mid, hi);
}

static void merge (Item items[], size_t lo, size_t mid, size_t hi)
{
	Item *tmp = calloc (hi - lo + 1, sizeof (Item));
	size_t i = lo, j = mid + 1, k = 0;

	while (i <= mid && j <= hi)
		tmp[k++] 
			= less (items[i], items[j]) 
			? items[i++] 
			: items[j++];

	while (i <= mid) tmp[k++] = items[i++];
	while (j <= hi)  tmp[k++] = items[j++];

	for (i = lo, k = 0; i <= hi; items[i++] = tmp[k++]);

	free (tmp);
}

//////////////////////////////////////////////////////////////////////

void sort_quick (Item items[], size_t lo, size_t hi)
{
	if (hi <= lo) return;
	size_t part = partition (items, lo, hi);
	sort_quick (items, lo, (part == 0) ? 0 : (part - 1));
	sort_quick (items, part + 1, hi);
}

static size_t partition (Item items[], size_t lo, size_t hi)
{
	Item v = items[lo];
	size_t i = lo + 1, j = hi;

	while (true) {
		while (less (items[i], v) && i < j) i++;
		while (less (v, items[j]) && i < j) j--;
		if (i == j) break;
		swap_idx (items, i, j);
	}

	j = less (items[i], v) ? i : i - 1;
	swap_idx (items, lo, j);
	return j;
}

//////////////////////////////////////////////////////////////////////

static inline void swap_idx (Item items[], size_t a, size_t b)
{
	Item t = items[b]; items[b] = items[a]; items[a] = t;
}

void print_array (Item *items, size_t n)
{
	// printf ("{ ");
	for (size_t i = 0; i < n; i++)
		printf ("%d\n", items[i]);
	// printf (" }\n");
}

void show_array (Item *items, size_t n)
{
	printf ("{ ");
	for (size_t i = 0; i < n; i++)
		printf ("%d, ", items[i]);
	printf (" }\n");
}