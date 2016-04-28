/* ribbit_blocked_cells.c
** Module for ribbit to determine if a subject, teacher, class,
** room, or a class meeting can use a specific space on the time
** table.
** Copyright (c) 3 April 2004 by Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ribbit_blocked_cells.h"
#include "../program/module.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/misc_structures/data.h"
#include "../data_types/misc_structures/bct.h"
#include "../data_types/school_structures/stc.h"

/* NB: The functions in this file could be made to look nicer
** by putting the different type of bcts in a single array,
** and putting the sizes in a different array! */

bct * subject_ids;
int num_subjects_registered;
bct * class_ids;
int num_classes_registered;
bct * teacher_ids;
int num_teachers_registered;
bct * room_ids;
int num_rooms_registered;
bct * meetings_ids;
int num_meetingses_registered;

int mod_initialize (module m)
{
    m->type = MOD_FOR_SUBJECTS | MOD_FOR_CLASSES | MOD_FOR_TEACHERS | MOD_FOR_ROOMS | MOD_FOR_MEETINGS;
    m->attribute = "blocked-cells";

    num_subjects_registered = 0;
    num_classes_registered = 0;
    num_teachers_registered = 0;
    num_rooms_registered = 0;
    num_meetingses_registered = 0;

    subject_ids = NULL;
    class_ids = NULL;
    teacher_ids = NULL;
    room_ids = NULL;
    meetings_ids = NULL;

    return(1);
}

int mod_register_id (int for_mod_type, int id, char * value)
{
    switch(for_mod_type) {
	case MOD_FOR_SUBJECTS:
	    num_subjects_registered++;
	    subject_ids = (bct *) check_realloc (subject_ids, sizeof(bct) * num_subjects_registered);
	    subject_ids[num_subjects_registered-1] = bct_constructor(id, value);
	    break;
	case MOD_FOR_CLASSES:
	    num_classes_registered++;
	    class_ids = (bct *) check_realloc (class_ids, sizeof(bct) * num_classes_registered);
	    class_ids[num_classes_registered-1] = bct_constructor(id, value);
	    break;
	case MOD_FOR_TEACHERS:
	    num_teachers_registered++;
	    teacher_ids = (bct *) check_realloc (teacher_ids, sizeof(bct) * num_teachers_registered);
	    teacher_ids[num_teachers_registered-1] = bct_constructor(id, value);
	    break;
	case MOD_FOR_ROOMS:
	    num_rooms_registered++;
	    room_ids = (bct *) check_realloc (room_ids, sizeof(bct) * num_rooms_registered);
	    room_ids[num_rooms_registered-1] = bct_constructor(id, value);
	    break;
	case MOD_FOR_MEETINGS:
	    num_meetingses_registered++;
	    meetings_ids = (bct *) check_realloc (meetings_ids, sizeof(bct) * num_meetingses_registered);
	    meetings_ids[num_meetingses_registered-1] = bct_constructor(id, value);
	    break;
    }
    
    return(1);
}

int mod_pre_generation(bigthing b)
{
    int i = 0;
    while (i < num_subjects_registered) {
	bct_init2(subject_ids[i], NUM_DAYS * NUM_PERIODS);
	bct_prepare_table(subject_ids[i], b);
	i++;
    }

    i=0;
    while (i < num_classes_registered) {
	bct_init2(class_ids[i], NUM_DAYS * NUM_PERIODS);
	bct_prepare_table(class_ids[i], b);
	i++;
    }

    i=0;
    while (i < num_teachers_registered) {
	bct_init2(teacher_ids[i], NUM_DAYS * NUM_PERIODS);
	bct_prepare_table(teacher_ids[i], b);
	i++;
    }

    i=0;
    while (i < num_rooms_registered) {
	bct_init2(room_ids[i], NUM_DAYS * NUM_PERIODS);
	bct_prepare_table(room_ids[i], b);
	i++;
    }

    i=0;
    while (i < num_meetingses_registered) {
	bct_init2(meetings_ids[i], NUM_DAYS * NUM_PERIODS);
	bct_prepare_table(meetings_ids[i], b);
	i++;
    }
}

/* return 0 if OK, 1 or more if not */
int mod_check_stc (bigthing b, stc cur)
{
    int list_pos;
    int num_teachers;
    int teacher_num;

    /* Is the stc's subject blocked at this time? */
    list_pos = mod_blocked_find_pos_by_id(subject_ids, num_subjects_registered, stc_get_subject(cur));
    if (list_pos != -1) {
	if (bct_is_marked(subject_ids[list_pos], b->pos_day, b->pos_period)) {
	    /* printf("blocked subject %d from class %d on day %d at period %d\n", stc_get_subject(cur), b->pos_class, b->pos_day, b->pos_period); */
	    return(1);
	}
    }

    /* Is the stc's class blocked at this time? */
    list_pos = mod_blocked_find_pos_by_id(class_ids, num_classes_registered, stc_get_class(cur));
    if (list_pos != -1) {
	if (bct_is_marked(class_ids[list_pos], b->pos_day, b->pos_period)) {
	    /* printf("blocked subject %d from class %d on day %d at period %d\n", stc_get_subject(cur), b->pos_class, b->pos_day, b->pos_period); */
	    return(1);
	}
    }

    /* Are any of the stc's teachers blocked at this time? */
    num_teachers = stc_get_num_teachers(cur);
    for(teacher_num=0;teacher_num < num_teachers; teacher_num++) {
	list_pos = mod_blocked_find_pos_by_id(teacher_ids, num_teachers_registered, stc_get_teacher_n(cur, teacher_num));
	if (list_pos != -1) {
	    if (bct_is_marked(teacher_ids[list_pos], b->pos_day, b->pos_period)) {
		/* printf("blocked subject %d from class %d on day %d at period %d\n", stc_get_subject(cur), b->pos_class, b->pos_day, b->pos_period); */
		return(1);
	    }
	}
    }

    /* Is the stc's room blocked at this time? */
    list_pos = mod_blocked_find_pos_by_id(subject_ids, num_rooms_registered, stc_get_room(cur));
    if (list_pos != -1) {
	if (bct_is_marked(room_ids[list_pos], b->pos_day, b->pos_period)) {
	    /* printf("blocked subject %d from class %d on day %d at period %d\n", stc_get_subject(cur), b->pos_class, b->pos_day, b->pos_period); */
	    return(1);
	}
    }

    /* Is the stc's id blocked at this time? */
    list_pos = mod_blocked_find_pos_by_id(meetings_ids, num_meetingses_registered, stc_get_id(cur));
    if (list_pos != -1) {
	if (bct_is_marked(meetings_ids[list_pos], b->pos_day, b->pos_period)) {
	    /* printf("blocked subject %d from class %d on day %d at period %d\n", stc_get_subject(cur), b->pos_class, b->pos_day, b->pos_period); */
	    return(1);
	}
    }
    
    return(0);
}

int mod_blocked_find_pos_by_id(bct * list_of_things, int total, int id)
{
    int i = 0;
    while (i < total) {
	if (bct_get_id(list_of_things[i]) == id) {
	    return (i);
	}
	i++;
    }

    /* it is not here */
    return(-1);
}
