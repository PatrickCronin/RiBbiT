/* mtt.c
** Master Time Table functions file.
** Defines the functions of an mtt data type.
** Patrick Cronin
** Copyright (c) 5 March 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "../../program/memory.h"
#include "stc.h"
#include "mtt.h"
#include "ttt.h"
#include "../list_structures/sandndlist.h"
#include "../misc_structures/data.h"
#include "../misc_structures/bigthing.h"
#include "class.h"
#include "subject.h"
#include "teacher.h"
#include "room.h"


mastertimetable mtt_constructor(void)
{
    mastertimetable newmtt = (mastertimetable) check_malloc (sizeof(mastertimetable_struct));
    mtt_init(newmtt);
    return(newmtt);
}

void mtt_init(mastertimetable m)
{
    m->timetable = (stc *) check_calloc (NUM_TUPLES, sizeof(stc));
}

stc mtt_get_entry_at(mastertimetable m, int class_id, int day_num, int period_num)
{
    int loc = mtt_make_loc_int(m, class_id, day_num, period_num);
    mtt_check_entry_location(m, loc);
    return (m->timetable[loc]);
}

stc mtt_pop_entry_at(mastertimetable m, int class_id, int day_num, int period_num)
{
    stc removed;
    removed = mtt_get_entry_at(m, class_id, day_num, period_num);
    mtt_set_entry_at(m, class_id, day_num, period_num, NULL);
    return(removed);
}

void mtt_set_entry_at(mastertimetable m, int class_id, int day_num, int period_num, stc mystc)
{
    int loc = mtt_make_loc_int(m, class_id, day_num, period_num);
    mtt_check_entry_location(m, loc);
    m->timetable[loc] = mystc;
}

int mtt_make_loc_int(mastertimetable m, int class_id, int day_num, int period_num)
{
    return ((class_id * PERIODS_PER_WEEK) + (day_num * NUM_PERIODS) + period_num);
}

void mtt_check_entry_location(mastertimetable m, int loc)
{
    if (loc > NUM_TUPLES) {
	fprintf(stderr,"mtt: Location for requested tuple is too large for this time table (fatal).\n(Max size: %d, requested location: %d)\n", NUM_TUPLES - 1, loc);
	exit(-1);
    }
    return;
}

void mtt_display(mastertimetable m)
{
    printf("No display function yet for mtt datatype.\n");
}

void mtt_save_solution(mastertimetable tt, program_data pd, mpz_t num_sols)
{
    int str_size;
    char * num_sols_from_mpz;
    char * outputfilename = NULL;
    FILE * output;

    str_size = mpz_sizeinbase(num_sols, 10);
    num_sols_from_mpz = (char *) check_malloc (sizeof(char) * (str_size + 1));
    num_sols_from_mpz = mpz_get_str(num_sols_from_mpz, 10, num_sols);
    num_sols_from_mpz[str_size] = '\0';

    str_size = str_size + strlen(pd->outputprefix);
    outputfilename = (char *) check_malloc (sizeof(char) * (str_size + 6));

    sprintf(outputfilename, "%s%s.html", pd->outputprefix, num_sols_from_mpz);
    outputfilename[str_size + 5] = '\0';

    printf("opening %s for output\n", outputfilename);
    
    output = fopen(outputfilename, "w");
    mtt_write_solution(tt, pd, output);
    fclose (output);

    free(outputfilename);
    free(num_sols_from_mpz);
}

void mtt_save_partial_solution(mastertimetable tt, program_data pd, mpz_t num_sols, int depth, muvdalist list)
{
    int str_size;
    char * num_sols_from_mpz;
    char * outputfilename = NULL;
    FILE * output;
    stc unplaced;
    int pos_save;
    int t = 0;

    str_size = mpz_sizeinbase(num_sols, 10);
    num_sols_from_mpz = (char *) check_malloc (sizeof(char) * (str_size + 1));
    num_sols_from_mpz = mpz_get_str(num_sols_from_mpz, 10, num_sols);
    num_sols_from_mpz[str_size] = '\0';

    /* NOTE: THE STRING OUTPUTFILENAME MEMORY ALLOCATION IS GHETTO! */
    str_size = str_size + strlen(pd->outputprefix);
    outputfilename = (char *) check_calloc (str_size + 30, sizeof(char));

    sprintf(outputfilename, "%s%s_partial_%d.html", pd->outputprefix, num_sols_from_mpz, depth);

    printf("opening %s for output\n", outputfilename);
    
    output = fopen(outputfilename, "w");
    
    pos_save = muvdalist_get_pos(list);
    if (muvdalist_init_pos_gn(list)) {
	/* Draw unplaced STCs */
	fprintf(output, "<table border=1><caption>Unplaced</caption><tr>\n");
	while ((unplaced = muvdalist_get_element_n(list, muvdalist_get_next_unmarked_element_position(list))) != NULL) {
	    fprintf(output, "<td>");
	    fprintf(output, "<b>%s</b><br>\n", sandndlist_find_name_by_id(pd->subjects, stc_get_subject(unplaced)));
	    t=0;
	    while (t < stc_get_num_teachers(unplaced)) {
		fprintf(output, "%s<br>\n", sandndlist_find_name_by_id(pd->teachers, stc_get_teacher_n(unplaced, t)));
		t++;
	    }
	    fprintf(output, "</td>");
	}
	fprintf(output, "</td></tr></table>\n");
    }
    muvdalist_set_pos(list, pos_save);

    mtt_write_solution(tt, pd, output);
    fclose (output);

    free(outputfilename);
    free(num_sols_from_mpz);
}

void mtt_write_solution(mastertimetable tt, program_data pd, FILE * output)
{
    int teacher_id;
    teachertimetable temp_tt;

    fprintf(output, "<html>\n\n<head>\n<title>output</title>\n</head>\n\n");
    fprintf(output, "<body>\n\n");
    
    mtt_write_class_table(tt, pd, output);
    fprintf(output, "<hr>\n");
    
    for(teacher_id = 0; teacher_id < NUM_TEACHERS; teacher_id++) {
	if (strcmp(sandndlist_find_name_by_id(pd->teachers, teacher_id), "Free") != 0) {
	    temp_tt = ttt_make_teacher_table(tt, pd, teacher_id);
	    ttt_write_teacher_table(temp_tt, pd, teacher_id, output);
	    ttt_free_shallow(temp_tt);
	}
    }

    fprintf(output, "</body>\n</html>\n");
}

void mtt_write_class_table(mastertimetable tt, program_data pd, FILE * output)
{
    int c, p, d, t;
    stc the_stc;
    
    for (d=0; d<NUM_DAYS; d++) {
	fprintf(output, "<h2>%s</h2>\n", sandndlist_find_name_by_id(pd->days, d));
	fprintf(output, "<table border=2 cellpadding=5>\n");
	for (p=-1; p<NUM_PERIODS; p++) { /* rows */
	    
	    if (p == -1) {
		/* for the first row: class names */
		fprintf(output, "\t<tr bgcolor=#bbbbbb>\n");
	    } else {
		/* for the other rows */
		fprintf(output, "\t<tr>\n");
	    }

	    for (c=-1; c<NUM_CLASSES; c++) { /* columns */
		if ((p == -1) && (c == -1)) {
		    /* for the first row and first column */
		    fprintf(output, "\t\t<td></td>\n");
		} else if (p == -1) {
		    /* for the rest of the first row */
		    fprintf(output, "\t\t<th>%s</th>\n", sandndlist_find_name_by_id(pd->classes, c));
		} else if (c == -1) {
		    /* for all of the first column cells */
		    fprintf(output, "\t\t<th bgcolor=#ddbbbb>Period %d</th>\n", p + 1);
		} else {
		    /* an STC cell */
		    the_stc = mtt_get_entry_at(tt, c, d, p);
		    if (the_stc != NULL) {
			if (stc_is_free_period(the_stc)) {
			    fprintf(output, "\t\t<td align=\"center\"><b>Free</b>\n");
			} else {
			    fprintf(output, "\t\t<td><b>%s</b><br>\n", sandndlist_find_name_by_id(pd->subjects, stc_get_subject(the_stc)));
			    for(t=0; t<stc_get_num_teachers(the_stc); t++) {
				fprintf(output, "\t\t%s<br>\n", sandndlist_find_name_by_id(pd->teachers, stc_get_teacher_n(the_stc, t)));
			    }
			    if (stc_get_room(the_stc) != -1) {
				fprintf(output, "\t\t<i>%s</i><br>\n", sandndlist_find_name_by_id(pd->rooms, stc_get_room(the_stc)));
			    }
			    /* fprintf(output, "\t\tSTC_ID: %d\n", stc_get_id(the_stc)); */
			}
			fprintf(output, "\t\t</td>\n");
		    }
		}
	    }
	    fprintf(output, "\t</tr>\n");
	}
	fprintf(output, "</table>\n<br>\n<br>\n<br>\n");
    }
}

void mtt_free_shallow(mastertimetable m)
{
    free(m->timetable);
}
