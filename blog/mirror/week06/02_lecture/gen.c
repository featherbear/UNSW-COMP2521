// gen.c: a program for generating sortable data

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 10000000

#define swap(A, I, J) \
	{ int tmp; tmp = A[(I)]; A[(I)] = A[(J)]; A[(J)] = tmp; }

static char *progName;
static void give_up (char *);

int main (int argc, char **argv)
{
	int N = 0; // number of values of data produced
	int order; // ordering of data
	int S = 0; // seed for random #'s
	progName = argv[0];

	if (argc < 3)
		give_up ("Not enough arguments");

	// how much data
	N = atoi (argv[1]);
	if (N < 2)
		give_up ("Too few values\n");
	if (N > MAXLINES - 1)
		give_up ("Too many values");

	// determine ordering
	switch (argv[2][0]) {
	case 'A':	case 'a':	order = 'A';	break;
	case 'D':	case 'd':	order = 'D';	break;
	case 'R':	case 'r':	order = 'R';	break;
	default:	give_up ("Invalid ordering");	break;
	}

	// set seed
	if (argc == 4 && order == 'R') {
		switch (order) {
		case 'A':	S = 1;	break;
		case 'D':	S = N + 1;	break;
		case 'R':	srand (S = atoi (argv[3])); break;
		}
	}

	int *values;
	if ((values = malloc (N * sizeof (int))) == NULL)
		give_up ("Can't allocate values[]");
	for (int i = 0; i < N; i++)
		values[i] = i;

	switch (order) {
	case 'D':
		for (int i = 0; i < N / 2; i++)
			swap (values, i, N - i - 1);
		break;

	case 'A':
		/* nothing to do */
		break;

	case 'R':
		for (int i = 0; i < N; i++) {
			int j = rand () % N;
			swap (values, i, j);
		}
		break;
	}

	printf ("%d\n", N);
	for (int i = 0; i < N; i++)
		printf ("%d\n", values[i]);

	return EXIT_SUCCESS;
}

static void give_up (char *message)
{
	fprintf (stderr, "%s\n", message);
	fprintf (stderr, "Usage: %s <n-values> (A|D|R) [seed]\n", progName);
	fprintf (stderr, "       A|D|R = Ascending|Descending|Random\n");
	exit (EXIT_FAILURE); // don't distinguish different errors
}
