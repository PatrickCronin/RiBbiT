/* ulvlist.h */
/* copyright (c) Patrick Cronin 25 January 2004 */

/* This is an unordered looping list intended for use with any
** single type of object.  An empty list consists of a pointer to
** an empty element (for exported function's speed and simplicity),
** a size counter, and function pointers relating to your object:
** free, display, copy, compare, and compare_id. (You must specify
** the element functions you want to use in the constructor.)
**
** This non-circular list structure has three functions that allow it
** to be treated as a circular list.  This structure is optimized for
** the use of inserts, removes, and frees, rather than list scanning.
** If you will have more calls to (the functions listed as list scanning
** functions) rather than (inserts and removes and frees), than you
** would probably be better off using cuvlist.h and cuvlist.c.
**
** Any time you want to have a list, you must declare it: 
** ulvlist mylist;
** And if you want to use it, you must allocate the memory for it:
**
** mylist=ulvlist_constructor(NULL,NULL,NULL,NULL,NULL);
**
** If the object you are putting in the list has associated functions
** with it for freeing, displaying, copying, comparing, or comparing
** its ids, you can change the line above to:
**
** mylist=ulvlist_constructor(el_free, el_disp, el_copy, el_comp, el_comp_id);
**
** You can also pick and choose what functions you like:
**
** mylist=ulvlist_constructor(el_free, el_disp, el_comp, NULL, NULL);
**
** If you do assign any to NULL, then be careful what ulvlist_ functions
** you call, as some do attempt to use the element functions (whether you
** have provided them or not).
**
** When you are done using the list, you should free it:
**
** ulvlist_free_shallow(mylist);
** 
** or
**
** ulvlist_free_deep(mylist);
**
** The difference between the deep functions and the shallow functions
** is that the shallow functions will only alter the memory of the list
** that it is operating on.  The deep functions will change the memory
** of the elements pointed to by the list.  For example: the free_shallow
** function above will free only memory that was created by
** uvlist3_constructor, but the free_deep function will use the element
** freeing function given to it during the list's construction to free
** each element's memory, in addition to the list's memory itself.  For
** this reason, you should be careful of whether to use deep or shallow
** functions.
**
** PLACES FOR IMPROVEMENT:
** 1. Should check for element functions not being defined before using,
** or at least warn user at some time about what ulvlist_ functions depend
** on what user-specified el_ functions.
** 2. General object usage is not consistent between _unordered_looping_
** void_list and the _element structs.  (For the element, we use it
** directly, and for the unordered_looping_void_list we use it indirectly,
** via the pointer: *ulvlist).  Maybe this should be made consistent.
**
** CHANGELOG:
** 25 January 2004: Initial implementation based on uvlist3.h.  Simply
** added accessor functions to retrieve a pointer to the first element
** (an element pointer, not the element->data pointer), the next element
** pointer (based on an existing element pointer--see list scanning
** functions in ulvlist.c for more information), and the data existing
** at an element pointer.
*/

#ifndef _UNORDERED_LOOPING_VOID_LIST
#define _UNORDERED_LOOPING_VOID_LIST

struct _element {
    void * data;
    void * next;
};
typedef struct _element element;

struct _unordered_looping_void_list {
    element * head;
    int size;
    element * position;
    void (*el_free)();
    void (*el_display)();
    int (*el_compare)();
    void* (*el_copy)();
    int (*el_compare_id)();
};
typedef struct _unordered_looping_void_list unordered_looping_void_list;
typedef struct _unordered_looping_void_list* ulvlist;

/* Memory allocating/freeing functions */
ulvlist ulvlist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void ulvlist_free_shallow (ulvlist);
void ulvlist_insert_beginning (ulvlist, void *);
void ulvlist_insert_end (ulvlist, void *);
ulvlist ulvlist_copy_shallow (ulvlist);
void ulvlist_remove_element_shallow (ulvlist, void *);
void ulvlist_remove_element_by_ptr_shallow(ulvlist, void *);
void* ulvlist_pop_first (ulvlist);

/* Deep (element memory-responsible) functions */
void ulvlist_free_deep (ulvlist);
ulvlist ulvlist_copy_deep (ulvlist);
void ulvlist_remove_element_deep (ulvlist, void *);

/* Non-memory-changing functions */
void ulvlist_init (ulvlist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void ulvlist_display (ulvlist);
int ulvlist_isempty (ulvlist);
int ulvlist_size(ulvlist);
int ulvlist_ismember (ulvlist, void *);
void* ulvlist_find (ulvlist, void *);
void* ulvlist_find_id (ulvlist, int);

/* List scanning functions (Also non-memory-changing) */
void* ulvlist_ptr_to_first_element (ulvlist);
void* ulvlist_data_at_ptr (void*);
void* ulvlist_ptr_to_next_element (ulvlist, void*);

/* More list scanning function (Also non-memory-changing) */
int ulvlist_init_pos (ulvlist);
int ulvlist_move_pos (ulvlist);
void* ulvlist_data_at_pos (ulvlist);
#endif
