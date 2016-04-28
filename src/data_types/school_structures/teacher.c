/* teacher.c
** storing information of a teacher
** Patrick Cronin (c) 15 February 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include "teacher.h"
#include "../../program/memory.h"

teacher teacher_constructor (void)
{
    teacher newteacher = (teacher) check_malloc (sizeof(teacher_struct));
    teacher_init(newteacher);
    return (newteacher);
}

void teacher_init (teacher t)
{
    t->name = NULL;
}

void teacher_set_all (teacher t, char * name)
{
    teacher_set_name(t, name);
}

void teacher_set_name (teacher t, char * name)
{
    if (t->name != NULL) free (t->name);
    t->name = (char *) check_malloc (sizeof(char) * (strlen(name) + 1));
    strcpy(t->name, name);
}

char * teacher_get_name (teacher t)
{
    return (t->name);
}

int teacher_compare_names (teacher t1, teacher t2)
{
    if (t1->name == t2->name) {
	return (1);
    }
    return (0);
}

void teacher_free (teacher t)
{
    if (t->name != NULL) {
	free (t->name);
    }
    free (t);
}

void teacher_display (teacher t)
{
    printf ("%s\n", t->name);    
}
