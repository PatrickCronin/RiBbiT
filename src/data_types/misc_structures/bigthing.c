/* bigthing.c
** Copyright (c) 17 April 2004 by Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "bigthing.h"
#include "data.h"
#include "../../program/memory.h"
#include "../../program/module.h"
#include "../../program/progress.h"
#include "../school_structures/mtt.h"
#include "../school_structures/meetings.h"
#include "../school_structures/subject.h"
#include "../school_structures/teacher.h"
#include "../school_structures/class.h"
#include "../school_structures/stc.h"
#include "../list_structures/muvdalist.h"
#include "../list_structures/ulvlist.h"
#include "../list_structures/sandndlist.h"
#include "../list_structures/silist.h"

#define CLASS_STATS
#define TEACHER_STATS
//#define DEBUG

bigthing bigthing_constructor(void)
{
    bigthing newbigthing = (bigthing) check_malloc (sizeof(bigthing_struct));
    bigthing_init1(newbigthing);
    return(newbigthing);
}

void bigthing_init1(bigthing b)
{
    b->pd = NULL;
}

void bigthing_init2(bigthing b)
{
    int i;
    b->tt = mtt_constructor();
    b->depth = 0;
    b->pos_class = 0;
    b->pos_day = 0;
    b->pos_period = 0;
    mpz_init_set_str(b->num_solutions, "0", 10);
    b->stcs = NULL;
    b->subjects_days_left = (int **) check_malloc (sizeof(int *) * NUM_CLASSES);
    for (i = 0; i < NUM_CLASSES; i++) {
	b->subjects_days_left[i] = (int *) check_calloc (NUM_SUBJECTS + 1, sizeof(int));
    }
    b->teachers_periods_teaching_in_days = (int **) check_malloc (sizeof(int *) * (NUM_TEACHERS + 1));
    for (i=0; i < (NUM_TEACHERS + 1); i++) {
	b->teachers_periods_teaching_in_days[i] = (int *) check_calloc (NUM_DAYS, sizeof(int));
    }
    b->progress = progress_constructor();
}

void bigthing_prepare_subject_days_left(bigthing b)
{
    meetings m;
    int c, s, p;

    if(ulvlist_init_pos(b->pd->meetingses)) {
	do {
	    m = ulvlist_data_at_pos(b->pd->meetingses);
	    c = meetings_get_class(m);
	    s = meetings_get_subject(m);
	    p = silist_get_size(meetings_get_periods_arrangement(m));
	    b->subjects_days_left[c][s] = p;
	} while (ulvlist_move_pos(b->pd->meetingses));
    }
    
}

void bigthing_advance_position(bigthing b)
{
    b->pos_period++;
    if (b->pos_period >= NUM_PERIODS) {
	b->pos_period = 0;
	b->pos_day = b->pos_day + 1;
    }
    if (b->pos_day >= NUM_DAYS) {
	b->pos_day = 0;
	b->pos_class = b->pos_class + 1;
    }
}

void bigthing_reverse_position(bigthing b)
{
    b->pos_period--;
    if (b->pos_period < 0) {
	b->pos_period = NUM_PERIODS - 1;
	b->pos_day = b->pos_day - 1;
    }
    if (b->pos_day < 0) {
	b->pos_day = NUM_DAYS - 1;
	b->pos_class = b->pos_class - 1;
    }
}

int bigthing_positions_completed(bigthing b)
{
    if (b->pos_class >= NUM_CLASSES) {
	return(1);
    }
    return(0);
}

int bigthing_positions_almost_completed(bigthing b)
{
    if ( (b->pos_class + 1 == NUM_CLASSES) &&
	 (b->pos_day + 1 == NUM_DAYS) &&
	 (b->pos_period + 1 == NUM_PERIODS))
    {
	return(1);
    }
    return(0);
}

int bigthing_prepare_stcs(bigthing b)
{
    int i,j,t;
    int id_counter;
    int num_meetings;
    meetings meet;
    module mod;
    stc s;

    id_counter = 0;
    
    /* prepare the stc list */
    b->stcs = (muvdalist *) check_malloc (sizeof(muvdalist_struct) * NUM_CLASSES);
    for (i=0; i < NUM_CLASSES; i++) {
	b->stcs[i] = muvdalist_constructor(stc_free_shallow, NULL, NULL, (void*) stc_shallow_copy, NULL);
    }

    /* foreach meeting */
    if (ulvlist_init_pos(b->pd->meetingses)) {
	do {
	    meet = (meetings) ulvlist_data_at_pos(b->pd->meetingses);
	    num_meetings = silist_get_size(meetings_get_periods_arrangement(meet));
	    
	    /* create the required number of stcs */
	    for (j=0; j < num_meetings; j++) {
		s = stc_constructor();
		stc_set_id(s, id_counter);
		stc_set_subject(s, meetings_get_subject(meet));
		stc_set_teachers(s, meetings_get_teachers(meet));
		stc_set_class(s, meetings_get_class(meet));
		stc_set_room(s, meetings_get_room(meet));
		stc_set_num_periods(s, silist_get_element_n((meetings_get_periods_arrangement(meet)), j));
		stc_set_period_num(s, 0);

		/* run all the modules associated to the meetings struct */
		for (t=0; t < meetings_get_num_mods(meet); t++) {
		    mod = meetings_get_mod_n(meet,t);
		    mod->mod_reg_id(MOD_FOR_MEETINGS, id_counter, meetings_get_mod_value_n(meet, t));
		}
		
		/* put the stc in the correct place */
		muvdalist_insert(b->stcs[stc_get_class(s)], s);

		/* increment the stc id counter */
		id_counter++;
	    }
	} while (ulvlist_move_pos(b->pd->meetingses));
    }
    return (id_counter);
}

/* Check the number of periods being taught to each class in a week.
** If it is less than NUM_PERIODS * NUM_DAYS, then add enough free
** periods to fill the table */
void bigthing_create_necessary_free_periods(bigthing b, int next_stc_id)
{
    int class_id;
    int num_periods;
    int free_subject_id;
    int free_teacher_id;
    subject s;
    teacher t;
    stc temp_stc;

    /* Add a:
       subject titled "Free"
       teacher titled "Free"

       Is this the best place to do this?
    */

    s = subject_constructor();
    subject_set_name(s, "Free");
    free_subject_id = sandndlist_insert(b->pd->subjects, s);
    NUM_SUBJECTS++;

    t = teacher_constructor();
    teacher_set_name(t, "Free");
    free_teacher_id = sandndlist_insert(b->pd->teachers, t);
    NUM_TEACHERS++;

    /* End misplaced addition of a free subject and teacher */

    for(class_id = 0; class_id < NUM_CLASSES; class_id++) {
	num_periods = 0;
	if (muvdalist_init_pos(b->stcs[class_id])) {
	    temp_stc = muvdalist_data_at_pos(b->stcs[class_id]);
	    do {
		num_periods = num_periods + stc_get_num_periods(temp_stc);
		if (!muvdalist_move_pos(b->stcs[class_id])) {
		    break;
		}
		temp_stc = muvdalist_data_at_pos(b->stcs[class_id]);
	    } while ((temp_stc = muvdalist_data_at_pos(b->stcs[class_id])) != NULL);
	}

	if (num_periods > (NUM_DAYS * NUM_PERIODS)) {
	    fprintf(stderr, "Sanity check error: Class %s has %d periods per week.\n[Max of your configuration is days-in-cycle * periods-in-day = %d]\n", class_get_name(sandndlist_find_element_by_id(b->pd->classes, class_id)), num_periods, NUM_DAYS * NUM_PERIODS);
	    exit(-1);
	}

#ifdef CLASS_STATS
	    printf("Class %s has %d periods per week, and %d free periods per week.\n", class_get_name(sandndlist_find_element_by_id(b->pd->classes, class_id)), num_periods, (NUM_DAYS * NUM_PERIODS) - num_periods);
#endif

	for ( ; num_periods < (NUM_DAYS * NUM_PERIODS); num_periods++) {
#ifdef DEBUG
	    printf("Adding free period for class %s\n", class_get_name(sandndlist_find_element_by_id(b->pd->classes, class_id)));
#endif
	    temp_stc = stc_constructor();
	    stc_set_id(temp_stc, next_stc_id);
	    stc_set_subject(temp_stc, free_subject_id);
	    stc_add_teacher(temp_stc, free_teacher_id);
	    stc_set_class(temp_stc, class_id);
	    stc_set_num_periods(temp_stc, 1);
	    stc_set_period_num(temp_stc, 0);
	    stc_set_free_period(temp_stc, 1);
	    muvdalist_insert(b->stcs[class_id], temp_stc);
	    next_stc_id++;
	}
    }
}

void bigthing_sanity_check(bigthing b)
{
    /* We've already checked the class's periods per week
    ** in the free periods function */
    bigthing_check_teachers_periods(b);
}

void bigthing_check_teachers_periods(bigthing b)
{
    int teacher_id;
    int num_periods;
    int class_id;
    stc temp_stc;

    for (teacher_id = 0; teacher_id < NUM_TEACHERS; teacher_id++) {
	if (strcmp(sandndlist_find_element_by_id(b->pd->teachers, teacher_id), "Free") != 0 ) {
	    num_periods = 0;
	    
	    for(class_id = 0; class_id < NUM_CLASSES; class_id++) {
		if (muvdalist_init_pos(b->stcs[class_id])) {
		    temp_stc = muvdalist_data_at_pos(b->stcs[class_id]);
		    do {
			if (stc_is_teaching(temp_stc, teacher_id)) {
			    num_periods = num_periods + stc_get_num_periods(temp_stc);
			}
			if(!muvdalist_move_pos(b->stcs[class_id])) {
			    break;
			}
		    } while ((temp_stc = muvdalist_data_at_pos(b->stcs[class_id])) != NULL);
		}		
	    }
	    
	    if ( (num_periods > (NUM_DAYS * NUM_PERIODS)) && (teacher_get_name(sandndlist_find_element_by_id(b->pd->teachers, teacher_id)) != "Free")) {
		fprintf(stderr, "Sanity check error: Teacher %s has %d periods per week\n[Max of your configuration is days-in-cycle * periods-in-day = %d]\n", teacher_get_name(sandndlist_find_element_by_id(b->pd->teachers, teacher_id)), num_periods, NUM_DAYS * NUM_PERIODS);
		exit(-1);
	    }

#ifdef TEACHER_STATS
	    printf("Teacher %s teaches %d periods per week, and has %d free periods per week.\n", teacher_get_name(sandndlist_find_element_by_id(b->pd->teachers, teacher_id)), num_periods, (NUM_DAYS * NUM_PERIODS) - num_periods);
#endif
	}
    }
}
