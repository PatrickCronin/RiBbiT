/* algorithm.c
** The main algorithm to generate all possibilities of the time table.
** Copyright (c) 11 March 2004 Patrick Cronin
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

void algorithm_generate_timetable(bigthing b, ulvlist current_possibilities_list)
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
		algorithm_generate_timetable(b, next_possibilities_list);
	    }
	    
	    /* If there are fake stcs for multiple periods, free them */
	    for (stc_num_periods = stc_get_num_periods(cur) - 1; stc_num_periods > 0; stc_num_periods--) {
		bigthing_reverse_position(b);
		stc_free_deep(mtt_pop_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period));
	    }
	    bigthing_reverse_position(b);
	    mtt_pop_entry_at(b->tt, b->pos_class, b->pos_day, b->pos_period);
	    ulvlist_insert_beginning(current_possibilities_list, cur);
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
