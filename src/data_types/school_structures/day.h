/* day.h
** storing information of a day
** Patrick Cronin (c) 10 April 2004
*/

#ifndef DAY_H
#define DAY_H

#include <stdio.h>
#include <stdlib.h>

struct _day {
    char * name;
};
typedef struct _day day_struct;
typedef struct _day * day;

day day_constructor (void);
void day_init (day);
void day_set_all (day, char*);
void day_set_name (day, char*);
char * day_get_name (day);
int day_compare_names (day, day);
void day_free (day); 
void day_display (day);

#endif
