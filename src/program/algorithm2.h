/* algorithm2.h
** Copyright (c) 17 April 2004 by Patrick Cronin
*/

#ifndef TIME_TABLE_ALGORITHMS_3_H
#define TIME_TABLE_ALGORITHMS_3_H

#include <gmp.h>
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/list_structures/ulvlist.h"
#include "../data_types/list_structures/ulvpclist.h"
#include "../data_types/school_structures/stc.h"

void algorithm2_generate_timetable_3(bigthing);
void algorithm2_arrange_stc_days_1(bigthing, int, int, ulvpclist **, mpz_t);
void algorithm2_arrange_stc_days_2(bigthing, int, int, ulvpclist **, mpz_t);
void algorithm2_arrange_stc_days_3(bigthing, int, int, ulvpclist **, mpz_t);
void algorithm2_arrange_stc_days_3_work(bigthing, int, int, ulvpclist **, mpz_t, int **);
int algorithm2_check_stc_types_tried(ulvlist, stc);
int algorithm2_check_stc_errors(bigthing, ulvpclist, stc, int);
void algorithm2_output_stc_arrangement(bigthing, ulvpclist **, mpz_t);
int algorithm2_arrangements_check_teacher_assignments(bigthing, ulvpclist **);
int algorithm2_check_previous_teacher_arrangements(bigthing, int *, stc);

void algorithm_generate_timetable_old_1(bigthing, ulvlist);
void algorithm_generate_timetable_2(bigthing);
int algorithm_check_stc_errors (bigthing, stc);
int algorithm_check_stc_types_tried(ulvlist, stc);
int algorithm_check_subjects_days_left(bigthing, stc);
int algorithm_check_teachers_periods_teaching_in_days(bigthing, stc);
#endif
