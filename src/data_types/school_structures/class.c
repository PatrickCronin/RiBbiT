/* class.c
** storing information of a class
** Patrick Cronin (c) 15 February 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include "class.h"
#include "../../program/memory.h"

class class_constructor (void)
{
    class newclass = (class) check_malloc (sizeof(class_struct));
    class_init(newclass);
    return (newclass);
}

void class_init (class c)
{
    c->name = NULL;
}

void class_set_all (class c, char * name)
{
    class_set_name(c, name);
}

void class_set_name (class c, char * name)
{
    if (c->name != NULL) free (c->name);
    c->name = (char *) check_malloc (sizeof(char) * (strlen(name) + 1));
    strcpy(c->name, name);
}

char * class_get_name (class c)
{
    return (c->name);
}

int class_compare_names (class c1, class c2)
{
    if (c1->name == c2->name) {
	return (1);
    }
    return (0);
}

void class_free (class c)
{
    if (c->name != NULL) {
	free (c->name);
    }
    free (c);
}

void class_display (class c)
{
    printf ("%s\n", c->name);    
}
