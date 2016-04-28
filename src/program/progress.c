/* progress.c
** Keep track of where we are in the process of generating
** all the possibilities of the timetable.
** Copyright (c) 11 March 2004 by Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include "progress.h"
#include "memory.h"
#include "../data_types/misc_structures/data.h"

pc progress_constructor (void)
{
  pc newpc = (pc) check_malloc (sizeof(pc_struct));
  return (newpc);
}

void progress_init (pc p)
{
  p->start = time(NULL);
  mpz_init(p->total);
  mpz_fac_ui(p->total, (unsigned long) NUM_CLASSES*PERIODS_PER_WEEK);  
  mpz_init(p->num_finished);
  return;
}

void progress_increment_counter_by_factorial(pc p, unsigned long i)
{
  mpz_t temp;
  mpz_init (temp);
  mpz_fac_ui(temp, i);
  mpz_add(p->num_finished, p->num_finished, temp);
  mpz_clear (temp);
}

void progress_display (pc p)
{
  mpf_t percent_complete_top;
  mpf_t percent_complete_bottom;
  mpf_t delta_t;
  mpf_t avg_speed;
  mpf_t time_remaining;
  mpz_t num_remaining;
  static double timedif = 0;

  time(&p->finish);

  if (difftime(p->finish, p->start) == 0) {
    return;
  }

  if (timedif == difftime(p->finish, p->start)) {
    return;
  }
  
  timedif = difftime(p->finish, p->start);

  mpf_init2 (percent_complete_top, 2048);
  mpf_init2 (percent_complete_bottom, 2048);
  mpf_set_z (percent_complete_top, p->num_finished);
  mpf_set_z (percent_complete_bottom, p->total);
  mpf_div (percent_complete_top, percent_complete_top, percent_complete_bottom);
  mpf_mul_ui (percent_complete_top, percent_complete_top, 100);
  

  mpf_init2 (delta_t, 1024);
  mpf_set_d (delta_t, difftime(p->finish, p->start));

  mpf_init2 (avg_speed, 1024);
  mpf_set_z (avg_speed, p->num_finished); /* conv num_finished to mpf_t */
  mpf_div (avg_speed, avg_speed, delta_t);  /* now we have a valid avg_speed */

  mpz_init (num_remaining);
  mpz_sub (num_remaining, p->total, p->num_finished);

  mpf_init2 (time_remaining, 1024);
  mpf_set_z (time_remaining, num_remaining);
  mpf_div(time_remaining, time_remaining, avg_speed);

  printf("---\n");
  printf("Percent complete: ");
  mpf_out_str(stdout, 10, 2048, percent_complete_top);
  printf("%%\n");

  printf("Progressing at ");
  mpf_out_str(stdout, 10, 3, avg_speed);
  printf(" guesses per second\n");

  printf("Estimating ");
  mpf_out_str(stdout, 10, 3, time_remaining);
  printf(" seconds remaining\n");

  mpf_clear(percent_complete_top);
  mpf_clear(percent_complete_bottom);
  mpf_clear(delta_t);
  mpf_clear(avg_speed);
  mpf_clear(time_remaining);
  mpz_clear(num_remaining);
}

//char* progress_convert_gmp_z_to_standard_form(pc,mpz_t);
