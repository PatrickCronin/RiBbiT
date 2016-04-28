/* stc.c
** Storage for a subject, teacher, class timetable element
** (c) Copyright Patrick Cronin 29 February 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include "../../program/memory.h"
#include "stc.h"

stc stc_constructor(void)
{
    stc newstc = (stc) check_malloc (sizeof(stc_struct));
    stc_init(newstc);
    return(newstc);
}

void stc_init(stc s)
{
    s->id = -1;
    s->subject = -1;
    s->teachers = silist_constructor();
    s->class = -1;
    s->room = -1;
    s->num_periods = -1;
    s->period_num = -1;
    s->free_period = 0; /* not a free period == the default */
}

void stc_free_shallow(stc s)
{
    free(s);
}

void stc_free_deep(stc s)
{
    silist_free(s->teachers);
    free(s);
}

void stc_set_id(stc s, int i)
{
    s->id = i;
}

int stc_get_id(stc s)
{
    return (s->id);
}

void stc_set_subject(stc s, int n)
{
    s->subject = n;
}

void stc_set_class(stc s, int n) {
    s->class = n;
}

void stc_set_room(stc s, int n) {
    s->room = n;
}

void stc_add_teacher(stc s, int n)
{
    silist_insert(s->teachers, n);
}

void stc_set_teachers(stc s, silist l)
{
    s->teachers = l;
}

void stc_set_num_periods(stc s, int n)
{
    s->num_periods = n;
}

void stc_set_period_num(stc s, int n)
{
    s->period_num = n;
}

int stc_get_subject(stc s)
{
    return (s->subject);
}

int stc_get_class(stc s)
{
    return (s->class);
}

int stc_get_room(stc s)
{
    return (s->room);
}

silist stc_get_teachers(stc s)
{
    return (s->teachers);
}

int stc_get_num_periods(stc s)
{
    return (s->num_periods);
}

int stc_get_period_num(stc s)
{
    return (s->period_num);
}

stc stc_shallow_copy(stc s)
{
    stc newstc = stc_constructor();
    newstc->id = s->id;
    newstc->subject = s->subject;
    newstc->class = s->class;
    newstc->room = s->room;
    newstc->num_periods = s->num_periods;
    newstc->period_num = s->period_num;
    newstc->free_period = s->free_period;

    /* free the to-be-unused teacherlist (this is a shallow copy) */
    silist_free(newstc->teachers);
    newstc->teachers = s->teachers;

    return(newstc);
}

stc stc_deep_copy(stc s)
{
    stc newstc = stc_constructor();
    newstc->id = s->id;
    newstc->subject = s->subject;
    newstc->class = s->class;
    newstc->room = s->room;
    newstc->num_periods = s->num_periods;
    newstc->period_num = s->period_num;
    newstc->free_period = s->free_period;

    /* free the teacherlist and copy the other one */
    silist_free(newstc->teachers);
    newstc->teachers = silist_copy_deep(s->teachers);

    return(newstc);
}

int stc_is_teaching(stc s, int n)
{
    if (silist_find(s->teachers, n) != -1) {
	/* found */
	return 1;
    }
    return 0;
}

int stc_get_num_teachers(stc s)
{
    return (silist_get_size(s->teachers));
}

int stc_get_teacher_n(stc s, int n)
{
    return (silist_get_element_n(s->teachers, n));
}

/* A specific function for making sure that we don't generate
** time tables that are all the same, minus the fact that the
** STC ids of the same subject and class are in different days
** and periods -> (avoiding combinations that have the same
** subjects at the same time, but the STC ids moved around).
*/
int stc_compare_types_stcs(stc s1, stc s2)
{
    if (
	(s1->subject == s2->subject) &&
	(s1->num_periods == s2->num_periods)
	)
	return(1);
    return (0);
}

void stc_set_free_period(stc s, char c)
/* 1 is yes, 0 is no */
{
    s->free_period = c;
}

int stc_is_free_period(stc s)
{
    return(s->free_period == 1);
}
