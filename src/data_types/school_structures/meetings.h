/* meetings.h
** storing information of meetings
** Patrick Cronin (c) 15 February 2004
*/

#ifndef MEETINGS
#define MEETINGS

#include <stdio.h>
#include <stdlib.h>
#include "../list_structures/silist.h"
#include "../list_structures/smlist.h"

struct _meetings {
    int class;
    int subject;
    silist teachers;
    int room;
    int hours_per_week;
    silist periods_arrangement;
    smlist module_attributes;
};
typedef struct _meetings meetings_struct;
typedef struct _meetings * meetings;

meetings meetings_constructor (void);
void meetings_init (meetings);
void meetings_set_all (meetings, int, int, int, int);
void meetings_set_class (meetings, int);
void meetings_set_subject (meetings, int);
void meetings_set_teacher (meetings, int);
void meetings_add_teacher (meetings, int);
void meetings_add_period_arrangement (meetings, int);
void meetings_set_room (meetings, int);
void meetings_set_hours_per_week (meetings, int);
int meetings_get_class (meetings);
int meetings_get_subject (meetings);
silist meetings_get_teachers (meetings);
silist meetings_get_periods_arrangement (meetings);
int meetings_get_room (meetings);
int meetings_get_hours_per_week (meetings);
void meetings_free (meetings); 
void meetings_display (meetings);
int meetings_get_num_mods (meetings);
module meetings_get_mod_n (meetings, int);
char * meetings_get_mod_value_n (meetings, int);
void meetings_insert_module(meetings, module, char*);
#endif




