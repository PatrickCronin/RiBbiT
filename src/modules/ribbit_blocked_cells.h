/* ribbit_blocked_cells.h
** Module for ribbit to determine if a subject, teacher, class,
** room, or a class meeting can use a specific space on the time
** table.
** Copyright (c) 3 April 2004 by Patrick Cronin
*/

#ifndef RIBBIT_BLOCKED_CELLS_H
#define RIBBIT_BLOCKED_CELLS_H

#include "../program/module.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/misc_structures/bct.h"
#include "../data_types/school_structures/stc.h"
#include "../data_types/list_structures/ulvpclist.h"

int mod_initialize (module);
int mod_register_id (int, int, char *);
int mod_pre_generation(bigthing);
int mod_check_stc (bigthing, stc);
int mod_check_stc_day (ulvpclist, stc, int);
int mod_blocked_find_pos_by_id(bct *, int, int);

#endif
