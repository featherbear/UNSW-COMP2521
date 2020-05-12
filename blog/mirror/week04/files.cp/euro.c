// Build and display Map/Graph of Europe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "places.h"

int main (void)
{
	Map europe = newMap();
	puts ("Map of Europe");
	puts ("=============");
	showMap(europe);
	return EXIT_SUCCESS;
}
