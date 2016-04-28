/* ctt.c
** Character Time Table functions file.
** Defines the functions of an ctt data type.
** Patrick Cronin
** Copyright (c) 5 March 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include "../../program/memory.h"
#include "ctt.h"
#include "data.h"
#include "bigthing.h"

charactertimetable ctt_constructor(void)
{
    charactertimetable newctt = (charactertimetable) check_malloc (sizeof(charactertimetable_struct));
    ctt_init(newctt);
    return(newctt);
}

void ctt_init(charactertimetable c)
{
    c->timetable = (char *) check_calloc (NUM_TUPLES, sizeof(char));
}

char ctt_get_entry_at(charactertimetable c, int class_id, int day_num, int period_num)
{
    int loc = ctt_make_loc_int(c, class_id, day_num, period_num);
    ctt_check_entry_location(c, loc);
    return ((c->timetable)[loc]);
}

char ctt_pop_entry_at(charactertimetable c, int class_id, int day_num, int period_num)
{
    char removed;
    removed = ctt_get_entry_at(c, class_id, day_num, period_num);
    ctt_set_entry_at(c, class_id, day_num, period_num, '\0');
    return(removed);
}

void ctt_set_entry_at(charactertimetable c, int class_id, int day_num, int period_num, char mychar)
{
    int loc = ctt_make_loc_int(c, class_id, day_num, period_num);
    ctt_check_entry_location(c, loc);
    c->timetable[loc] = mychar;
}

int ctt_make_loc_int(charactertimetable c, int class_id, int day_num, int period_num)
{
    return ((class_id * PERIODS_PER_WEEK) + (day_num * NUM_PERIODS) + period_num);
}

void ctt_check_entry_location(charactertimetable c, int loc)
{
    if (loc > NUM_TUPLES) {
	fprintf(stderr,"ctt: Location for requested tuple is too large for this time table (fatal).\n(Max size: %d, requested location: %d)\n", NUM_TUPLES - 1, loc);
	exit(-1);
    }
    return;
}

void ctt_display(charactertimetable c)
{
    printf("No display function yet for ctt datatype.\n");
}
