/* bct.h
** Blocked Cells Table header file
** Describes the structure and functions of an bct data type.
** Patrick Cronin
** Copyright (c) 4 April 2004
*/

#ifndef BLOCKED_CELLS_TABLE_H
#define BLOCKED_CELLS_TABLE_H

#include "data.h"
#include "bigthing.h"
#include "../list_structures/silist.h"

struct _blocked_cells_table {
    int id;
    char * format_string;
    int size;
    char * timetable;
};
typedef struct _blocked_cells_table blocked_cells_table_struct;
typedef struct _blocked_cells_table *bct;

bct bct_constructor(int, char *);
void bct_init1(bct, int, char *);
void bct_init2(bct, int size);
void bct_display(bct);
void bct_mark(bct, int day, int period);
int bct_is_marked(bct, int day, int period);
int bct_make_and_check_loc_int(int day, int period);
int bct_get_id(bct);
void bct_prepare_table(bct, bigthing);
void bct_prepare_format_group(bct, char *, bigthing);
void bct_parse_part(bct, char *, silist, int, bigthing);
void bct_parse_entry_token(char *, silist);
void bct_parse_list_token(char *, silist, int, bigthing);
void bct_parse_asterisk_token_for_days(silist);
void bct_parse_asterisk_token_for_periods(silist);
void bct_parse_day_token(char *, silist, bigthing);
int bct_get_token_type (char *, int);
char ** tokenize_vector(char *, char);
#endif
