/* day.c
** storing information of a day
** Patrick Cronin (c) 10 April 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day.h"
#include "../../program/memory.h"

day day_constructor (void)
{
    day newday = (day) check_malloc (sizeof(day_struct));
    day_init(newday);
    return (newday);
}

void day_init (day s)
{
    s->name = NULL;
}

void day_set_all (day s, char * name)
{
    day_set_name(s, name);
}

void day_set_name (day s, char * name)
{
    if (s->name != NULL) free (s->name);
    s->name = (char *) check_malloc (sizeof(char) * (strlen(name) + 1));
    strcpy(s->name, name);
}

char * day_get_name (day s)
{
    return (s->name);
}

int day_compare_names (day s1, day s2)
{
    if (s1->name == s2->name) {
	return (1);
    }
    return (0);
}

void day_free (day s)
{
    if (s->name != NULL) {
	free (s->name);
    }
    free (s);
}

void day_display (day s)
{
    printf ("%s\n", s->name);    
}
