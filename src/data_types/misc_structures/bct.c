/* bct.c
** Blocked Cells Table functions file
** Describes the structure and functions of a bct data type.
** Patrick Cronin
** Copyright (c) 4 April 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bct.h"
#include "data.h"
#include "bigthing.h"
#include "../list_structures/sandndlist.h"
#include "../list_structures/silist.h"
#include "../../program/memory.h"

#define ENTRY_TOKEN    1
#define LIST_TOKEN     2
#define ASTERISK_TOKEN 3
#define DAY_TOKEN      4
#define UNKNOWN_TOKEN  5

#define DAYS_PART 1
#define PERIODS_PART 2

bct bct_constructor(int id, char * value)
{
    bct newbct = (bct) check_malloc (sizeof(blocked_cells_table_struct));
    bct_init1(newbct, id, value);
    return(newbct);
}

void bct_init1(bct b, int id, char * value)
{
    b->id = id;
    b->format_string = (char*) check_malloc (sizeof(char) * (strlen(value) + 1));
    strncpy(b->format_string, value, strlen(value) + 1);
    b->size = -1;
    b->timetable = NULL;
}

void bct_init2(bct b, int size)
{
    b->size = size;
    b->timetable = (char *) check_calloc (size, sizeof(char));
}

void bct_display(bct b)
{
    printf("There is no display function yet for the BCT data type.\n");
}

void bct_mark(bct b, int day, int period)
{
    b->timetable[bct_make_and_check_loc_int(day, period)] = 'b';
}

int bct_is_marked(bct b, int day, int period) {
    return (b->timetable[bct_make_and_check_loc_int(day, period)] == 'b');
}

int bct_make_and_check_loc_int(int day, int period)
{
    int loc = (day * NUM_PERIODS) + period;
    if (loc + 1 >= (NUM_DAYS * NUM_PERIODS)) {
	
    }
    return (loc);
}

void bct_prepare_table(bct b, bigthing bt)
{
    char ** format_groups = NULL;
    int i = 0;

    format_groups = tokenize_vector(b->format_string, ';');
    
    while (format_groups[i] != NULL) {
	bct_prepare_format_group(b, format_groups[i], bt);
	i++;
    }
    
    free(format_groups);
}

int bct_get_id(bct b)
{
    return (b->id);
}

/* The functions under this comment are all used for
** parsing the b->format_string and marking the b->timetable
** according to the format string.
*/

void bct_prepare_format_group (bct b, char * fg, bigthing bt)
{
    silist days, periods;
    int per, day;
    char ** parts;

    days = silist_constructor();
    periods = silist_constructor();

    parts = tokenize_vector(fg, ':');
    bct_parse_part(b, parts[0], days, DAYS_PART, bt);
    bct_parse_part(b, parts[1], periods, PERIODS_PART, bt);

    for (day=0; day < silist_get_size(days); day++) {
	for (per=0; per < silist_get_size(periods); per++) {
	    bct_mark(b,
		     silist_get_element_n(days, day),
		     silist_get_element_n(periods, per)
		);
	}
    }
    
    free(parts);
}

void bct_parse_part(bct b, char * part_string, silist the_list, int part_type, bigthing bt)
{
    char ** part_tokens;
    int token_type;
    int i = 0;

    part_tokens = tokenize_vector(part_string, ',');

    while (part_tokens[i] != NULL) {
	token_type = bct_get_token_type(part_tokens[i], part_type);
	switch (token_type) {
	    case ENTRY_TOKEN:
		bct_parse_entry_token(part_tokens[i], the_list);
		break;
	    case LIST_TOKEN:
		bct_parse_list_token(part_tokens[i], the_list, part_type, bt);
		break;
	    case ASTERISK_TOKEN:
		if (part_type == DAYS_PART) {
		    bct_parse_asterisk_token_for_days(the_list);
		} else {
		    bct_parse_asterisk_token_for_periods(the_list);
		}
		break;
	    case DAY_TOKEN:
		bct_parse_day_token(part_tokens[i], the_list, bt);
		break;
	    case UNKNOWN_TOKEN:
		fprintf(stderr, "blocked_cells: cannot parse token %s in your configuration file. (fatal)\n", part_tokens[i]);
		exit(-1);
		break;
	}
	i++;
    }
    free(part_tokens);
}

void bct_parse_entry_token(char * token, silist l)
{
    silist_insert(l, ((int) strtol(token, NULL, 10)) - 1);
}

void bct_parse_list_token(char * token, silist l, int part_type, bigthing bt)
{
    int start, finish, temp, tok_type, day_id;
    char ** parts = tokenize_vector(token, '-');

    if (part_type == DAYS_PART) {
	/* is the first part of the list a day or a number? */
	tok_type = bct_get_token_type(parts[0], part_type);
	switch(tok_type) {
	    case ENTRY_TOKEN:
		start = ((int) strtol(parts[0], NULL, 10)) - 1;
		break;
	    case DAY_TOKEN:
		day_id = sandndlist_find_id_by_name(bt->pd->days, parts[0]);
		if (day_id == -1) {
		    fprintf(stderr, "Config error in blocked-cells value: day name %s is not recognized [fatal].\n", token);
		    exit(-1);
		}
		start = day_id;
		break;
	    default:
		fprintf(stderr, "Uh-Oh -- unknown token %s????\n", parts[0]);
		exit(-1);
		break;
	}
	tok_type = bct_get_token_type(parts[1], part_type);
	switch(tok_type) {
	    case ENTRY_TOKEN:
		finish = ((int) strtol(parts[1], NULL, 10)) - 1;
		break;
	    case DAY_TOKEN:
		day_id = sandndlist_find_id_by_name(bt->pd->days, parts[1]);
		if (day_id == -1) {
		    fprintf(stderr, "Config error in blocked-cells value: day name %s is not recognized [fatal].\n", token);
		    exit(-1);
		}
		finish = day_id;
		break;
	    default:
		fprintf(stderr, "Uh-Oh -- unknown token????\n");
		exit(-1);
		break;
	}
    } else {
	start = ((int) strtol(parts[0], NULL, 10)) - 1;
	finish = ((int) strtol(parts[1], NULL, 10)) - 1;
    }
    
    /* make sure that start is less than finish
    ** -> simple swap if necessary */
    if (start > finish) {
	temp = start;
	start = finish;
	finish = temp;
    }

    for (temp = start; temp <= finish; temp++) {
	silist_insert(l, temp);
    }

    free(parts);
}

void bct_parse_asterisk_token_for_days(silist l)
{
    int i;
    for (i=0; i < NUM_DAYS; i++) {
	silist_insert(l, i);
    }
}

void bct_parse_asterisk_token_for_periods(silist l)
{
    int i;
    for (i=0; i < NUM_PERIODS; i++) {
	silist_insert(l, i);
    }
}

void bct_parse_day_token(char * token, silist l, bigthing bt)
{
    int day_id = sandndlist_find_id_by_name(bt->pd->days, token);
    if (day_id == -1) {
	fprintf(stderr, "Config error in blocked-cells value: day name %s is not recognized [fatal].\n", token);
	exit(-1);
    }
    silist_insert(l, day_id);
}

int bct_get_token_type (char * token, int part_type)
{
    int i;
    int num_dashes = 0;

    if ((token[0] == '*') && (token[1] == '\0')) {
	return(ASTERISK_TOKEN);
    }
    else {
	
	/* is it a number? */
	i = 0;
	while (isdigit(token[i])) {
	    i++;
	}
	if (token[i] == '\0') {
	    return (ENTRY_TOKEN);
	}

	if (part_type == DAYS_PART) {
	    /* is it the name of a day? */
	    i = 0;
	    while (isalnum(token[i])) {
		i++;
	    }
	    if (token[i] == '\0') {
		return (DAY_TOKEN);
	    }

	    /* is it a valid list?  e.g. 2-5 */
	    i = 0;
	    while (isalnum(token[i]) || (token[i] == '-')) {
		if (token[i] == '-') {
		    num_dashes++;
		    if (num_dashes > 1) return (UNKNOWN_TOKEN);
		}
		i++;
	    }
	    if (token[i] == '\0') {
		return (LIST_TOKEN);
	    }
	} else {
	    /* part_type == PERIODS_PART */
	    i = 0;
	    while (isdigit(token[i]) || (token[i] == '-')) {
		if (token[i] == '-') {
		    num_dashes++;
		    if (num_dashes > 1) return (UNKNOWN_TOKEN);
		}
		i++;
	    }
	    if (token[i] == '\0') {
		return (LIST_TOKEN);
	    }
	}

	/* otherwise, we don't know */
	return(UNKNOWN_TOKEN);
    }
}

char ** tokenize_vector(char * str, char delim)
{
    char ** tokenv = NULL;
    int tokenvsize = 0;
    
    while (*str != '\0') {
	/* allocate a tokenv entry */
	tokenvsize++;
	tokenv = (char **) check_realloc (tokenv, sizeof(char *) * tokenvsize);
	tokenv[tokenvsize-1] = str;

	/* forward the str char* until we are at the end of
	** the string, or at a \0 character */
	while ( (*str != '\0') && (*str != delim) )
	{
	    str++;
	}
	
	if (*str == delim) {
	    *str = '\0';
	    str++;
	}
    }

    /* Put in a null entry so we know when to stop */
    tokenvsize++;
    tokenv = (char **) check_realloc (tokenv, sizeof(char *) * tokenvsize);
    tokenv[tokenvsize-1] = NULL;

    return (tokenv);
}

