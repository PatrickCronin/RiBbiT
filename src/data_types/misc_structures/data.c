/* data.c
** Miscellaneous important data functions
** Copyright (c) 11 March 2004 by Patrick Cronin
*/

#include "data.h"
#include "stdlib.h"
#include "../../program/memory.h"
#include "../school_structures/subject.h"
#include "../school_structures/teacher.h"
#include "../school_structures/class.h"
#include "../school_structures/room.h"
#include "../school_structures/meetings.h"
#include "../school_structures/stc.h"
#include "../school_structures/day.h"
#include "../list_structures/sandndlist.h"
#include "../list_structures/ulvlist.h"
#include "../../program/module.h"


program_data data_initialize(void)
{
    program_data d;

    /* init global variables */
    NUM_DAYS = 0;
    NUM_PERIODS = 0;
    NUM_TUPLES = 0;
    PERIODS_PER_WEEK = 0;
    NUM_SUBJECTS = 0;
    NUM_TEACHERS = 0;
    NUM_CLASSES = 0;
    NUM_ROOMS = 0;

    /* init program data */
    d = (program_data) check_malloc (sizeof(program_data_struct));
    d->configfile = NULL;
    d->outputprefix = NULL;
    d->progname = NULL;
    d->subjects = sandndlist_constructor(subject_compare_names, subject_get_name, subject_free, subject_display);
    d->teachers = sandndlist_constructor(teacher_compare_names, teacher_get_name, teacher_free, teacher_display);
    d->classes = sandndlist_constructor(class_compare_names, class_get_name, class_free, class_display);
    d->rooms = sandndlist_constructor(room_compare_names, room_get_name, room_free, room_display);
    d->meetingses = ulvlist_constructor(meetings_free, meetings_display, NULL, NULL, NULL);
    d->modules = sandndlist_constructor(module_compare_names, module_get_name, module_free, module_display);
    d->days = sandndlist_constructor(day_compare_names, day_get_name, day_free, day_display);
    return (d);
}

void data_calculate_global_helpers(program_data d)
{
    NUM_SUBJECTS = sandndlist_size(d->subjects);
    NUM_TEACHERS = sandndlist_size(d->teachers);
    NUM_CLASSES = sandndlist_size(d->classes);
    NUM_ROOMS = sandndlist_size(d->rooms);
    PERIODS_PER_WEEK = NUM_PERIODS * NUM_DAYS;
    NUM_TUPLES = NUM_CLASSES * PERIODS_PER_WEEK;
}


