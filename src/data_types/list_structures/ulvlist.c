/* ulvlist.c */
/* Copyright (c) Patrick Cronin 25 January 2004 */
/* Information about this list structure can be found in ulvlist.h */

#include <stdio.h>
#include <stdlib.h>
#include "ulvlist.h"
#include "../../program/memory.h"

ulvlist ulvlist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    ulvlist l = (ulvlist) check_malloc (sizeof(unordered_looping_void_list));
    l->head = (element *) check_malloc (sizeof(element));
    ulvlist_init(l, el_free, el_display, el_compare, el_copy, el_compare_id);
    return (l);
}

void ulvlist_init (ulvlist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    l->head->data = NULL;
    l->head->next = NULL;
    l->size = 0;
    l->position = NULL;
    l->el_free = el_free;
    l->el_display = el_display;
    l->el_compare = el_compare;
    l->el_copy = el_copy;
    l->el_compare_id = el_compare_id;
}

void ulvlist_free_shallow (ulvlist l)
{
    element * current, *next;
    current = l->head->next;
    while (current != NULL) {
	next = current->next;
	free(current);
	current = next;
    }
    free(l->head);
    free(l);
}

void ulvlist_free_deep (ulvlist l)
{
    element * current, *next;
    current = l->head->next;
    while (current != NULL) {
	next = current->next;
	l->el_free(current->data);
	free(current);
	current = next;
    }
    free(l->head);
    free(l);
}

void ulvlist_insert_beginning (ulvlist l, void * newel)
{
    /* The newel must already have had memory allocated to itself! */
    element * old_first_element = l->head->next;
    l->head->next = (element *) check_malloc (sizeof(element));
    ((element *)l->head->next)->data = newel;
    ((element *)l->head->next)->next = old_first_element;
    (l->size)++;
}

void ulvlist_insert_end (ulvlist l, void * newel)
{
    element * ptr = l->head;
    
    while (ptr->next != NULL) {
	ptr = ptr->next;
    }
    ptr->next = (element *) check_malloc (sizeof(element));
    ((element *) ptr->next)->data = newel;
    ((element *) ptr->next)->next = NULL;
    (l->size)++;
}

void ulvlist_display (ulvlist l)
{
    element * ptr = l->head->next;
    printf("Size: %d\n", l->size);
    while (ptr != NULL) {
	l->el_display(ptr->data);
	ptr = ptr->next;
    }
}

ulvlist ulvlist_copy_shallow (ulvlist l)
{
    element *ptr, *newptr;
    ulvlist newlist = ulvlist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    ptr = l->head->next;
    newptr = newlist->head;

    while  (ptr != NULL) {
	newptr->next = (element *) check_malloc (sizeof(element));
	newptr = newptr->next;
	/* The position copying could be improved slightly */
	if (l->position == ptr) {
	    newlist->position = newptr;
	}
	newptr->data = ptr->data;
	ptr = ptr->next;
    }
    newptr->next = NULL;
    newlist->size = l->size;

    return (newlist);
}

ulvlist ulvlist_copy_deep (ulvlist l)
{
    element *ptr, *newptr;
    ulvlist newlist = ulvlist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    ptr = l->head->next;
    newptr = newlist->head;

    while  (ptr != NULL) {
	newptr->next = (element *) check_malloc (sizeof(element));
	newptr = newptr->next;
	/* The position copying could be improved slightly */
	if (l->position == ptr) {
	    newlist->position = newptr;
	}
	newptr->data = l->el_copy(ptr->data);
	ptr = ptr->next;
    }
    newptr->next = NULL;
    newlist->size = l->size;
    
    return (newlist);
}

int ulvlist_isempty (ulvlist l)
{
    return ( (ulvlist_size(l) == 0) ? 1 : 0);
}
    
int ulvlist_size (ulvlist l)
{
    return (l->size);
}

int ulvlist_ismember (ulvlist l, void * elem)
{
    if (ulvlist_find (l, elem) != NULL) return 1;
    return 0;
}

void * ulvlist_find (ulvlist l, void * elem)
{
    element * ptr = l->head->next;
    while (ptr != NULL) {
	if (l->el_compare(ptr->data, elem)) {
	    return ptr->data;
	}
	ptr = ptr->next;
    }
    return NULL;
}

void * ulvlist_find_id (ulvlist l, int id)
/* finds an element by its id (using a pre-specified el_compare_id function */
{
    element * ptr = l->head->next;
    while (ptr != NULL) {
	if (l->el_compare_id(ptr->data, id)) {
	    return ptr->data;
	}
	ptr = ptr->next;
    }
    return NULL;
}

void ulvlist_remove_element_shallow (ulvlist l, void * elem)
{
    element * ptr, *saved;
    ptr = l->head;
    
    saved = ptr;
    ptr = ptr->next;
    while (ptr != NULL) {
	if (l->el_compare(ptr->data, elem)) {
	    saved->next = ptr->next;
	    free(ptr);
	    (l->size)--;
	    return;
	}
	saved = ptr;
	ptr = ptr->next;
    }
    
    fprintf(stderr, "ulvlist: attempted to remove a non-existent element (non-fatal)\n");
}

void ulvlist_remove_element_by_ptr_shallow(ulvlist l, void * elem)
{
    element * ptr, *saved;
    ptr = l->head;
    
    saved = ptr;
    ptr = ptr->next;
    while (ptr != NULL) {
	if (ptr->data == elem) {
	    saved->next = ptr->next;
	    free(ptr);
	    (l->size)--;
	    return;
	}
	saved = ptr;
	ptr = ptr->next;
    }
    
    fprintf(stderr, "ulvlist: attempted to remove a non-existent element (non-fatal)\n");
}


void ulvlist_remove_element_deep (ulvlist l, void * elem)
{
    element * ptr, *saved;
    ptr = l->head;
    
    saved = ptr;
    ptr = ptr->next;
    while (ptr != NULL) {
	if (l->el_compare(ptr->data, elem)) {
	    saved->next = ptr->next;
	    l->el_free(ptr->data);
	    free(ptr);
	    (l->size)--;
	    return;
	}
	saved = ptr;
	ptr = ptr->next;
    }
    
    fprintf(stderr, "ulvlist: attempted to remove a non-existent element (non-fatal)\n");
}

void* ulvlist_pop_first (ulvlist l)
{
    void * first, *next;

    if (l->size == 0) {
	fprintf(stderr, "ulvlist: attempted to remove from an empty list (non-fatal)\n");
	return NULL;
    }
    first = ((element *)l->head->next)->data;
    next = ((element *)l->head->next)->next;
    free(l->head->next);
    l->head->next = next;
    (l->size)--;
    return first;
}

void* ulvlist_ptr_to_first_element (ulvlist l)
/* Returns an element* type-casted to a void* for use with
** ulvlist_ptr_to_next_element and ulvlist_data_at_ptr.
*/
{
    if (l->size == 0) {
	fprintf(stderr, "ulvlist: attempted to get ptr to first element of an empty list (non-fatal)\n");
    }
    return ((void*)l->head->next);
}

void* ulvlist_ptr_to_next_element (ulvlist l, void* ptr)
/* The void* `ptr' needs to actually be an element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function does not care what list it is actually
** operating on (unless the ptr->next == NULL)! You should be sure
** that the ptr you sent to it came from one of the ulvlist functions
** (either ulvlist_ptr_to_first_element or ulvlist_ptr_to_next_element)!
**
** 
*/
{
    element * p = ((element *) ptr)->next;
    if (p == NULL) return (l->head->next);
    return (p);
}

void* ulvlist_data_at_ptr (void* ptr)
/* The void* `ptr' needs to actually be an element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function has no idea what list it is actually
** operating on! You should be sure that the ptr you sent to it came
** from one of the ulvlist functions (either ulvlist_ptr_to_first_element
** or ulvlist_ptr_to_next_element)!
*/
{
    return ( ((element *) ptr)->data );
}


/* NOTES ON USING THE FOLLOWING LIST SCANNING FUNCTIONS
**
** You MUST call ulvlist_init_pos before any of the other following
** functions.  Otherwise, the list's position pointer will be NULL.
** If you are using the functions, and then you remove an element
** from the list, then it is possible that the position pointer is
** no longer valid!  Be aware of that!
*/
int ulvlist_init_pos (ulvlist l)
{
    if (l->size > 0) {
	l->position = l->head->next;
	return(1);
    }
    return(0);
}

int ulvlist_move_pos (ulvlist l)
/* This function will move the list's position pointer if possible.
** It will return 1 if the new pointer can be used, or 0 if the new
** pointer is invalid (NULL).
*/
{
    if (l->position) {
	l->position = l->position->next;
	if (l->position) {
	    return(1);
	}
    }
    return(0);
}

void* ulvlist_data_at_pos (ulvlist l)
{
    if (l->position) {
	return ((void*) l->position->data);
    }
    return(NULL);
}
