/* algorithm.c
** The main algorithm to generate all possibilities of the time table.
** Copyright (c) 17 April 2004 Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "algorithm.h"
#include "module.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/list_structures/ulvlist.h"
#include "../data_types/list_structures/sandndlist.h"
#include "../data_types/school_structures/stc.h"
#include "../data_types/school_structures/mtt.h"

// #define __DEBUG

void algorithm_generate_timetable_2(bigthing b)
{
    int stc_list_cur;
    int stc_num_periods;
    int stc_num_teachers;
    int n;
    stc cur, multi_cur;
    ulvlist types_tried;
#ifdef __DEBUG
    char c;
#endif

    b->depth++;

    if ((b->depth % 10) == 0) {
	printf("Recursive depth of %d out of %d\n", b->depth, NUM_TUPLES);
    }

    muvdalist_init_pos_gn(b->stcs[b->pos_class]);
    stc_list_cur = muvdalist_get_next_unmarked_element_position(b->stcs[b->pos_class]);

    types_tried = ulvlist_constructor(stc_free_shallow, NULL, stc_compare_types_stcs, (void *) stc_shallow_copy, NULL);

    while (stc_list_cur != -1) {
	cur = (stc) muvdalist_get_element_n(b->stcs[b->pos_class], stc_list_cur);
	if (
 	    (algorithm_check_stc_types_tried(types_tried, cur)) &&
	    (!algorithm_check_stc_errors(b, cur)) &&
	    (algorithm_check_subjects_days_left(b, cur)) &&
	    (algorithm_check_teachers_periods_teaching_in_days(b, cur))
	   )
	{
	    b->subjects_days_left[b->pos_class][stc_get_subject(cur)]--;

	    stc_num_teachers = stc_get_num_teachers(cur);
	    for (n=0; n<stc_num_teachers; n++) {
		b->teachers_periods_teaching_in_days[stc_get_teacher_n(cur,n)][b->pos_day]++;
	    }

	    /* Make sure that we don't try to put the same type of
	    ** stc here again! */
 	    ulvlist_insert_beginning(types_tried, cur);

	    muvdalist_mark_element (b->stcs[b->pos_class], stc_list_cur);

	    /* Put in the first stc as the real stc */
	    mtt_set_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period, cur);
	    bigthing_advance_position(b);

	    /* Put in other multi-period stc's */
	    for(stc_num_periods=1; stc_num_periods < stc_get_num_periods(cur); stc_num_periods++) {
		multi_cur = stc_deep_copy(cur);
		stc_set_period_num(multi_cur, stc_num_periods);
		mtt_set_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period, multi_cur);
		bigthing_advance_position(b);
	    }
	    
	    if (bigthing_positions_completed(b)) {
		mpz_add_ui(b->num_solutions, b->num_solutions, (unsigned long int) 1);
		mtt_save_solution(b->tt, b->pd, b->num_solutions);
	    } else {
#ifdef __DEBUG
		//if (bigthing_positions_almost_completed(b)) {
		if (b->depth == 225) {
		    printf("Depth of %d. Do you want to analyze? [n]: ", b->depth);
		    c = getchar();
		    if (c == 'y') {
			mtt_save_partial_solution(b->tt, b->pd, b->num_solutions, b->depth, b->stcs[b->pos_class]);
			printf("Saved partial solution %d\n", b->depth);
		    }
		    /* if we typed a character, then clear the carriage return */
		    if (c != 10) {
			c = getchar();
		    }
		}
#endif

		algorithm_generate_timetable_2(b);
	    }
	    
	    /* If there are fake stcs for multiple periods, free them */
	    for (stc_num_periods = stc_get_num_periods(cur) - 1; stc_num_periods > 0; stc_num_periods--) {
		bigthing_reverse_position(b);
		stc_free_deep(mtt_pop_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period));
	    }
	    bigthing_reverse_position(b);
	    mtt_pop_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period);
	    muvdalist_unmark_element(b->stcs[b->pos_class], stc_list_cur);
	    muvdalist_set_pos(b->stcs[b->pos_class], stc_list_cur);
	    b->subjects_days_left[b->pos_class][stc_get_subject(cur)]++;
	    for (n=0; n<stc_num_teachers; n++) {
		b->teachers_periods_teaching_in_days[stc_get_teacher_n(cur,n)][b->pos_day]--;
	    }
	}
	stc_list_cur = muvdalist_get_next_unmarked_element_position(b->stcs[b->pos_class]);
    }
    ulvlist_free_shallow(types_tried);

    b->depth--;
    return;   
}

void algorithm_generate_timetable_old_1(bigthing b, ulvlist current_possibilities_list)
{
    int stc_num_periods;
    stc cur, multi_cur;
    ulvlist next_possibilities_list;
    ulvlist types_tried;

    b->depth++;

    if ((b->depth % 30) == 0) {
	printf("Recursive depth of %d out of %d\n", b->depth, NUM_TUPLES);
    }

    ulvlist_init_pos(current_possibilities_list);
    cur = (stc) ulvlist_data_at_pos(current_possibilities_list);

    types_tried = ulvlist_constructor(stc_free_shallow, NULL, stc_compare_types_stcs, (void *) stc_shallow_copy, NULL);

    while (cur != NULL) {
	if (
 	    (algorithm_check_stc_types_tried(types_tried, cur)) &&
	    (!algorithm_check_stc_errors(b, cur))
	   )
	{
	    /* Make sure that we don't try to put the same type of
	    ** stc here again! */
 	    ulvlist_insert_beginning(types_tried, cur);

	    ulvlist_move_pos(current_possibilities_list);
    	    ulvlist_remove_element_by_ptr_shallow(current_possibilities_list, cur);

	    /* Put in the first stc as the real stc */
	    mtt_set_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period, cur);
	    bigthing_advance_position(b);

	    /* Put in other multi-period stc's */
	    for(stc_num_periods=1; stc_num_periods < stc_get_num_periods(cur); stc_num_periods++) {
		multi_cur = stc_deep_copy(cur);
		stc_set_period_num(multi_cur, stc_num_periods);
		mtt_set_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period, multi_cur);
		bigthing_advance_position(b);
	    }

	    if (bigthing_positions_completed(b)) {
		mpz_add_ui(b->num_solutions, b->num_solutions, (unsigned long int) 1);
		mtt_save_solution(b->tt, b->pd, b->num_solutions);
	    } else {
		if (ulvlist_size(current_possibilities_list) != 0) {
		    next_possibilities_list = ulvlist_copy_shallow(current_possibilities_list);
		} else {
		    /* not pos_class + 1 here because we've already 
		       advanced the bigthing position just above */
		    next_possibilities_list = ulvlist_copy_shallow(b->stcs[b->pos_class]);
		}
		algorithm_generate_timetable_old_1(b, next_possibilities_list);
	    }
	    
	    /* If there are fake stcs for multiple periods, free them */
	    for (stc_num_periods = stc_get_num_periods(cur) - 1; stc_num_periods > 0; stc_num_periods--) {
		bigthing_reverse_position(b);
		stc_free_deep(mtt_pop_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period));
	    }
	    bigthing_reverse_position(b);
	    mtt_pop_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period);
	    ulvlist_insert_beginning(current_possibilities_list, cur);

/* problem here?
** Do we need this else statement?
** shouldn't the move_pos and the data_at_pos be together outside of
** the if() ? */
	} else {


	    ulvlist_move_pos(current_possibilities_list);
	}
	cur = ulvlist_data_at_pos(current_possibilities_list);
    }
    ulvlist_free_shallow(current_possibilities_list);
    ulvlist_free_shallow(types_tried);

    b->depth--;
    return;
}

/* Stupid !
void algorithm_generate_timetable(bigthing b, ulvlist current_possibilities_list)
{
    while (CLASS LIST IS NOT EMPTY) {
	if ( STC IS OK ) {
	    DONT USE THIS TYPE AGAIN;

	    MARK THIS ELEMENT;

	    COPY THIS ELEMENT INTO THE TIMETABLE;

	    ADVANCE POSITION POINTERS;

	    IF FINISHED {
		WRITE SOLUTION;
	    } ELSE {
		CALL THIS FUNCTION AGAIN;
	    }

	    REVERSE POSITION POINTERS;

	    TAKE OUT THIS ELEMENT FROM THE TIMETABLE;
	} else {
	    GET ANOTHER ELEMENT TO TRY FROM THE CLASS LIST;
	}
	cur = ulvlist_data_at_pos(current_possibilities_list);
    }
    FREE CRAP;

    return;
}
*/
	    

int algorithm_check_stc_errors (bigthing b, stc cur)
{
    int i = 0;
    int errors = 0;
    int period_num = 0;
    int stc_num_periods;
    stc multi_cur;
    module mod;

    stc_num_periods = stc_get_num_periods(cur);

    /* if we have an stc for 4 periods, but there are only 3 periods
    ** left in the day, it obviously won't fit -- just save time */
    if (stc_num_periods > (NUM_PERIODS - b->pos_period)) {
	return(1);
    }

    /* if its only one period, don't do the rediculous stuff we'll
    ** do for a double or triple period */
    if (stc_num_periods == 1) {
	while ((errors == 0) && (i < sandndlist_size(b->pd->modules))) {
	    mod = sandndlist_find_element_by_id(b->pd->modules, i);
	    errors = mod->mod_check_stc(b, cur);
	    i++;
	}
    } else {
	while ((errors == 0) && (period_num < stc_num_periods)) {
	    i = 0;
	    while ((errors == 0) && (i < sandndlist_size(b->pd->modules))) {
		mod = sandndlist_find_element_by_id(b->pd->modules, i);
		errors = mod->mod_check_stc(b, cur);
		i++;
	    }
	    /* temporarily put the stc there, and then we can test another */
	    multi_cur = stc_deep_copy(cur);
	    stc_set_period_num(multi_cur, period_num);
	    mtt_set_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period, multi_cur);
	    bigthing_advance_position(b);
	    period_num++;
	}
	/* undo the testing changes that we made to the time table */
	while (period_num > 0) {
	    bigthing_reverse_position(b);
	    stc_free_deep(mtt_pop_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period));
	    period_num--;
	}
    }
    return(errors);
}

int algorithm_check_stc_types_tried(ulvlist types_tried, stc cur)
{
    if (ulvlist_find(types_tried, cur) != NULL) {
	/* fail if found */
	return(0);
    }
    /* succeed if not found */
    return(1);
}

int algorithm_check_subjects_days_left(bigthing b, stc cur)
/* returns 1 for success, 0 for problem */
{
    int s;
    int days_left_in_table;

    /* Last day? Don't need to check. */
    if ((b->pos_day) + 1 == NUM_DAYS) {
	return (1);
    }

    /* Will the current stc go up to the end of the day?
    ** (check for multiple periods!)
    ** If it does not, we don't need to check */
    if ((b->pos_period) + stc_get_num_periods(cur) != NUM_PERIODS) {
	return (1);
    }
    
    b->subjects_days_left[b->pos_class][stc_get_subject(cur)]--;
    s = 0;
    days_left_in_table = NUM_DAYS - 1 - b->pos_day;

    /* NUM_SUBJECTS - 1 because the last subject in the subject list
    ** is the FREE subject */
    while (s < NUM_SUBJECTS - 1) {
	if (b->subjects_days_left[b->pos_class][s] > days_left_in_table) {
	    b->subjects_days_left[b->pos_class][stc_get_subject(cur)]++;
	    return (0);
	}
    	s++;
    }
    b->subjects_days_left[b->pos_class][stc_get_subject(cur)]++;
    return(1);
}

int algorithm_check_teachers_periods_teaching_in_days(bigthing b, stc cur)
/* returns 1 for success, 0 for problem */
{
    int check_day;
    int stc_num_teachers;
    int teacher_num;
    int ok; /* flag */

    /* Last day? Don't need to check. */
    if ((b->pos_day) + 1 == NUM_DAYS) {
	return (1);
    }

    /* Will the current stc go up to the end of the day?
    ** (check for multiple periods!)
    ** If it does not, we don't need to check */
    if ((b->pos_period) + stc_get_num_periods(cur) != NUM_PERIODS) {
	return (1);
    }

    stc_num_teachers = stc_get_num_teachers(cur);
    for(teacher_num = 0; teacher_num < stc_num_teachers; teacher_num++) {
	b->teachers_periods_teaching_in_days[stc_get_teacher_n(cur,teacher_num)][b->pos_day]++;
    }
    
    for (teacher_num = 0; teacher_num < NUM_TEACHERS - 1; teacher_num++) {
	check_day = NUM_DAYS - 1;
	ok = 0;
	while (check_day > b->pos_day) {
	    if (b->teachers_periods_teaching_in_days[teacher_num][b->pos_day] != NUM_PERIODS) {
		/* it is ok! */
		ok++;
		break;
	    }
	    check_day--;
	}
	if (!ok) {
	    return(0);
	}
    }

    return (1);
}
