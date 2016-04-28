/* ttt.c
** Teacher Time Table functions file.
** Defines the functions of an ttt data type.
** Patrick Cronin
** Copyright (c) 5 March 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "../../program/memory.h"
#include "stc.h"
#include "ttt.h"
#include "../list_structures/sandndlist.h"
#include "../misc_structures/data.h"
#include "../misc_structures/bigthing.h"
#include "class.h"
#include "subject.h"
#include "teacher.h"
#include "room.h"


teachertimetable ttt_constructor(void)
{
    teachertimetable newttt = (teachertimetable) check_malloc (sizeof(teachertimetable_struct));
    ttt_init(newttt);
    return(newttt);
}

void ttt_init(teachertimetable t)
{
    t->timetable = (stc *) check_calloc (PERIODS_PER_WEEK, sizeof(stc));
}

stc ttt_get_entry_at(teachertimetable t, int day_num, int period_num)
{
    int loc = ttt_make_loc_int(t, day_num, period_num);
    ttt_check_entry_location(t, loc);
    return (t->timetable[loc]);
}

stc ttt_pop_entry_at(teachertimetable t, int day_num, int period_num)
{
    stc removed;
    removed = ttt_get_entry_at(t, day_num, period_num);
    ttt_set_entry_at(t, day_num, period_num, NULL);
    return(removed);
}

void ttt_set_entry_at(teachertimetable t, int day_num, int period_num, stc mystc)
{
    int loc = ttt_make_loc_int(t, day_num, period_num);
    ttt_check_entry_location(t, loc);
    t->timetable[loc] = mystc;
}

int ttt_make_loc_int(teachertimetable t, int day_num, int period_num)
{
    return ((day_num * NUM_PERIODS) + period_num);
}

void ttt_check_entry_location(teachertimetable t, int loc)
{
    if (loc > PERIODS_PER_WEEK) {
	fprintf(stderr,"ttt: Location for requested tuple is too large for this time table [fatal].\n(Max size: %d, requested location: %d)\n", PERIODS_PER_WEEK - 1, loc);
	exit(-1);
    }
    return;
}

void ttt_display(teachertimetable t)
{
    printf("No display function yet for ttt datatype.\n");
}

/* This function only works for teachers that teach 1 period per period
** i.e. are not teaching two different classes at the same time */
teachertimetable ttt_make_teacher_table(mastertimetable tt, program_data pd, int teacher_id)
{
    int class_id, day_id, period_id;
    stc temp_stc;

    teachertimetable newtable = ttt_constructor();

    for (class_id=0; class_id < NUM_CLASSES; class_id++) {
	for (day_id=0; day_id < NUM_DAYS; day_id++) {
	    for (period_id=0; period_id < NUM_PERIODS; period_id++) {
		temp_stc = mtt_get_entry_at(tt, class_id, day_id, period_id);
		if (temp_stc != NULL) {
		    if (stc_is_teaching(temp_stc, teacher_id)) {
			ttt_set_entry_at(newtable, day_id, period_id, temp_stc);
		    }
		}
	    }
	}
    }

    return (newtable);    
}


void ttt_write_teacher_table(teachertimetable t, program_data pd, int teacher_id, FILE * output)
{
    int p, d;
    stc the_stc;
    
    fprintf(output, "<h2>%s's Time Table</h2>\n", sandndlist_find_name_by_id(pd->teachers, teacher_id));
    fprintf(output, "<table border=2 cellpadding=5>\n");
    for (p=-1; p<NUM_PERIODS; p++) { /* rows */
	
	if (p == -1) {
	    /* for the first row: class names */
	    fprintf(output, "\t<tr bgcolor=#bbbbbb>\n");
	} else {
	    /* for the other rows */
	    fprintf(output, "\t<tr>\n");
	}
	
	for (d=-1; d<NUM_DAYS; d++) { /* columns */
	    if ((p == -1) && (d == -1)) {
		/* for the first row and first column */
		fprintf(output, "\t\t<td></td>\n");
	    } else if (d == -1) {
		/* for the rest of the first row */
		fprintf(output, "\t\t<th bgcolor=\"#ddddbb\">Period %d</th>\n", p + 1);
	    } else if (p == -1) {
		/* for all of the first column cells */
		fprintf(output, "\t\t<th>%s</th>\n", sandndlist_find_name_by_id(pd->days, d));
	    } else {
		/* an STC cell */
		the_stc = ttt_get_entry_at(t, d, p);
		if (the_stc) {
		    fprintf(output, "\t\t<td><b>%s</b><br>%s<br>\n", sandndlist_find_name_by_id(pd->subjects, the_stc->subject), sandndlist_find_name_by_id(pd->classes, stc_get_class(the_stc)));
		    if (stc_get_room(the_stc) != -1) {
			fprintf(output, "\t\t<i>%s</i><br>\n", sandndlist_find_name_by_id(pd->rooms, stc_get_room(the_stc)));
		    }
		    
		    /* fprintf(output, "\t\tSTC_ID: %d\n", stc_get_id(the_stc)); */
		    fprintf(output, "\t\t</td>\n");
		} else {
		    /* this teacher has a free period now */
		    fprintf(output, "\t\t<td align=\"center\"><b>Free</b></td>\n");
		}
	    }
	}
	fprintf(output, "\t</tr>\n");
    }
    fprintf(output, "</table>\n<br>\n<br>\n<br>\n");
}

void ttt_free_shallow(teachertimetable t)
{
    free(t->timetable);
}
