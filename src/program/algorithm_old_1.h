/* algorithm.h
** Copyright (c) 11 March 2004 by Patrick Cronin
*/

#ifndef TIME_TABLE_ALGORITHMS
#define TIME_TABLE_ALGORITHMS

#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/list_structures/ulvlist.h"
#include "../data_types/school_structures/stc.h"

void algorithm_generate_timetable(bigthing, ulvlist);
int algorithm_check_stc_errors (bigthing, stc);
int algorithm_check_stc_types_tried(ulvlist, stc);

#endif
