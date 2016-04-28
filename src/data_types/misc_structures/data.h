/* data.h
** Miscellaneous important data definitions
** Copyright (c) 11 March 2004 by Patrick Cronin
*/

#ifndef RIBBIT_DATA
#define RIBBIT_DATA

#include "../list_structures/sandndlist.h"
#include "../list_structures/ulvlist.h"

int NUM_DAYS;
int NUM_PERIODS;
int NUM_TUPLES;
int PERIODS_PER_WEEK;
int NUM_SUBJECTS;
int NUM_TEACHERS;
int NUM_CLASSES;
int NUM_ROOMS;

struct _program_data {
    char * progname;
    char * configfile;
    char * outputprefix; 
    sandndlist subjects;
    sandndlist teachers;
    sandndlist classes;
    sandndlist rooms;
    ulvlist meetingses;
    sandndlist modules;
    sandndlist days;
    long max_num_arrangements;
};
typedef struct _program_data program_data_struct;
typedef struct _program_data* program_data;

program_data data_initialize(void);
void data_calculate_global_helpers(program_data);

#endif
