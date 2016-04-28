/* algorithm.h
** Copyright (c) 17 April 2004 by Patrick Cronin
*/

#ifndef TIME_TABLE_ALGORITHMS_2_H
#define TIME_TABLE_ALGORITHMS_2_H

#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/list_structures/ulvlist.h"
#include "../data_types/school_structures/stc.h"

void algorithm_generate_timetable_old_1(bigthing, ulvlist);
void algorithm_generate_timetable_2(bigthing);
int algorithm_check_stc_errors (bigthing, stc);
int algorithm_check_stc_types_tried(ulvlist, stc);
int algorithm_check_subjects_days_left(bigthing, stc);
int algorithm_check_teachers_periods_teaching_in_days(bigthing, stc);
#endif
