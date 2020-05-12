////////////////////////////////////////////////////////////////////////
// COMP2521 19x1 ... Assignment 1: A Simple Text Buffer
//
// 2018-11-01	v0.1	Team Textbuffer <cs2521@cse.unsw.edu.au>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "textbuffer.h"

static Textbuffer load_file (const char *filename);


int
main (int argc, char **argv)
{
	Textbuffer buf;

	if (argc == 1)
		buf = textbuffer_new ("");
	else if (argc == 2)
		buf = load_file (argv[1]);
	else
		errx (EX_USAGE, "usage: %s [<filename>]", argv[0]);

	char line[BUFSIZ] = {};
	bool insert_mode = false;
	while (fgets (line, BUFSIZ, stdin) != NULL) {
		if (insert_mode && strcmp (line, ".") != 0) {
			textbuffer_insert_line (buf, line);
			continue;
		}

		switch (line[0]) {
		case '.': break;
		
		}
	}

	return EXIT_SUCCESS;
}


static Textbuffer
load_file (const char *filename)
{
	Textbuffer buf;

	int fd;
	if ((fd = open (filename, O_RDONLY)) <= 0) {
		warn ("couldn't open '%s'", filename);
		return buf = textbuffer_new ("");
	}

	struct stat sb;
	if (fstat (fd, &sb) != 0) {
		warn ("couldn't stat '%s'", filename);
		close (fd);
		return buf = textbuffer_new ("");
	}

	size_t file_size = (size_t) sb.st_size;

	char *contents;
	if ((contents = mmap (
			 NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0
		)) == NULL)
	{
		warn ("couldn't map '%s'", filename);
		close (fd);
		return buf = textbuffer_new ("");
	}

	buf = textbuffer_new (contents);

	munmap (contents, file_size);
	close (fd);
	return buf;
}
