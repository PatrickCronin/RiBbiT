/* algorithm2.c
** The main algorithm to generate all possibilities of the time table.
** First, we organize the STCs into the days that each shall be on for
** a class. (This grouping is NOT guaranteed to be correct due to
** modules like blocked_cells and probably others in the future.) Then,
** we'll do a recursive backtracking search to see if we can generate
** a functional time table using that permutation of STCs into days.
** Copyright (c) 26 December 2004 Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <ctype.h>
#include "memory.h"
#include "algorithm2.h"
#include "module.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/list_structures/ulvpclist.h"
#include "../data_types/list_structures/ulvlist.h"
#include "../data_types/list_structures/sandndlist.h"
#include "../data_types/school_structures/stc.h"
#include "../data_types/school_structures/mtt.h"

void algorithm2_generate_timetable_3(bigthing b)
{
/*
  pseudo-code:
  
  Variable of the class's day's stc list
  
  1. Generate a probably-functional permutation of class STCs.
  (This algorithm could be improved if we made could guarantee that each
  permutation was functional.) 
  
  1. (Re-worded) Generate an STC list for each class/day pair. (Assign
  each stc a day, and we'll do the rest of the work in the next step.)

  2. Do a recursive backtracking search on that permutation of STCs.

  3. Repeat 1 and 2 (recursively) until all possible combinations are
  generated.

  This algorithm is an improvement over an unorganized direct recursive
  backtracking search because it will dramatically reduce the number of
  possibilities we try overall.

  Future:
  F1a. Will it be possible to monitor progress?
  F1b. Could we guarantee a soluntion time?
  F2. Could we guarantee a solution?
*/

  ulvpclist ** class_stcs_by_day;
  int class, day;
  mpz_t num_arrangements;

  /* allocate and initialize each class/day list */
  class_stcs_by_day = (ulvpclist **) check_malloc (sizeof(ulvlist*) * NUM_CLASSES);
  for (class = 0; class < NUM_CLASSES; class++) {
    class_stcs_by_day[class] = (ulvpclist *) check_malloc (sizeof(ulvlist) * NUM_DAYS);
    for (day = 0; day < NUM_DAYS; day++) {
        class_stcs_by_day[class][day] = ulvpclist_constructor(stc_free_shallow, NULL, stc_compare_types_stcs, (void *) stc_shallow_copy, NULL);
    }
  }

  mpz_init_set_str(num_arrangements, "0", 10);

  algorithm2_arrange_stc_days_3(b, 0, 0, class_stcs_by_day, num_arrangements);

  /* destroy and free each class/day list */
  for (class = 0; class < NUM_CLASSES; class++) {
    for (day = 0; day < NUM_DAYS; day++) {
      free(class_stcs_by_day[class][day]);
    }
    free(class_stcs_by_day[class]);
  }
  free(class_stcs_by_day);

}

void algorithm2_arrange_stc_days_1(bigthing b, int class, int day, ulvpclist ** stc_lists, mpz_t num_arrangements)
{
  int pos;
  stc try;
  ulvlist types_tried;

  muvdalist_init_pos_gn(b->stcs[class]);
  pos = muvdalist_get_next_unmarked_element_position(b->stcs[class]);
  
  types_tried = ulvlist_constructor(stc_free_shallow, NULL, stc_compare_types_stcs, (void *) stc_shallow_copy, NULL);

  while ((pos != -1) && (b->stop_arrangements_flag == 0)) {

      try = (stc) muvdalist_get_element_n(b->stcs[class], pos);
      if (
	  (algorithm2_check_stc_types_tried(types_tried, try)) &&
	  (algorithm2_check_stc_errors(b, stc_lists[class][day], try, day))
	  )
	{
	  /* Make sure that we don't try to put the same type of
	  ** stc here again! */
	  ulvlist_insert_beginning(types_tried, try);
	  
	  muvdalist_mark_element (b->stcs[class], pos);
	  
	  ulvpclist_insert_beginning(stc_lists[class][day], try);
	  ulvpclist_add_pc(stc_lists[class][day], stc_get_num_periods(try));

	  if (ulvpclist_pc(stc_lists[class][day]) == NUM_PERIODS) {
	    day++;
	    if (day == NUM_DAYS) {
	      day = 0;
	      class++;
	    }
	  }

	  if (class == NUM_CLASSES) {
	    // printf("We're good!\n");
	    /* We have a good arrangement of STCs */
	    mpz_add_ui(num_arrangements, num_arrangements, (unsigned long int) 1);
	    algorithm2_output_stc_arrangement(b, stc_lists, num_arrangements);

	    if ((b->pd->max_num_arrangements != -1) &&
		(mpz_cmp_ui(num_arrangements, b->pd->max_num_arrangements) != -1))
	      {
		b->stop_arrangements_flag = 1;
	      }

	    /* now, go back */
	    class--;
	    day = NUM_DAYS - 1;
	  } else {
	    /* keep on truckin' */
	    algorithm2_arrange_stc_days_1(b, class, day, stc_lists, num_arrangements);
	    if (ulvpclist_pc(stc_lists[class][day]) == 0) {
	      day--;
	      if (day == -1) {
		day = NUM_DAYS - 1;
		class--;
	      }
	    }
	  }
	  
	  ulvpclist_pop_first(stc_lists[class][day]);
	  ulvpclist_sub_pc(stc_lists[class][day], stc_get_num_periods(try));

	  muvdalist_unmark_element(b->stcs[class], pos);
	  muvdalist_set_pos(b->stcs[class], pos);
	}
      pos = muvdalist_get_next_unmarked_element_position(b->stcs[class]);
  }
  
  ulvlist_free_shallow(types_tried);
  return;
}


/* This function is quick for generating at least one STC day arrangment.  It is probably very slow
** for generating them all. */
void algorithm2_arrange_stc_days_2(bigthing b, int class, int day, ulvpclist ** stc_lists, mpz_t num_arrangements)
{
  int pos;
  stc try;
  ulvlist types_tried;

  /* Pseudo-code:
     For class 0, day 0, put in the first fitting STC
     Move to day 1.  Put in first fitting STC
     Move to day 2.  Put in first fitting STC
     etc...
     
     Move to class 1, day 0. Put in the first fitting STC
     etc...
  */

  muvdalist_init_pos_gn(b->stcs[class]);
  pos = muvdalist_get_next_unmarked_element_position(b->stcs[class]);
  
  types_tried = ulvlist_constructor(stc_free_shallow, NULL, stc_compare_types_stcs, (void *) stc_shallow_copy, NULL);

  while ((pos != -1) && (b->stop_arrangements_flag == 0)) {

      try = (stc) muvdalist_get_element_n(b->stcs[class], pos);
      if (
	  (algorithm2_check_stc_types_tried(types_tried, try)) &&
	  (algorithm2_check_stc_errors(b, stc_lists[class][day], try, day))
	  )
	{
	  /* Make sure that we don't try to put the same type of
	  ** stc here again! */
	  ulvlist_insert_beginning(types_tried, try);
	  
	  muvdalist_mark_element (b->stcs[class], pos);
	  
	  ulvpclist_insert_beginning(stc_lists[class][day], try);
	  ulvpclist_add_pc(stc_lists[class][day], stc_get_num_periods(try));

	  day++;
	  if (day == NUM_DAYS) {
	    day = 0;
	  }

	  /* Did we finish the class? */
	  if (muvdalist_get_num_unmarked(b->stcs[class]) == 0) {
	    class++;
	  }
	  
	  if (class == NUM_CLASSES) {
	    // printf("We're good!\n");
	    /* We have a full arrangement of STCs */
	  
	    /* Check if the teacher assignments on each day are viable */
	    /* i.e. each teacher teaches up to 8 periods a day */
	    if (algorithm2_arrangements_check_teacher_assignments(b, stc_lists)) {
	      mpz_add_ui(num_arrangements, num_arrangements, (unsigned long int) 1);
	      algorithm2_output_stc_arrangement(b, stc_lists, num_arrangements);

	      if ((b->pd->max_num_arrangements != -1) &&
		  (mpz_cmp_ui(num_arrangements, b->pd->max_num_arrangements) != -1))
		{
		  b->stop_arrangements_flag = 1;
		}
	    } else {
	      // printf("Generated invalid STC day arrangement!\n");
	    }	    
	  } else {
	    /* keep on truckin' */
	    algorithm2_arrange_stc_days_2(b, class, day, stc_lists, num_arrangements);
	  }

	  day--;
	  if (day == -1) {
	    day = NUM_DAYS - 1;
	  }
	  if ((class == NUM_CLASSES) ||
	      (muvdalist_get_num_unmarked(b->stcs[class]) == muvdalist_size(b->stcs[class]))) {
	    class--;
	  }
	  
	  ulvpclist_pop_first(stc_lists[class][day]);
	  ulvpclist_sub_pc(stc_lists[class][day], stc_get_num_periods(try));

	  muvdalist_unmark_element(b->stcs[class], pos);
	  muvdalist_set_pos(b->stcs[class], pos);
	}
      pos = muvdalist_get_next_unmarked_element_position(b->stcs[class]);
  }
  
  ulvlist_free_shallow(types_tried);
  return;
}

void algorithm2_arrange_stc_days_3(bigthing b, int class, int day, ulvpclist ** stc_lists, mpz_t num_arrangements)
{
  int ** teacher_period_count;
  int d;

  teacher_period_count = (int **) check_malloc (sizeof(int*) * NUM_DAYS);
  for (d=0; d < NUM_DAYS; d++) {
    teacher_period_count[d] = (int *) check_calloc (NUM_TEACHERS, sizeof(int));
  }
  
  algorithm2_arrange_stc_days_3_work(b, class, day, stc_lists, num_arrangements, teacher_period_count);

  for (d=0; d < NUM_DAYS; d++) {
    free(teacher_period_count[d]);
  }
  free(teacher_period_count);
  return;
}

void algorithm2_arrange_stc_days_3_work(bigthing b, int class, int day, ulvpclist ** stc_lists, mpz_t num_arrangements, int ** teacher_period_count)
{
  int pos, t;
  stc try;
  ulvlist types_tried;

  /* Pseudo-code:
     For class 0, day 0, put in the first fitting STC
     Move to day 1.  Put in first fitting STC
     Move to day 2.  Put in first fitting STC
     etc...
     
     Move to class 1, day 0. Put in the first fitting STC
     etc...
  */

  muvdalist_init_pos_gn(b->stcs[class]);
  pos = muvdalist_get_next_unmarked_element_position(b->stcs[class]);
  
  types_tried = ulvlist_constructor(stc_free_shallow, NULL, stc_compare_types_stcs, (void *) stc_shallow_copy, NULL);

  while ((pos != -1) && (b->stop_arrangements_flag == 0)) {

      try = (stc) muvdalist_get_element_n(b->stcs[class], pos);
      if (
	  (algorithm2_check_stc_types_tried(types_tried, try)) &&
	  (algorithm2_check_previous_teacher_arrangements(b, teacher_period_count[day], try)) &&
	  (algorithm2_check_stc_errors(b, stc_lists[class][day], try, day))
	  )
	{
	  /* Make sure that we don't try to put the same type of
	  ** stc here again! */
	  ulvlist_insert_beginning(types_tried, try);

	  /* Add the teachers to the period counting chart */
	  for (t=0; t < stc_get_num_teachers(try); t++) {
	    teacher_period_count[day][stc_get_teacher_n(try,t)]++;
	  }
	  
	  muvdalist_mark_element (b->stcs[class], pos);
	  ulvpclist_insert_beginning(stc_lists[class][day], try);
	  ulvpclist_add_pc(stc_lists[class][day], stc_get_num_periods(try));

	  day++;
	  if (day == NUM_DAYS) {
	    day = 0;
	  }

	  /* Did we finish the class? */
	  if (muvdalist_get_num_unmarked(b->stcs[class]) == 0) {
	    printf("Finished %s\n", sandndlist_find_name_by_id(b->pd->classes, class));
	    class++;
	  }
	  
	  if (class == NUM_CLASSES) {
	    // printf("We're good!\n");
	    /* We have a full arrangement of STCs */
	    mpz_add_ui(num_arrangements, num_arrangements, (unsigned long int) 1);
	    algorithm2_output_stc_arrangement(b, stc_lists, num_arrangements);
	    
	    if ((b->pd->max_num_arrangements != -1) &&
		(mpz_cmp_ui(num_arrangements, b->pd->max_num_arrangements) != -1))
	      {
		b->stop_arrangements_flag = 1;
	      }
	  } else {
	    /* keep on truckin' */
	    algorithm2_arrange_stc_days_3_work(b, class, day, stc_lists, num_arrangements, teacher_period_count);
	  }

	  day--;
	  if (day == -1) {
	    day = NUM_DAYS - 1;
	  }
	  if ((class == NUM_CLASSES) ||
	      (muvdalist_get_num_unmarked(b->stcs[class]) == muvdalist_size(b->stcs[class]))) {
	    class--;
	    printf("Unfinished %s\n", sandndlist_find_name_by_id(b->pd->classes, class));
	  }

	  /* Remove the teachers from the period counting chart */
	  for (t=0; t < stc_get_num_teachers(try); t++) {
	    teacher_period_count[day][stc_get_teacher_n(try,t)]--;
	  }
	  
	  ulvpclist_pop_first(stc_lists[class][day]);
	  ulvpclist_sub_pc(stc_lists[class][day], stc_get_num_periods(try));

	  muvdalist_unmark_element(b->stcs[class], pos);
	  muvdalist_set_pos(b->stcs[class], pos);
	}
      pos = muvdalist_get_next_unmarked_element_position(b->stcs[class]);
  }
  
  ulvlist_free_shallow(types_tried);
  return;
}

int algorithm2_check_previous_teacher_arrangements(bigthing b, int * teacher_period_count, stc try)
{
  int t;
  for (t=0; t<stc_get_num_teachers(try); t++) {
    if (teacher_period_count[stc_get_teacher_n(try, t)] + stc_get_num_periods(try) > NUM_PERIODS) {
      //      printf("Momentum slowed by %s teaching %s to %s\n", sandndlist_find_name_by_id(b->pd->teachers, stc_get_teacher_n(try, t)), sandndlist_find_name_by_id(b->pd->subjects, stc_get_subject(try)), sandndlist_find_name_by_id(b->pd->classes, stc_get_class(try)));
      return(0);
    }
  }
  return(1);
}

int algorithm2_check_stc_types_tried(ulvlist types_tried, stc cur)
{
    if (ulvlist_find(types_tried, cur) != NULL) {
	/* fail if found */
	return(0);
    }
    /* succeed if not found */
    return(1);
}

int algorithm2_check_stc_errors (bigthing b, ulvpclist stc_list, stc cur, int day)
{
    int i = 0;
    int errors = 0;
    int stc_num_periods;
    module mod;

    stc_num_periods = stc_get_num_periods(cur);

    /* if we have an stc for 4 periods, but there are only 3 periods
    ** left in the day, it obviously won't fit -- just save time */
    if (stc_num_periods > (NUM_PERIODS - ulvpclist_pc(stc_list))) {
	return(1);
    }

    while ((errors == 0) && (i < sandndlist_size(b->pd->modules))) {
      mod = sandndlist_find_element_by_id(b->pd->modules, i);
      if (mod->mod_check_stc_day != NULL) {
	errors = mod->mod_check_stc_day(stc_list, cur, day);
      }
      i++;
    }

    if (errors) return(0);
    else return(1);
}

void algorithm2_output_stc_arrangement(bigthing b, ulvpclist ** stc_lists, mpz_t num_arrangements)
{
    int str_len;
    char * arr_num;
    char * outputfilename = NULL;
    FILE * output;
    int class,day,t;
    stc cur_stc;

    /* generate a filename and create a new file for output */
    str_len = mpz_sizeinbase(num_arrangements, 10);
    arr_num = (char *) check_malloc (sizeof(char) * (str_len + 1));
    arr_num = mpz_get_str(arr_num, 10, num_arrangements);
    arr_num[str_len] = '\0';

    /* NOTE: THE STRING OUTPUTFILENAME MEMORY ALLOCATION IS GHETTO! */
    str_len = str_len + strlen(b->pd->outputprefix);
    outputfilename = (char *) check_calloc (str_len + 30, sizeof(char));
    sprintf(outputfilename, "%s_stc_arrangement_%s.html", b->pd->outputprefix, arr_num);
    printf("opening %s for output\n", outputfilename);
    output = fopen(outputfilename, "w");

    fprintf(output, "<html>\n\n<head>\n<title>STC arrangement number %s</title>\n</head>\n\n", arr_num);
    fprintf(output, "<body>\n\n");

    for (class=0; class < NUM_CLASSES; class++) {
      fprintf(output, "<table border=1>\n");
      fprintf(output, "<caption>%s</caption>\n", sandndlist_find_name_by_id(b->pd->classes, class));
      fprintf(output, "<tr>\n");
      for (day=0; day < NUM_DAYS; day++) {
	fprintf(output, "\t<th>%s</th>\n", sandndlist_find_name_by_id(b->pd->days, day));
	ulvpclist_init_pos(stc_lists[class][day]);
	while ((cur_stc = ulvpclist_data_at_pos(stc_lists[class][day])) != NULL) {
	  if (stc_is_free_period(cur_stc)) {
	    fprintf(output, "\t<td align=\"center\"><b>Free</b>\n");
	  } else {
	    fprintf(output, "\t<td><b>%s</b><br>\n", sandndlist_find_name_by_id(b->pd->subjects, stc_get_subject(cur_stc)));
	    for(t=0; t<stc_get_num_teachers(cur_stc); t++) {
	      fprintf(output, "\t%s<br>\n", sandndlist_find_name_by_id(b->pd->teachers, stc_get_teacher_n(cur_stc, t)));
	    }
	    if (stc_get_room(cur_stc) != -1) {
	      fprintf(output, "\t<i>%s</i><br>\n", sandndlist_find_name_by_id(b->pd->rooms, stc_get_room(cur_stc)));
	    }
	    fprintf(output, "\tID: <b>%d</b><br>\n", stc_get_id(cur_stc));
	  }
	  fprintf(output, "\t</td>\n");
	  ulvpclist_move_pos(stc_lists[class][day]);
	}
	fprintf(output, "</tr>\n");
      }
      fprintf(output, "</table>\n\n");
      
    }
    fprintf(output, "</body></html>\n");
    fclose(output);
    free(arr_num);
    free(outputfilename);
}

int algorithm2_arrangements_check_teacher_assignments(bigthing b, ulvpclist ** stc_lists)
{
  int * teacher_period_count;
  int day, class, teacher;
  stc the_stc;

  teacher_period_count = (int *) check_malloc (sizeof(int) * NUM_TEACHERS);

  for (day = 0; day < NUM_DAYS; day++) {
    memset(teacher_period_count, 0, sizeof(int) * NUM_TEACHERS);
    for (class = 0; class < NUM_CLASSES; class++) {
      ulvpclist_init_pos(stc_lists[class][day]);
      while ((the_stc = ulvpclist_data_at_pos(stc_lists[class][day])) != NULL) {
	for (teacher=0; teacher < stc_get_num_teachers(the_stc); teacher++) {
	  teacher_period_count[stc_get_teacher_n(the_stc, teacher)]++;
	  if (teacher_period_count[stc_get_teacher_n(the_stc, teacher)] > NUM_PERIODS) {
	    free(teacher_period_count);
	    return(0);
	  }
	}
	ulvpclist_move_pos(stc_lists[class][day]);
      }
    }
  }

  free(teacher_period_count);
  return(1);
}

void algorithm_generate_timetable_2(bigthing b)
{
  int stc_list_cur;
  int stc_num_periods;
  int stc_num_teachers;
  int n;
  stc cur, multi_cur;
  ulvlist types_tried;

    b->depth++;

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
