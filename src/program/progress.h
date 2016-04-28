/* progress.h
** Keep track of where we are in the process of generating
** all the possibilities of the timetable.
** Copyright (c) 11 March 2004 by Patrick Cronin
*/

/* This structure really doesn't need to be object-oriented. */

#ifndef PROGRESS_COUNTER
#define PROGRESS_COUNTER

#include <time.h>
#include <gmp.h>

struct _progress_counter {
  time_t start;
  time_t finish;
  mpz_t total;
  mpz_t num_finished;
};
typedef struct _progress_counter pc_struct;
typedef struct _progress_counter* pc;

pc progress_constructor(void);
void progress_init(pc);
void progress_increment_counter_by_factorial(pc, unsigned long);
void progress_display (pc);
//char* progress_convert_gmp_z_to_standard_form(pc,mpz_t);


#endif
