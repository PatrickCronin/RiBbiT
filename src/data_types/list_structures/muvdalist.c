/* muvdalist.c */
/* Copyright (c) Patrick Cronin 25 January 2004 */
/* Information about this list structure can be found in muvdalist.h */

#include <stdio.h>
#include <stdlib.h>
#include "muvdalist.h"
#include "../../program/memory.h"

#define CAREFUL_MODE

muvdalist muvdalist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    muvdalist l = (muvdalist) check_malloc (sizeof(muvdalist_struct));
    muvdalist_init(l, el_free, el_display, el_compare, el_copy, el_compare_id);
    return (l);
}

inline void muvdalist_init (muvdalist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)())
{
    l->list = NULL;
    l->size = 0;
    l->num_marked = 0;
    l->position = -1;
    l->el_free = el_free;
    l->el_display = el_display;
    l->el_compare = el_compare;
    l->el_copy = el_copy;
    l->el_compare_id = el_compare_id;
}

void muvdalist_free_shallow (muvdalist l)
{
    int n = 0;
    while (n < l->size) {
    	free(l->list[n]);
	n++;
    }
    free(l->list);
    free(l);
}

void muvdalist_free_deep (muvdalist l)
{
    int n = 0;
    while (n < l->size) {
	l->el_free(l->list[n]->el);
    	free(l->list[n]);
	n++;
    }
    free(l->list);
    free(l);
}

void muvdalist_insert (muvdalist l, void * newel)
{
    l->list = (melement *) check_realloc (l->list, sizeof(melement) * (l->size + 1));
    l->list[l->size] = (melement) check_malloc (sizeof(melement_struct));
    l->list[l->size]->marked = 0;
    l->list[l->size]->el = newel;
    l->size++;
}

void muvdalist_display (muvdalist l)
{
    int i = 0;
    printf("Size: %d\n", l->size);
    while (i < l->size) {
	l->el_display(l->list[i]->el);
	i++;
    }
}

/*
Not changed over! -- Don't use
muvdalist muvdalist_copy_shallow (muvdalist l)
{
    element *ptr, *newptr;
    muvdalist newlist = muvdalist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    ptr = l->head->next;
    newptr = newlist->head;

    while  (ptr != NULL) {
	newptr->next = (element *) check_malloc (sizeof(element));
	newptr = newptr->next;
	/* The position copying could be improved slightly */
/*	if (l->position == ptr) {
	    newlist->position = newptr;
	}
	newptr->data = ptr->data;
	ptr = ptr->next;
    }
    newptr->next = NULL;
    newlist->size = l->size;

    return (newlist);
}

muvdalist muvdalist_copy_deep (muvdalist l)
{
    element *ptr, *newptr;
    muvdalist newlist = muvdalist_constructor(l->el_free, l->el_display, l->el_compare, l->el_copy, l->el_compare_id);

    ptr = l->head->next;
    newptr = newlist->head;

    while  (ptr != NULL) {
	newptr->next = (element *) check_malloc (sizeof(element));
	newptr = newptr->next;
	/* The position copying could be improved slightly */
/*	if (l->position == ptr) {
	    newlist->position = newptr;
	}
	newptr->data = l->el_copy(ptr->data);
	ptr = ptr->next;
    }
    newptr->next = NULL;
    newlist->size = l->size;
    
    return (newlist);
}
*/

inline int muvdalist_isempty (muvdalist l)
{
    return ( (muvdalist_size(l) == 0) ? 1 : 0);
}
    
inline int muvdalist_size (muvdalist l)
{
    return (l->size);
}
/*
int muvdalist_ismember (muvdalist l, void * elem)
{
    if (muvdalist_find (l, elem) != NULL) return 1;
    return 0;
}

void * muvdalist_find (muvdalist l, void * elem)
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

void * muvdalist_find_id (muvdalist l, int id)
/* finds an element by its id (using a pre-specified el_compare_id function */
/*
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
*/
/*
void muvdalist_remove_element_shallow (muvdalist l, void * elem)
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
    
    fprintf(stderr, "muvdalist: attempted to remove a non-existent element (non-fatal)\n");
}

void muvdalist_remove_element_by_ptr_shallow(muvdalist l, void * elem)
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
    
    fprintf(stderr, "muvdalist: attempted to remove a non-existent element (non-fatal)\n");
}


void muvdalist_remove_element_deep (muvdalist l, void * elem)
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
    
    fprintf(stderr, "muvdalist: attempted to remove a non-existent element (non-fatal)\n");
}
*/
/*
void* muvdalist_pop_first (muvdalist l)
{
    void * first, *next;

    if (l->size == 0) {
	fprintf(stderr, "muvdalist: attempted to remove from an empty list (non-fatal)\n");
	return NULL;
    }
    first = ((element *)l->head->next)->data;
    next = ((element *)l->head->next)->next;
    free(l->head->next);
    l->head->next = next;
    (l->size)--;
    return first;
}
*/
/*
void* muvdalist_ptr_to_first_element (muvdalist l)
/* Returns an element* type-casted to a void* for use with
** muvdalist_ptr_to_next_element and muvdalist_data_at_ptr.
*/
/*
{
    if (l->size == 0) {
	fprintf(stderr, "muvdalist: attempted to get ptr to first element of an empty list (non-fatal)\n");
    }
    return ((void*)l->head->next);
}
*/
/*
void* muvdalist_ptr_to_next_element (muvdalist l, void* ptr)
/* The void* `ptr' needs to actually be an element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function does not care what list it is actually
** operating on (unless the ptr->next == NULL)! You should be sure
** that the ptr you sent to it came from one of the muvdalist functions
** (either muvdalist_ptr_to_first_element or muvdalist_ptr_to_next_element)!
**
** 
*/
/*
{
    element * p = ((element *) ptr)->next;
    if (p == NULL) return (l->head->next);
    return (p);
}
void* muvdalist_data_at_ptr (void* ptr)
/* The void* `ptr' needs to actually be an element* typecasted to a
** void*.  Thus we have the following:
** Warning: this function has no idea what list it is actually
** operating on! You should be sure that the ptr you sent to it came
** from one of the muvdalist functions (either muvdalist_ptr_to_first_element
** or muvdalist_ptr_to_next_element)!
*/
/*
{
    return ( ((element *) ptr)->data );
}
*/

/* NOTES ON USING THE FOLLOWING LIST SCANNING FUNCTIONS
**
** You MUST call muvdalist_init_pos before any of the other following
** functions.  Otherwise, the list's position pointer will be NULL.
** If you are using the functions, and then you remove an element
** from the list, then it is possible that the position pointer is
** no longer valid!  Be aware of that!
*/
inline int muvdalist_init_pos (muvdalist l)
{
    if (l->size > 0) {
	l->position = 0;
	return(1);
    }
    return(0);
}

inline int muvdalist_move_pos (muvdalist l)
/* This function will move the list's position pointer if possible.
** It will return 1 if the new pointer can be used, or 0 if the new
** pointer is invalid (NULL).
*/
{
#ifdef CAREFUL_MODE
    if ((l->position + 1 < l->size) && (l->position >= 0)){
	l->position++;
	return(1);
    }
    return(0);
#else
    l->position++;
    return(1);
#endif
}

inline void* muvdalist_data_at_pos (muvdalist l)
{
#ifdef CAREFUL_MODE
    if ((l->position >= 0) && (l->position < l->size)) {
	return ((void*) l->list[l->position]->el);
    }
    return(NULL);
#else
    return ((void*) l->list[l->position]->el);
#endif
}

inline int muvdalist_get_num_unmarked (muvdalist l)
{
    return (l->size - l->num_marked);
}

inline void* muvdalist_get_element_n (muvdalist l, int n)
{
    if (n == -1) {
	return(NULL);
    }
#ifdef CAREFUL_MODE
    muvdalist_check_loc (l, n);
#endif
    return (l->list[n]->el);
}

inline int muvdalist_is_marked (muvdalist l, int n)
{
#ifdef CAREFUL_MODE
    muvdalist_check_loc(l, n);
#endif
    return (l->list[n]->marked == 1);
}

inline void muvdalist_mark_element (muvdalist l, int n)
{
#ifdef CAREFUL_MODE
    muvdalist_check_loc(l, n);
#endif
    if (! l->list[n]->marked) {
	l->list[n]->marked = 1;
	l->num_marked++;
    }
}

inline void muvdalist_unmark_element (muvdalist l, int n)
{
#ifdef CAREFUL_MODE
    muvdalist_check_loc(l, n);
#endif
    if (l->list[n]->marked) {
	l->list[n]->marked = 0;
	l->num_marked--;
    }
}

inline int muvdalist_init_pos_gn (muvdalist l)
{
    if (l->size > 0) {
	l->position = -1;
	return(1);
    }
    return(0) ;   
}

inline int muvdalist_get_pos (muvdalist l)
{
    return (l->position);
}

inline void muvdalist_set_pos (muvdalist l, int n)
{
#ifdef CAREFUL_MODE
    muvdalist_check_loc(l, n);
#endif
    l->position = n;
}

inline int muvdalist_get_next_marked_element_position (muvdalist l)
{
    l->position++;
    while (l->position < l->size) {
	if (l->list[l->position]->marked == 1) {
	    return(l->position);
	}
	l->position++;
    }
    return (-1);
}

inline int muvdalist_get_next_unmarked_element_position (muvdalist l)
{
    l->position++;
    while (l->position < l->size) {
	if (l->list[l->position]->marked == 0) {
	    return(l->position);
	}
	l->position++;
    }
    return (-1);
}

inline void muvdalist_check_loc(muvdalist l, int n)
{
    if ((n >= l->size) || (n < 0)) {
	fprintf(stderr, "muvdalist_error: looking for an element number (%d) that is not in this list (max %d) [fatal].\n", n, l->size - 1);
	exit(-1);
    }
}

void * muvdalist_get_unmarked(muvdalist l)
{
    int i = 0;
    while (i < l->size) {
	if (l->list[i]->marked == 0) {
	    return (l->list[i]->el);
	}
	i++;
    }
    return (NULL);
}
