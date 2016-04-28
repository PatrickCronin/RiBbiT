/* ribbit_teacher_sametime.c
** Module for ribbit to determine if a teacher is already
** teaching on this day.
** Copyright (c) 2 April 2004 by Patrick Cronin
*/

#include <stdio.h>
#include "../program/module.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/school_structures/mtt.h"
#include "../data_types/school_structures/stc.h"


int mod_initialize (module m)
{
    m->type = MOD_GENERAL;
    return(1);
}

/* return 1 if OK, 0 if not */
int mod_check_stc (bigthing b, stc cur)
{
    int class;
    int teacher_num;
    int num_teachers;
    stc temp_stc;

    if (stc_is_free_period(cur)) {
	return(0);
    }

    class = b->pos_class - 1;
    num_teachers = stc_get_num_teachers(cur);

    while (class >= 0) {
	temp_stc = mtt_get_entry_at(b->tt, class, b->pos_day, b->pos_period);
	teacher_num = 0;
	/* check all teachers that are in the new stc */
	while (teacher_num < num_teachers) {
	    if (stc_is_teaching(temp_stc, stc_get_teacher_n(cur, teacher_num))) {
		return (1);
	    }
	    teacher_num++;
	}
	class--;
    }
    return(0);
}
