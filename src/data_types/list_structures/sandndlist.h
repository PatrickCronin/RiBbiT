/* sandndlist.h
** a small sized, auto-id, no duplication, no deletion list structure
** Patrick Cronin (c) 14 February 2004
*/

#ifndef _SANDND_LIST_H
#define _SANDND_LIST_H

struct _sandndlist {
    int size;
    void** list;
    int (*el_compare_name)();
    char* (*el_get_name)();
    void (*el_free)();
    void (*el_display)();
};
typedef struct _sandndlist sandndlist_struct;
typedef struct _sandndlist* sandndlist;

sandndlist sandndlist_constructor(int (*)(), char* (*)(), void (*)(), void (*)());
void sandndlist_init(sandndlist, int (*)(), char* (*)(), void (*)(), void (*)());
int sandndlist_insert(sandndlist, void *);
void sandndlist_free_shallow(sandndlist);
void sandndlist_free_deep(sandndlist);
void* sandndlist_find_element_by_id(sandndlist, int);
void* sandndlist_find_element_by_name(sandndlist, char *);
char* sandndlist_find_name_by_id(sandndlist, int);
int sandndlist_find_id_by_name(sandndlist, char *);
void sandndlist_display(sandndlist);
int sandndlist_size(sandndlist);

#endif
