/* ttt.h
** Teacher Time Table header file
** Describes the structure and functions of an ttt data type.
** Patrick Cronin
** Copyright (c) 5 March 2004
*/

#ifndef TEACHER_TIME_TABLE
#define TEACHER_TIME_TABLE

#include <gmp.h>
#include "stc.h"
#include "mtt.h"
#include "../misc_structures/data.h"

struct _teachertimetable {
    stc * timetable;
};
typedef struct _teachertimetable teachertimetable_struct;
typedef struct _teachertimetable* teachertimetable;

teachertimetable ttt_constructor(void);
void ttt_init(teachertimetable);
stc ttt_get_entry_at(teachertimetable, int, int);
void ttt_set_entry_at(teachertimetable, int, int, stc);
int ttt_make_loc_int(teachertimetable, int, int);
void ttt_check_entry_location(teachertimetable, int);
void ttt_display(teachertimetable);
void ttt_free_shallow(teachertimetable);
teachertimetable ttt_make_teacher_table(mastertimetable, program_data, int);
void ttt_write_teacher_table(teachertimetable, program_data, int, FILE *);
#endif
