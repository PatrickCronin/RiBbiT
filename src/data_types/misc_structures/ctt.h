/* ctt.h
** Character Time Table header file
** Describes the structure and functions of an ctt data type.
** Patrick Cronin
** Copyright (c) 5 March 2004
*/

#ifndef CHARACTER_TIME_TABLE
#define CHARACTER_TIME_TABLE

#include "data.h"

struct _charactertimetable {
    char * timetable;
};
typedef struct _charactertimetable charactertimetable_struct;
typedef struct _charactertimetable* charactertimetable;

charactertimetable ctt_constructor(void);
void ctt_init(charactertimetable);
char ctt_get_entry_at(charactertimetable, int, int, int);
char ctt_pop_entry_at(charactertimetable, int, int, int);
void ctt_set_entry_at(charactertimetable, int, int, int, char);
int ctt_make_loc_int(charactertimetable, int, int, int);
void ctt_check_entry_location(charactertimetable, int);
void ctt_display(charactertimetable);
#endif
