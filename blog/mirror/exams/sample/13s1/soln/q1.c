// recursive function to find maximum in a[lo..hi]
int max(int a[], int lo, int hi)
{
	//printf("a[%d]=%d, a[%d]=%d\n",lo,a[lo],hi,a[hi]);
	if (lo >= hi)
		return a[hi];
	else if (a[lo] < a[hi])
		return max(a, lo+1, hi);
	else if (a[lo] > a[hi])
		return max(a, lo, hi-1);
	else // (a[lo] == a[hi])
		return max(a, lo, hi-1);
}
