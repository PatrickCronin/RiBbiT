/* meetings.c
** storing information of meetings
** NB: A meetings structure is plural because we store the adjacency
** inforamtion (meaning a meetings struct can hold 5 times a class meets
** with a teacher in a certain place at a certain time).
** Patrick Cronin (c) 15 February 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include "../../program/memory.h"
#include "meetings.h"
#include "teacher.h"
#include "../list_structures/silist.h"

meetings meetings_constructor (void)
{
    meetings newmeetings = (meetings) check_malloc (sizeof(meetings_struct));
    meetings_init(newmeetings);
    return (newmeetings);
}

void meetings_init (meetings m)
{
    m->class = 0;
    m->subject = 0;
    m->teachers = silist_constructor();
    m->room = 0;
    m->hours_per_week = 0;
    m->periods_arrangement = silist_constructor();
    m->module_attributes = smlist_constructor();
}

void meetings_set_all (meetings m, int c, int s, int r, int h)
{
    meetings_set_class(m, c);
    meetings_set_subject(m, s);
    meetings_set_room(m, r);
    meetings_set_hours_per_week(m, h);
}

void meetings_set_class (meetings m, int c)
{
    m->class = c;
}

void meetings_set_subject (meetings m, int s)
{
    m->subject = s;
}

void meetings_add_teacher(meetings m, int t)
{
    silist_insert(m->teachers, t);
}

void meetings_add_period_arrangement(meetings m, int a)
{
    silist_insert(m->periods_arrangement, a);
}

void meetings_set_room(meetings m, int r)
{
    m->room = r;
}

void meetings_set_hours_per_week(meetings m, int h)
{
    m->hours_per_week = h;
}

int meetings_get_class (meetings m)
{
    return (m->class);
}

int meetings_get_subject (meetings m)
{
    return (m->subject);
}

silist meetings_get_teachers (meetings m)
{
    return (m->teachers);
}

silist meetings_get_periods_arrangement (meetings m)
{
    return (m->periods_arrangement);
}

int meetings_get_room (meetings m)
{
    return (m->room);
}

int meetings_get_hours_per_week(meetings m)
{
    return (m->hours_per_week);
}

void meetings_free (meetings m)
{
    silist_free(m->teachers);
    silist_free(m->periods_arrangement);
    free (m);
}

void meetings_display (meetings m)
{
    printf ("class: %d\n", m->class);
    printf ("subject: %d\n", m->subject);
    printf ("teacher ids:---\n");
    silist_display(m->teachers);
    printf ("---end teacher ids\n");
    printf ("room: %d\n", m->room);
    printf ("hours_per_week: %d\n", m->hours_per_week);
    printf ("periods arrangement:---\n");
    silist_display(m->periods_arrangement);
    printf ("---end periods arrangement\n");
}

int meetings_get_num_mods (meetings m)
{
    return(smlist_get_size(m->module_attributes));
}

module meetings_get_mod_n (meetings m, int n)
{
    return(smlist_get_module_n(m->module_attributes, n));
}
    
char * meetings_get_mod_value_n (meetings m, int n)
{
    return(smlist_get_value_n(m->module_attributes, n));
}

void meetings_insert_module (meetings meet, module mod, char * str)
{
    smlist_insert(meet->module_attributes, mod, str);
}
