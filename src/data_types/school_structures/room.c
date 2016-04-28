/* room.c
** storing information of a room
** Patrick Cronin (c) 15 February 2004
*/

#include <stdio.h>
#include <stdlib.h>
#include "room.h"
#include "../../program/memory.h"

room room_constructor (void)
{
    room newroom = (room) check_malloc (sizeof(room_struct));
    room_init(newroom);
    return (newroom);
}

void room_init (room r)
{
    r->name = NULL;
}

void room_set_all (room r, char * name)
{
    room_set_name(r, name);
}

void room_set_name (room r, char * name)
{
    if (r->name != NULL) free (r->name);
    r->name = (char *) check_malloc (sizeof(char) * (strlen(name) + 1));
    strcpy(r->name, name);
}

char * room_get_name (room r)
{
    return (r->name);
}

int room_compare_names (room r1, room r2)
{
    if (r1->name == r2->name) {
	return (1);
    }
    return (0);
}

void room_free (room r)
{
    if (r->name != NULL) {
	free (r->name);
    }
    free (r);
}

void room_display (room r)
{
    printf ("%s\n", r->name);    
}
