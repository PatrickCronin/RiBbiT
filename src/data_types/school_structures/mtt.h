/* mtt.h
** Master Time Table header file
** Describes the structure and functions of an mtt data type.
** Patrick Cronin
** Copyright (c) 5 March 2004
*/

#ifndef MASTER_TIME_TABLE
#define MASTER_TIME_TABLE

#include <gmp.h>
#include "stc.h"
#include "../misc_structures/data.h"
#include "../list_structures/muvdalist.h"

struct _mastertimetable {
    stc * timetable;
};
typedef struct _mastertimetable mastertimetable_struct;
typedef struct _mastertimetable* mastertimetable;

mastertimetable mtt_constructor(void);
void mtt_init(mastertimetable);
stc mtt_get_entry_at(mastertimetable, int, int, int);
stc mtt_pop_entry_at(mastertimetable, int, int, int);
void mtt_set_entry_at(mastertimetable, int, int, int, stc);
int mtt_make_loc_int(mastertimetable, int, int, int);
void mtt_check_entry_location(mastertimetable, int);
void mtt_display(mastertimetable);
void mtt_save_solution(mastertimetable, program_data, mpz_t);
void mtt_save_partial_solution(mastertimetable, program_data, mpz_t, int, muvdalist);
void mtt_write_solution(mastertimetable, program_data, FILE *);
void mtt_write_class_table(mastertimetable, program_data, FILE *);
void mtt_free_shallow(mastertimetable);
#endif
