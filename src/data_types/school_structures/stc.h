/* stc.h
** Storage for a subject, teacher, class timetable element
** (c) Copyright Patrick Cronin 29 February 2004
*/

#ifndef STCTUPLE
#define STCTUPLE

#include "../list_structures/silist.h"

struct _stc {
    int id;
    int subject;
    silist teachers;
    int class;
    int room;
    int num_periods;
    int period_num;
    char free_period; /* 0 for no, 1 for yes */
};
typedef struct _stc stc_struct;
typedef struct _stc* stc;

stc stc_constructor(void);
void stc_init(stc);
void stc_free_shallow(stc);
void stc_free_deep(stc);
void stc_set_id(stc, int);
int stc_get_id(stc);
void stc_set_subject(stc, int);
void stc_set_class(stc, int);
void stc_set_room(stc, int);
void stc_add_teacher(stc, int);
void stc_set_teachers(stc, silist);
void stc_set_num_periods(stc, int);
void stc_set_period_num(stc, int);
int stc_get_subject(stc);
int stc_get_class(stc);
int stc_get_room(stc);
silist stc_get_teachers(stc);
int stc_get_num_periods(stc);
int stc_get_period_num(stc);
stc stc_shallow_copy(stc);
stc stc_deep_copy(stc s);
int stc_is_teaching(stc, int);
int stc_get_num_teachers(stc);
int stc_get_teacher_n(stc, int);
int stc_compare_types_stcs(stc, stc);
void stc_set_free_period(stc, char);
int stc_is_free_period(stc);

#endif
