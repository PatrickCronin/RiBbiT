/* subject.c
** storing information of a subject
** Patrick Cronin (c) 15 February 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include "subject.h"
#include "../../program/memory.h"

subject subject_constructor (void)
{
    subject newsubject = (subject) check_malloc (sizeof(subject_struct));
    subject_init(newsubject);
    return (newsubject);
}

void subject_init (subject s)
{
    s->name = NULL;
}

void subject_set_all (subject s, char * name)
{
    subject_set_name(s, name);
}

void subject_set_name (subject s, char * name)
{
    if (s->name != NULL) free (s->name);
    s->name = (char *) check_malloc (sizeof(char) * (strlen(name) + 1));
    strcpy(s->name, name);
}

char * subject_get_name (subject s)
{
    return (s->name);
}

int subject_compare_names (subject s1, subject s2)
{
    if (s1->name == s2->name) {
	return (1);
    }
    return (0);
}

void subject_free (subject s)
{
    if (s->name != NULL) {
	free (s->name);
    }
    free (s);
}

void subject_display (subject s)
{
    printf ("%s\n", s->name);    
}
