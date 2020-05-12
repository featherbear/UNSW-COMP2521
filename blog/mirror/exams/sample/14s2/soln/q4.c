#include <stdlib.h>
#include <stdio.h>

#define NOT_FOUND (-1)

int ncalls;

int binary_search(int key, int a[], int lo, int hi)
{
	ncalls++;
    if (lo > hi)
		return NOT_FOUND;
	else if (lo == hi) {
		if (key == a[lo])
			return lo;
		else
			return NOT_FOUND;
    }
	else {
		int mid = (lo+hi)/2;
		if (key < a[mid])
			return binary_search(key, a, lo, mid-1);
		else if (key > a[mid])
			return binary_search(key, a, mid+1, hi);
		else // (key == a[mid])
			return mid;
	}
}

int main(int argc, char *argv[])
{
	int i, j, k, a[100], N = 10;

	N = atoi(argv[1]);
	printf("%d\n",N);
	for (i = 0, j = 0; i < N; i++, j+= 2) a[i] = j;
	for (k = 1; k <= N; k++) {
		ncalls = 0;
		i = binary_search(k, a, 0, N-1);
		printf("for %d, [%d] in %d calls\n",k,i,ncalls);
	}
	return 0;
}
