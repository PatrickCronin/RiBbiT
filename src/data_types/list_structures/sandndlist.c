#include <stdio.h>
#include <stdlib.h>
#include "sandndlist.h"
#include "../../program/memory.h"

sandndlist sandndlist_constructor(int (*el_compare_name)(), char* (*el_get_name)(), void (*el_free)(), void (*el_display)())
{
    sandndlist newlist = (sandndlist) check_malloc (sizeof(sandndlist_struct));
    sandndlist_init(newlist, el_compare_name, el_get_name, el_free, el_display);
    return (newlist);
}

void sandndlist_init(sandndlist l, int (*el_compare_name)(), char* (*el_get_name)(), void (*el_free)(), void (*el_display)())
{
    l->size = 0;
    l->el_compare_name = el_compare_name;
    l->el_get_name = el_get_name;
    l->el_free = el_free;
    l->el_display = el_display;
    l->list = NULL;
}

int sandndlist_insert(sandndlist l, void* elem)
{
    int i = sandndlist_find_id_by_name(l, l->el_get_name(elem));
    if (i == -1) {
	l->list = (void **) check_realloc (l->list, sizeof(void *) * (l->size+1) );
	l->list[l->size] = elem;
	(l->size)++;
	return (l->size - 1);
    }
    return (i);
    
}

void sandndlist_free_shallow(sandndlist l)
{
    if (l->size != 0) {
	free (l->list);
    }
    free (l);
}

void sandndlist_free_deep(sandndlist l)
{
    int c;
    for (c = 0; c < l->size; c++) {
	l->el_free(l->list[c]);
    }
    if (l->size != 0) {
	free (l->list);
    }
    free (l);
}


void* sandndlist_find_element_by_id(sandndlist l, int i)
{
    if (l->size - 1 < i) {
	/* The list isn't big enough to have that id */
	return (NULL);
    }
    return (l->list[i]);
}

void* sandndlist_find_element_by_name(sandndlist l, char* name)
{
    int c = 0;
    while (c < l->size) {
	if (strcmp (l->el_get_name(l->list[c]), name) == 0) {
	    return (l->list[c]);	    
	}
	c++;
    }
    return (NULL);
}

char* sandndlist_find_name_by_id(sandndlist l, int i)
{
    if (l->size - 1 < i) {
	/* the list isn't big enough to have that id */
	return (NULL);
    }
    return (l->el_get_name(l->list[i]));
}

int sandndlist_find_id_by_name(sandndlist l, char* name)
{
    int c = 0;
    while (c < l->size) {
	if (strcmp (l->el_get_name(l->list[c]), name) == 0) {
	    return (c);
	}
	c++;
    }
    return (-1);
}

void sandndlist_display (sandndlist l)
{
    int c;
    printf("List size: %d\n", l->size);
    for(c=0; c < l->size; c++) {
	printf("id %d: ", c);
	l->el_display(l->list[c]);
    }
}

int sandndlist_size(sandndlist l)
{
    return (l->size);
}
