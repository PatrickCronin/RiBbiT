/* class.h
** storing information of a class
** Patrick Cronin (c) 15 February 2004
*/

#ifndef CLASS
#define CLASS

#include <stdio.h>
#include <stdlib.h>

struct _class {
    char * name;
};
typedef struct _class class_struct;
typedef struct _class * class;

class class_constructor (void);
void class_init (class);
void class_set_all (class, char*);
void class_set_name (class, char*);
char * class_get_name (class);
int class_compare_names (class, class);
void class_free (class); 
void class_display (class);

#endif
