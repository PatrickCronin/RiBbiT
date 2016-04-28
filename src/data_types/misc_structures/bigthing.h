/* bigthing.h
** A structure of random things needed in the recursive algorithm
** for generating the timetable.  We need to put everything in the
** bigthing structure so that the recursive function has a minimum
** number of parameters, and therefore the size of the function stack
** doesn't grow completely out of control during the recursion.
** Copyright (c) 17 April 2004
*/

#ifndef BIG_THING_2_H
#define BIG_THING_2_H

#include <gmp.h>
#include "../school_structures/mtt.h"
#include "../list_structures/muvdalist.h"
#include "data.h"

struct _bigthing {
    /* needed for the recursion */
    mastertimetable tt;
    int depth;
    int pos_class;
    int pos_day;
    int pos_period;
    mpz_t num_solutions;
    muvdalist *stcs;

    int ** subjects_days_left;
    int ** teachers_periods_teaching_in_days;
    program_data pd;
    int stop_arrangements_flag;
};
typedef struct _bigthing bigthing_struct;
typedef struct _bigthing* bigthing;

bigthing bigthing_constructor(void);
void bigthing_init1(bigthing);
void bigthing_init2(bigthing);
void bigthing_advance_position(bigthing);
void bigthing_reverse_position(bigthing);
int bigthing_positions_completed(bigthing);
int bigthing_positions_almost_completed(bigthing);
void bigthing_prepare_subject_days_left(bigthing);
int bigthing_prepare_stcs(bigthing);
void bigthing_create_necessary_free_periods(bigthing, int);
void bigthing_sanity_check(bigthing);
void bigthing_check_teachers_periods(bigthing);
#endif

