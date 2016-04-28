/* teacher.h
** storing information of a teacher
** Patrick Cronin (c) 15 February 2004
*/

#ifndef TEACHER
#define TEACHER

#include <stdio.h>
#include <stdlib.h>

struct _teacher {
    char * name;
};
typedef struct _teacher teacher_struct;
typedef struct _teacher * teacher;

teacher teacher_constructor (void);
void teacher_init (teacher);
void teacher_set_all (teacher, char*);
void teacher_set_name (teacher, char*);
char * teacher_get_name (teacher);
int teacher_compare_names (teacher, teacher);
void teacher_free (teacher); 
void teacher_display (teacher);

#endif
