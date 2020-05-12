#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "places.h"

int main (int argc, char *argv[])
{
	if (argc < 2)
		errx (EX_USAGE, "usage: %s <name|abbrev>", argv[0]);

	int id;
	if (strlen(argv[1]) > 2) {
		id = nameToID (argv[1]);
		if (id != NOWHERE)
			printf("%s has ID %d\n", argv[1], id);
	} else {
		id = abbrevToID(argv[1]);
		if (id != NOWHERE)
			printf("%s is %s (%d)\n", argv[1], idToName(id), id);
	}

	if (id == NOWHERE)
		printf ("Invalid place '%s'\n", argv[1]);

	return EXIT_SUCCESS;
}
