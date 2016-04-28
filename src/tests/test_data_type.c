#include <stdio.h>
#include <stdlib.h>
#include "test_data_type.h"
#include <string.h>


t_id_name id_name_init (void)
{
    t_id_name t;
    t = (t_id_name) malloc (sizeof(id_name));
    t->id = -1;
    t->name = NULL;
    return (t);
}

void id_name_free (t_id_name t)
{
    if (t->name != NULL) free(t->name);
    t->name = NULL;
    t->id = -1;
    free(t);
}

void id_name_setid (t_id_name t, int i)
{
    t->id = i;
}

int id_name_getid (t_id_name t)
{
    return t->id;
}

void id_name_setname (t_id_name t, char * n)
{
    if (t->name != NULL) free(t->name);
    t->name = (char *) malloc (sizeof(char) * (strlen(n) + 1) );
    strcpy (t->name, n);
}

char * id_name_getname (t_id_name t)
{
    return t->name;
}

void id_name_display (t_id_name t)
{
    printf ("ID: %d, Name: %s\n", t->id, t->name);
}

void * id_name_copy (t_id_name t)
{
    t_id_name new = id_name_init();
    id_name_setid(new, id_name_getid(t));
    id_name_setname(new, id_name_getname(t));
    return ((void *)new);
}

int id_name_compare (t_id_name t1, t_id_name t2)
{
    if (t1->id == t2->id) return 1;
    return 0;
}
