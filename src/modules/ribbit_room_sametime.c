/* ribbit_room_sametime.c
** Module for ribbit to determine if a room is already
** beings used at this time.
** Copyright (c) 11 April 2004 by Patrick Cronin
*/

#include <stdio.h>
#include "../program/module.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/school_structures/mtt.h"
#include "../data_types/school_structures/stc.h"


int mod_initialize (module m)
{
    /* This module could be a MOD_FOR_ROOMS, but the room name
    ** is built in to the STC datatype, so it doesn't need to 
    ** be implemented as a MOD_FOR_ROOMS */
    m->type = MOD_GENERAL;
    return(1);
}

/* return 0 if OK, 1 if not */
int mod_check_stc (bigthing b, stc cur)
{
    int class;
    int room_num;
    stc temp_stc;

    /* Don't check free periods */
    if (stc_is_free_period(cur)) {
	return(0);
    }
    
    room_num = stc_get_room(cur);

    /* Don't check STCs with unassigned room ids */
    if (room_num == -1) {
	return(0);
    }

    class = b->pos_class - 1;

    while (class >= 0) {
	temp_stc = mtt_get_entry_at(b->tt, class, b->pos_day, b->pos_period);
	if (stc_get_room(temp_stc) == room_num) {
	    return(1);
	}
	class--;
    }
    return(0);
}
