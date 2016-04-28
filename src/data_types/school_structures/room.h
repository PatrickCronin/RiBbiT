/* room.h
** storing information of a room
** Patrick Cronin (c) 15 February 2004
*/

#ifndef ROOM
#define ROOM

#include <stdio.h>
#include <stdlib.h>

struct _room {
    char * name;
};
typedef struct _room room_struct;
typedef struct _room * room;

room room_constructor (void);
void room_init (room);
void room_set_all (room, char*);
void room_set_name (room, char*);
char * room_get_name (room);
int room_compare_names (room, room);
void room_free (room); 
void room_display (room);

#endif
