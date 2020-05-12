#include <string.h>
#include <stdio.h>

#include "ht.h"

int main (void)
{
	size_t ht_size = 17;
	ht *ht = ht_new (ht_size);
	ht_show (ht);

	char buf[BUFSIZ];
	while (fgets (buf, BUFSIZ, stdin) != NULL) {
		// lose the newline
		*(strrchr (buf, '\n')) = '\0';
		// split on tab
		char *val = strchr (buf, '\t');
		*val = '\0';
		val++;

		//printf ("'%s' [%zu]\n", buf, key_hash (buf, ht_size));
		ht_insert (ht, buf, val);
	}

	ht_show (ht);
}
