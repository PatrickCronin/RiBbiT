/* ulvpclist.c */
/* Copyright (c) Patrick Cronin 26 December 2004 */
/* Information about this list structure can be found in ulvpclist.h */

#include <stdio.h>
#include <stdlib.h>
#include "ulvpclist.h"
#include "../../program/memory.h"

ulvpclist ulvpclist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    ulvpclist l = (ulvpclist) check_malloc (sizeof(unordered_looping_void_pc_list));
    l->head = (ulvpc_element *) check_malloc (sizeof(ulvpc_element));
    ulvpclist_init(l, el_free, el_display, el_compare, el_copy, el_compare_id);
    return (l);
}

void ulvpclist_init (ulvpclist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    l->head->data = NULL;
    l->head->next = NULL;
    l->size = 0;
    l->pc = 0;
    l->position = NULL;
    l->el_free = el_free;
    l->el_display = el_display;
    l->el_compare = el_compare;
    l->el_copy = el_copy;
    l->el_compare_id = el_compare_id;
}

void ulvpclist_free_shallow (ulvpclist l)
{
  ulvpc_element * current, *next;
  current = l->head->next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  free(l->head);
  free(l);
}

void ulvpclist_free_deep (ulvpclist l)
{
  ulvpc_element * current, *next;
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

void ulvpclist_insert_beginning (ulvpclist l, void * newel)
{
    /* The newel must already have had memory allocated to itself! */
    ulvpc_element * old_first_element = l->head->next;
    l->head->next = (ulvpc_element *) check_malloc (sizeof(ulvpc_element));
    ((ulvpc_element *)l->head->next)->data = newel;
    ((ulvpc_element *)l->head->next)->next = old_first_element;
    (l->size)++;
}

void ulvpclist_insert_end (ulvpclist l, void * newel)
{
  ulvpc_element * ptr = l->head;
    
  while (ptr->next != NULL) {
    ptr = ptr->next;
  }
  ptr->next = (ulvpc_element *) check_malloc (sizeof(ulvpc_element));
  ((ulvpc_element *) ptr->next)->data = newel;
  ((ulvpc_element *) ptr->next)->next = NULL;
  (l->size)++;
}

void ulvpclist_display (ulvpclist l)
{
    ulvpc_element * ptr = l->head->next;
    printf("Size: %d\n", l->size);
    printf("PC: %d\n", l->pc);
    while (ptr != NULL) {
	l->el_display(ptr->data);
	ptr = ptr->next;
    }
}

ulvpclist ulvpclist_copy_shallow (ulvpclist l)
{
    ulvpc_element *ptr, *newptr;
    ulvpclist newlist = ulvpclist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    ptr = l->head->next;
    newptr = newlist->head;

    while  (ptr != NULL) {
	newptr->next = (ulvpc_element *) check_malloc (sizeof(ulvpc_element));
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
    newlist->pc = l->pc;

    return (newlist);
}

ulvpclist ulvpclist_copy_deep (ulvpclist l)
{
    ulvpc_element *ptr, *newptr;
    ulvpclist newlist = ulvpclist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    ptr = l->head->next;
    newptr = newlist->head;

    while  (ptr != NULL) {
	newptr->next = (ulvpc_element *) check_malloc (sizeof(ulvpc_element));
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
    newlist->pc = l->pc;
    
    return (newlist);
}

int ulvpclist_isempty (ulvpclist l)
{
    return ( (ulvpclist_size(l) == 0) ? 1 : 0);
}
    
int ulvpclist_size (ulvpclist l)
{
    return (l->size);
}

int ulvpclist_pc (ulvpclist l)
{
  return (l->pc);
}

void ulvpclist_add_pc (ulvpclist l, int n)
{
  l->pc = l->pc + n;
}

void ulvpclist_sub_pc (ulvpclist l, int n)
{
  l->pc = l->pc - n;
}


int ulvpclist_ismember (ulvpclist l, void * elem)
{
    if (ulvpclist_find (l, elem) != NULL) return 1;
    return 0;
}

void * ulvpclist_find (ulvpclist l, void * elem)
{
    ulvpc_element * ptr = l->head->next;
    while (ptr != NULL) {
	if (l->el_compare(ptr->data, elem)) {
	    return ptr->data;
	}
	ptr = ptr->next;
    }
    return NULL;
}

void * ulvpclist_find_id (ulvpclist l, int id)
/* finds an ulvpc_element by its id (using a pre-specified el_compare_id function */
{
    ulvpc_element * ptr = l->head->next;
    while (ptr != NULL) {
	if (l->el_compare_id(ptr->data, id)) {
	    return ptr->data;
	}
	ptr = ptr->next;
    }
    return NULL;
}

void ulvpclist_remove_element_shallow (ulvpclist l, void * elem)
{
    ulvpc_element * ptr, *saved;
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
    
    fprintf(stderr, "ulvpclist: attempted to remove a non-existent element (non-fatal)\n");
}

void ulvpclist_remove_element_by_ptr_shallow(ulvpclist l, void * elem)
{
    ulvpc_element * ptr, *saved;
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
    
    fprintf(stderr, "ulvpclist: attempted to remove a non-existent element (non-fatal)\n");
}


void ulvpclist_remove_element_deep (ulvpclist l, void * elem)
{
    ulvpc_element * ptr, *saved;
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
    
    fprintf(stderr, "ulvpclist: attempted to remove a non-existent element (non-fatal)\n");
}

void* ulvpclist_pop_first (ulvpclist l)
{
    void * first, *next;

    if (l->size == 0) {
	fprintf(stderr, "ulvpclist: attempted to remove from an empty list (non-fatal)\n");
	return NULL;
    }
    first = ((ulvpc_element *)l->head->next)->data;
    next = ((ulvpc_element *)l->head->next)->next;
    free(l->head->next);
    l->head->next = next;
    (l->size)--;
    return first;
}

void* ulvpclist_ptr_to_first_element (ulvpclist l)
/* Returns an ulvpc_element* type-casted to a void* for use with
** ulvpclist_ptr_to_next_element and ulvpclist_data_at_ptr.
*/
{
    if (l->size == 0) {
	fprintf(stderr, "ulvpclist: attempted to get ptr to first element of an empty list (non-fatal)\n");
    }
    return ((void*)l->head->next);
}

void* ulvpclist_ptr_to_next_element (ulvpclist l, void* ptr)
/* The void* `ptr' needs to actually be an element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function does not care what list it is actually
** operating on (unless the ptr->next == NULL)! You should be sure
** that the ptr you sent to it came from one of the ulvpclist functions
** (either ulvpclist_ptr_to_first_element or ulvpclist_ptr_to_next_element)!
**
** 
*/
{
    ulvpc_element * p = ((ulvpc_element *) ptr)->next;
    if (p == NULL) return (l->head->next);
    return (p);
}

void* ulvpclist_data_at_ptr (void* ptr)
/* The void* `ptr' needs to actually be an ulvpc_element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function has no idea what list it is actually
** operating on! You should be sure that the ptr you sent to it came
** from one of the ulvpclist functions (either ulvpclist_ptr_to_first_element
** or ulvpclist_ptr_to_next_element)!
*/
{
    return ( ((ulvpc_element *) ptr)->data );
}


/* NOTES ON USING THE FOLLOWING LIST SCANNING FUNCTIONS
**
** You MUST call ulvpclist_init_pos before any of the other following
** functions.  Otherwise, the list's position pointer will be NULL.
** If you are using the functions, and then you remove a ulvpc_element
** from the list, then it is possible that the position pointer is
** no longer valid!  Be aware of that!
*/
int ulvpclist_init_pos (ulvpclist l)
{
    if (l->size > 0) {
	l->position = l->head->next;
	return(1);
    }
    return(0);
}

int ulvpclist_move_pos (ulvpclist l)
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

void* ulvpclist_data_at_pos (ulvpclist l)
{
    if (l->position) {
	return ((void*) l->position->data);
    }
    return(NULL);
}
