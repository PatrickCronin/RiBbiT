/* ulvpclist.h */
/* copyright (c) Patrick Cronin 26 December 2004 */

/* This is an unordered looping list intended for use with any
** single type of object.  The pc is a simple period counter
** variable. An empty list consists of a pointer to an empty element
** (for exported function's speed and simplicity), a size counter,
** and function pointers relating to your object: free, display,
** copy, compare, and compare_id. (You must specify the element
** functions you want to use in the constructor.)
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
** mylist=ulvpclist_constructor(NULL,NULL,NULL,NULL,NULL);
**
** If the object you are putting in the list has associated functions
** with it for freeing, displaying, copying, comparing, or comparing
** its ids, you can change the line above to:
**
** mylist=ulvpclist_constructor(el_free, el_disp, el_copy, el_comp, el_comp_id);
**
** You can also pick and choose what functions you like:
**
** mylist=ulvpclist_constructor(el_free, el_disp, el_comp, NULL, NULL);
**
** If you do assign any to NULL, then be careful what ulvlist_ functions
** you call, as some do attempt to use the element functions (whether you
** have provided them or not).
**
** When you are done using the list, you should free it:
**
** ulvpclist_free_shallow(mylist);
** 
** or
**
** ulvpclist_free_deep(mylist);
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
**
** 26 December 2004: Added the period counter for use with class-day stc
** lists. The list still counts the number of elements in the list, and
** now it helps by counting the total number of periods from each of those
** elements.
**
** 25 January 2004: Initial implementation based on uvlist3.h.  Simply
** added accessor functions to retrieve a pointer to the first element
** (an element pointer, not the element->data pointer), the next element
** pointer (based on an existing element pointer--see list scanning
** functions in ulvlist.c for more information), and the data existing
** at an element pointer.
*/

#ifndef _UNORDERED_LOOPING_VOID_LIST_PC
#define _UNORDERED_LOOPING_VOID_LIST_PC

struct _ulvpc_element {
    void * data;
    void * next;
};
typedef struct _ulvpc_element ulvpc_element;

struct _unordered_looping_void_pc_list {
  element * head;
  int size;
  int pc;
  element * position;
  void (*el_free)();
  void (*el_display)();
  int (*el_compare)();
  void* (*el_copy)();
  int (*el_compare_id)();
};
typedef struct _unordered_looping_void_pc_list unordered_looping_void_pc_list;
typedef struct _unordered_looping_void_pc_list* ulvpclist;

/* Memory allocating/freeing functions */
ulvpclist ulvpclist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void ulvpclist_free_shallow (ulvpclist);
void ulvpclist_insert_beginning (ulvpclist, void *);
void ulvpclist_insert_end (ulvpclist, void *);
ulvpclist ulvpclist_copy_shallow (ulvpclist);
void ulvpclist_remove_element_shallow (ulvpclist, void *);
void ulvpclist_remove_element_by_ptr_shallow(ulvpclist, void *);
void* ulvpclist_pop_first (ulvpclist);

/* Deep (element memory-responsible) functions */
void ulvpclist_free_deep (ulvpclist);
ulvlist ulvpclist_copy_deep (ulvpclist);
void ulvpclist_remove_element_deep (ulvpclist, void *);

/* Non-memory-changing functions */
void ulvpclist_init (ulvpclist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void ulvpclist_display (ulvpclist);
int ulvpclist_isempty (ulvpclist);
int ulvpclist_size(ulvpclist);
int ulvpclist_ismember (ulvpclist, void *);
void* ulvpclist_find (ulvpclist, void *);
void* ulvpclist_find_id (ulvpclist, int);

/* List scanning functions (Also non-memory-changing) */
void* ulvpclist_ptr_to_first_element (ulvpclist);
void* ulvpclist_data_at_ptr (void*);
void* ulvpclist_ptr_to_next_element (ulvpclist, void*);

/* More list scanning function (Also non-memory-changing) */
int ulvpclist_init_pos (ulvpclist);
int ulvpclist_move_pos (ulvpclist);
void* ulvpclist_data_at_pos (ulvpclist);
#endif
