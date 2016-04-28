/* progress.h
** Keep track of where we are in the process of generating
** all the possibilities of the timetable.
** Copyright (c) 11 March 2004 by Patrick Cronin
*/

/* This structure really doesn't need to be object-oriented. */

#ifndef PROGRESS_COUNTER
#define PROGRESS_COUNTER

#include <gmp.h>

struct _progress_counter {
    gmp_z counter;
    gmp_z total;
};
typedef struct _progress_counter pc_struct;
typedef struct _progress_counter* pc;

pc progress_constructor(void);
void progress_init(pc);
void progress_increment_counter_by_factorial(pc,int);
char* progress_convert_gmp_z_to_standard_form(pc,gmp_z);


#endif
