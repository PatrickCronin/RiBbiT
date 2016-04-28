/* smlist.h
** A simple list structure for module information.
** Best for small lists -- the list is stored in consecutive
** memory addresses.
** Copyright 08 April 2004 Patrick Cronin
*/

#ifndef SIMPLE_MODULE_LIST_H
#define SIMPLE_MODULE_LIST_H

#include "../../program/module.h"

struct _mod_footprint {
    module mod;
    char * value;
};
typedef struct _mod_footprint mod_footprint_struct;
typedef struct _mod_footprint * mod_footprint;

struct _smlist {
    int size;
    mod_footprint * list;
};
typedef struct _smlist smlist_struct;
typedef struct _smlist * smlist;

smlist smlist_constructor(void);
void smlist_init(smlist);
int smlist_find(smlist, module);
int smlist_get_size(smlist);
module smlist_get_module_n(smlist, int);
char * smlist_get_value_n(smlist, int);
void smlist_insert(smlist, module, char *);
void smlist_free(smlist);
void smlist_display(smlist);

#endif
