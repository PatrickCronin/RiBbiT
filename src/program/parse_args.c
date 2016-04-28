/* parse_args.c
** Functions to parse the command line arguments
** Copyright (c) 13 March 2004 by Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_args.h"
#include "memory.h"
#include "../data_types/misc_structures/data.h"

void parse_program_arguments(program_data d, int argc, char *argv[])
{
    int icount, ocount;
    char ** end_ptr;
    d->progname = *argv;
    argv++;

    icount = 0;
    ocount = 0;

    end_ptr = (char **) check_malloc (sizeof(char *));
    *end_ptr = (char *) check_malloc (sizeof(char));

    while (*argv != NULL) {
	if (strcmp("-i", *argv) == 0) {
	    icount++;
	    argv++;
	    d->configfile = *argv;
	    argv++;
	}
	else if (strcmp("-o", *argv) == 0) {
	    ocount++;
	    argv++;
	    d->outputprefix = *argv;
	    argv++;
	}
	else if (strcmp("-a", *argv) == 0) {
	    argv++;
	    d->max_num_arrangements = strtol(argv[0], end_ptr, 10);
	    if ((*end_ptr[0] != '\0') ||
		(d->max_num_arrangements < 1)) {
	      fprintf(stderr, "invalid input for maximum number of arrangements\n");
	      exit(-1);
	    }
	    argv++;
	}
    }
    
    if ( (icount != 1) || (ocount != 1) ) {
	print_usage(d->progname);
    }
    
}

void print_usage(char * progname)
{
    fprintf(stderr,"usage: %s -i infile -o prefix_for_output_files\n", progname);
    exit(-1);
}
