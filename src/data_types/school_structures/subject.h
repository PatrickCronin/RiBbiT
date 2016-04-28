/* subject.h
** storing information of a subject
** Patrick Cronin (c) 15 February 2004
*/

#ifndef SUBJECT
#define SUBJECT

#include <stdio.h>
#include <stdlib.h>

struct _subject {
    char * name;
};
typedef struct _subject subject_struct;
typedef struct _subject * subject;

subject subject_constructor (void);
void subject_init (subject);
void subject_set_all (subject, char*);
void subject_set_name (subject, char*);
char * subject_get_name (subject);
int subject_compare_names (subject, subject);
void subject_free (subject); 
void subject_display (subject);

#endif
