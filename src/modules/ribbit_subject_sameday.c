/* ribbit_subject_sameday.c
** Module for ribbit to determine if a subject is already being
** taught on this day.
** Copyright (c) 27 March 2004 by Patrick Cronin
*/

#include <stdio.h>
#include "../program/module.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/school_structures/mtt.h"
#include "../data_types/school_structures/stc.h"


int mod_initialize (module m)
{
    m->type = MOD_GENERAL;
    return(1);
}

/* return 1 if stc wouldn't fit, 0 if no problem */
int mod_check_stc_day (ulvpclist list, stc cur, int day)
{
  stc temp_stc;

  if (stc_is_free_period(cur)) {
    return(0);
  }

  ulvpclist_init_pos(list);
    while ((temp_stc = (stc) ulvpclist_data_at_pos(list)) != NULL) {
      if (
	  (stc_get_subject(cur) == stc_get_subject(temp_stc)) &&
	  (stc_get_id(cur) != stc_get_id(temp_stc))
	  ) return (1);
      ulvpclist_move_pos(list);
    }
    
    return(0);
}

/* return 1 if subject would be doubled up, 0 if no problem */
int mod_check_stc (bigthing b, stc cur)
{
    int period;
    stc temp_stc;
    
    if (stc_is_free_period(cur)) {
	return(0);
    }

    period = b->pos_period - 1;
    
    while (period >= 0) {
	temp_stc = mtt_get_entry_at(b->tt, b->pos_class, b->pos_day, period);
	if (
	    (stc_get_subject(cur) == stc_get_subject(temp_stc)) &&
	    (stc_get_id(cur) != stc_get_id(temp_stc))
	    ) return (1);
	period--;
    }
    return(0);
}
