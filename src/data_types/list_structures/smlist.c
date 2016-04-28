/* smlist.c
** A simple list structure for strings.
** Best for small lists -- the list is stored in consecutive
** memory addresses.
** Copyright 8 April 2004 Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../program/memory.h"
#include "smlist.h"

smlist smlist_constructor(void)
{
    smlist newsmlist = (smlist) check_malloc (sizeof(smlist_struct));
    smlist_init (newsmlist);
    return (newsmlist);
}

void smlist_init(smlist s)
{
    s->size = 0;
    s->list = NULL;
}

int smlist_find(smlist s, module mod_addr)
{
    int c=0;
    
    while (c < s->size) {
	if (s->list[c]->mod == mod_addr) {
	    return(c);
	}
	c++;
    }
    /* not found */
    return(-1);
}

int smlist_get_size(smlist s)
{
    return (s->size);
}

module smlist_get_module_n(smlist s, int n)
{
    if (n >= s->size) {
	fprintf(stderr, "smlist: a non-existant element was requested. (non-fatal)\n");
	return (NULL);
    }
    return (s->list[n]->mod);
}

char * smlist_get_value_n(smlist s, int n)
{
    if (n >= s->size) {
	fprintf(stderr, "smlist: a non-existant element was requested. (non-fatal)\n");
	return (NULL);
    }
    return (s->list[n]->value);
}

void smlist_insert(smlist s, module mod_addr, char * str)
{
/*     mod_footprint mf = (mod_footprint) check_malloc (sizeof(mod_footprint_struct)); */

    s->list = (mod_footprint *) check_realloc (s->list, sizeof(mod_footprint_struct) * (s->size + 1));
    s->list[s->size] = (mod_footprint) check_malloc (sizeof(mod_footprint_struct));
    s->list[s->size]->mod = mod_addr;
    s->list[s->size]->value = (char *) check_malloc (sizeof(char) * (strlen(str) + 1));
    strncpy(s->list[s->size]->value, str, strlen(str) + 1);
    /* mf->mod = mod_addr;
    mf->value = (char *) check_malloc (sizeof(char) * (strlen(str) + 1));
    strncpy(mf->value, str, (strlen(str) + 1));
    s->list[s->size] = mf;
    */
    s->size++;
}

void smlist_free(smlist s)
{
    int i;

    if (s->list != NULL) {
	for (i=0; i<s->size; i++) {
	    free(s->list[i]->value);
	    free(s->list[i]);
	}
	free (s->list);
    }
    free(s);
}

void smlist_display(smlist s)
{
    int c;
    printf("list size: %d\n", s->size);
    for(c=0; c<s->size; c++) {
	printf("element %d: %s\n", c, s->list[c]->value);
    }
}
