// Determine if/how two Places are adjacent

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "map.h"
#include "places.h"

int main (int argc, char **argv)
{
	if (argc < 3)
		errx (EX_USAGE, "usage: %s <place1> <place2>", argv[0]);

	// convert args to place IDs
	int id1 = (strlen (argv[1]) == 2)
		? abbrevToID (argv[1])
		: nameToID (argv[1]);
	int id2 = (strlen (argv[2]) == 2)
		? abbrevToID (argv[2])
		: nameToID (argv[2]);

	// check place validity
	if (id1 == NOWHERE)
		errx (EX_SOFTWARE, "invalid place name: %s", argv[1]);

	if (id2 == NOWHERE)
		errx (EX_SOFTWARE, "invalid place name: %s", argv[2]);

	Map europe = newMap ();

	// check for direct connection
	TransportID t[MAX_TRANSPORT];

	printf ("Between %s and %s ...\n", idToName (id1), idToName (id2));
	int n = connections (europe, id1, id2, t);
	if (n == 0)
		printf ("No direct connection\n");
	else {
		for (int i = 0; i < n; i++) {
			switch (t[i]) {
			case ROAD:	puts ("Road connection"); break;
			case RAIL:	puts ("Rail connection"); break;
			case BOAT:	puts ("Boat connection"); break;

			case ANY:
			case NONE:
			default:	assert (!! "unreachable");
			}
		}
	}

	return EXIT_SUCCESS;
}
