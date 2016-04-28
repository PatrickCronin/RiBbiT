/* sslist.h
** A simple list structure for strings.
** Best for small lists -- the list is stored in consecutive
** memory addresses.
** Copyright 27 February 2004 Patrick Cronin
*/

#ifndef SIMPLE_STRING_LIST_H
#define SIMPLE_STRING_LIST_H

struct _sslist {
    int size;
    char ** list;
};
typedef struct _sslist sslist_struct;
typedef struct _sslist * sslist;

sslist sslist_constructor(void);
void sslist_init(sslist);
int sslist_find(sslist, char *);
int sslist_get_size(sslist);
char * sslist_get_element_n(sslist, int);
void sslist_insert(sslist, char *);
void sslist_free(sslist);
void sslist_display(sslist);

#endif
