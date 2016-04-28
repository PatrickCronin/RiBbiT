/* cuvlist.c */
/* Circular Unordered Void List Structure and Functions */
/* Copyright (c) Patrick Cronin 02 February 2004 */
/* Information about this list structure can be found in cuvlist.h */

#include <stdio.h>
#include <stdlib.h>
#include "cuvlist.h"
#include "../../program/memory.h"

cuvlist cuvlist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    cuvlist l = (cuvlist) check_malloc (sizeof(circular_unordered_void_list));
    l->head = (element) check_malloc (sizeof(cuv_element));
    cuvlist_init(l, el_free, el_display, el_compare, el_copy, el_compare_id);
    return (l);
}

void cuvlist_init (cuvlist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    l->head->data = NULL;
    l->head->next = NULL;
    l->foot = NULL;
    l->size = 0;
    l->el_free = el_free;
    l->el_display = el_display;
    l->el_compare = el_compare;
    l->el_copy = el_copy;
    l->el_compare_id = el_compare_id;
}

void cuvlist_free_shallow (cuvlist l)
{
    element current, next, last;
    
    if (l->size != 0) {
	current = l->head->next;
	last = l->foot->next;
	do {
	    next = current->next;
	    free(current);
	    current = next;
	} while (current != last);
    }
    free(l->head);
    free(l);
}

void cuvlist_free_deep (cuvlist l)
{
    element current, next, last;
    
    if (l->size > 0) {
	current = l->head->next;
	last = l->foot->next;
	do {
	    next = current->next;
	    l->el_free(current->data);
	    free(current);
	    current = next;
	} while (current != last);
    }
    free(l->head);
    free(l);
}

void cuvlist_insert_beginning (cuvlist l, void * newel)
{
    /* The newel must already have had memory allocated to itself! */
    element old_first_element = l->head->next;
    l->head->next = (element) check_malloc (sizeof(cuv_element));
    ((element) l->head->next)->data = newel;

    /* Make sure the first element points to the old first element. */
    if (l->size == 0) {
	/* here, we also need to establish the `end' of the list */
	/* the l->foot->next will complete the loop for us very soon */
	l->foot = l->head->next;
    } else {
	((element) l->head->next)->next = old_first_element;
    }

    /* Re-establish the circle to include the new element */
    l->foot->next = l->head->next;

    /* Increase the size of the list */
    (l->size)++;
}

void cuvlist_insert_end (cuvlist l, void * newel)
{
    if (l->size == 0) cuvlist_insert_beginning (l, newel);

    else {
	/* Allocate memory and prepare the new element */
	l->foot->next = (element) check_malloc (sizeof(cuv_element));
	((element) l->foot->next)->data = newel;
	((element) l->foot->next)->next = l->head->next;
	
	/* Re-complete the circle */
	l->foot = l->foot->next;
	
	/* Increase the size of the list */
	(l->size)++;
    }
}

void cuvlist_display (cuvlist l)
{
    element ptr = l->head->next;
    
    printf("Size: %d\n", l->size);
    
    if (l->size > 0) {
	do {
	    l->el_display(ptr->data);
	    ptr = ptr->next;
	} while (ptr != l->foot->next);
    }
}

cuvlist cuvlist_copy_shallow (cuvlist l)
{
    element ptr, newptr;
    cuvlist newlist = cuvlist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    if (l->size > 0) {
	ptr = l->head->next;
	newptr = newlist->head;
	do {
	    newptr->next = (element) check_malloc (sizeof(cuv_element));
	    newptr = newptr->next;
	    newptr->data = ptr->data;
	    ptr = ptr->next;
	} while  (ptr != l->foot->next);
	newlist->foot = newptr;
	newptr->next = newlist->head->next;
    }
    newlist->size = l->size;
    
    return (newlist);
}

cuvlist cuvlist_copy_deep (cuvlist l)
{
    element ptr, newptr;
    cuvlist newlist = cuvlist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    if (l->size > 0) {
	ptr = l->head->next;
	newptr = newlist->head;
	do {
	    newptr->next = (element) check_malloc (sizeof(cuv_element));
	    newptr = newptr->next;
	    newptr->data = l->el_copy(ptr->data);
	    ptr = ptr->next;
	} while  (ptr != l->foot->next);
	newlist->foot = newptr;
	newptr->next = newlist->head->next;
    }
    newlist->size = l->size;
    
    return (newlist);
}

int cuvlist_isempty (cuvlist l)
{
    return ( (cuvlist_size(l) == 0) ? 1 : 0);
}
    
int cuvlist_size (cuvlist l)
{
    return (l->size);
}

int cuvlist_ismember (cuvlist l, void * elem)
{
    if (cuvlist_find (l, elem) != NULL) return 1;
    return 0;
}

void * cuvlist_find (cuvlist l, void * elem)
{
    element ptr;

    if (l->size > 0) {
	ptr = l->head->next;
	do {
	    if (l->el_compare(ptr->data, elem)) {
		return ptr->data;
	    }
	    ptr = ptr->next;
	} while (ptr != l->foot->next);
    }
    return NULL;
}

void * cuvlist_find_id (cuvlist l, int id)
/* finds an element by its id (using a pre-specified el_compare_id function */
{
    element first, ptr;

    if (l->size > 0) {
	first = l->head->next;
	ptr = first;
	do {
	    if (l->el_compare_id(ptr->data, id)) {
		return ptr->data;
	    }
	    ptr = ptr->next;
	} while (ptr != first);
    }
    return NULL;
}

void cuvlist_remove_element_shallow (cuvlist l, void * elem)
{
    element ptr, saved;

    if (l->size > 0) {
	saved = l->head;
	ptr = saved->next;
	do {
	    if (l->el_compare(ptr->data, elem)) {
		if (l->size == 1) {
		    l->head->next = NULL;
		    l->foot = NULL;
		} else {
		    saved->next = ptr->next;
		    /* if we deleted the last element, update l->foot */
		    if (ptr == l->foot) {
			l->foot = saved;
		    }
		}
		free(ptr);
		(l->size)--;
		return;
	    }
	    saved = ptr;
	    ptr = ptr->next;
	} while (ptr != l->foot->next);
    }

    fprintf(stderr, "cuvlist: attempted to remove a non-existent element (non-fatal)\n");
}

void cuvlist_remove_element_deep (cuvlist l, void * elem)
{
    element ptr, saved;

    if (l->size > 0) {
	saved = l->head;
	ptr = saved->next;
	do {
	    if (l->el_compare(ptr->data, elem)) {
		if (l->size == 1) {
		    l->head->next = NULL;
		    l->foot = NULL;
		} else {
		    saved->next = ptr->next;
		    /* if we deleted the last element, update l->foot */
		    if (ptr == l->foot) {
			l->foot = saved;
		    }
		}
		l->el_free(ptr->data);
		free(ptr);
		(l->size)--;
		return;
	    }
	    saved = ptr;
	    ptr = ptr->next;
	} while (ptr != l->foot->next);
    }

    fprintf(stderr, "cuvlist: attempted to remove a non-existent element (non-fatal)\n");
}

void* cuvlist_pop_first (cuvlist l)
{
    void *first, *new_first;

    if (l->size <= 0) {
	fprintf(stderr, "cuvlist: attempted to remove from an empty list (non-fatal)\n");
	return NULL;
    }

    first = ((element)l->head->next)->data;
    new_first = ((element)l->head->next)->next;
    free(l->head->next);
    if (l->size > 1) {
	l->head->next = new_first;
    } else {
	l->head->next = NULL;
	l->foot = NULL;
    }

    (l->size)--;

    return first;
}

void* cuvlist_ptr_to_first_element (cuvlist l)
/* Returns an element* type-casted to a void* for use with
** cuvlist_ptr_to_next_element and ulvlist_data_at_ptr.
*/
{
    if (l->size == 0) {
	fprintf(stderr, "cuvlist: attempted to get ptr to first element of an empty list (non-fatal)\n");
    }
    return ((void*)l->head->next);
}

void* cuvlist_ptr_to_next_element (void* ptr)
/* The void* `ptr' needs to actually be an element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function does not care what list it is actually
** operating on (or if it is even operating on a list)! You should be sure
** that the ptr you sent to it came from one of the cuvlist functions
** (either cuvlist_ptr_to_first_element or cuvlist_ptr_to_next_element)!
**
** 
*/
{
    return (((element) ptr)->next);
}

void* cuvlist_data_at_ptr (void* ptr)
/* The void* `ptr' needs to actually be an element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function has no idea what list it is actually
** operating on (or if it is even operating on a list)! You should be sure
** that the ptr you sent to it came from one of the ulvlist functions
** (either cuvlist_ptr_to_first_element or cuvlist_ptr_to_next_element)!
*/
{
    return ( ((element) ptr)->data );
}


