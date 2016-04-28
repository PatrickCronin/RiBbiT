/* cuvlist.h */
/* Circular Unordered Void List Structure and Functions */
/* copyright (c) Patrick Cronin 05 February 2004 */

/* This is a circular unordered list intended for use with any
** object.  An empty list consists of a pointer to an empty element
** (for functions' speed and simplicity), a size counter, and
** function pointers relating to your object: free, display, copy,
** compare, and compare_id.
**
** This circular list structure is optimized for more calls to the list
** scanning functions than to any of the inserts, removes, or frees.
** If you will be doing more (inserts and removes and frees) than the 3
** functions listed as list scanning functions, you'd probably be better
** off using ulvlist.h and ulvlist.c.
**
** Any time you want to have a circular list around, you must declare it:
**
** cuvlist mylist;
**
** And if you want to use it, you must allocate the memory for it:
**
** mylist=cuvlist_constructor(NULL,NULL,NULL,NULL,NULL);
**
** If the object you are putting in the list has associated functions
** with it for freeing, displaying, copying, comparing, or comparing
** its ids, you can change the line above to:
**
** mylist=cuvlist_constructor(el_free, el_disp, el_copy, el_comp, el_comp_id);
**
** You can also pick and choose what functions you like:
**
** mylist=cuvlist_constructor(el_free, el_disp, el_comp, NULL, NULL);
**
** If you do assign any to NULL, then becareful what uvlist3_ functions
** you call, as some do attempt to use the element functions (whether you
** provide them or not).
**
** When you are done using the list, you should free it:
**
** cuvlist_free_shallow(mylist);
** 
** or
**
** cuvlist_free_deep(mylist);
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
**
** CHANGELOG:
**
** cuvlist.c cuvlist.h: added a pointer to the end of the list to make
** re-connecting the circle a snap.  Functions changed: cuvlist_init,
** cuvlist_free_shallow, cuvlist_free_deep, cuvlist_insert_beginning,
** cuvlist_insert_end, cuvlist_display, cuvlist_copy_shallow,
** cuvlist_copy_deep, cuvlist_find, cuvlist_remove_element_shallow,
** cuvlist_remove_element_deep, and cuvlist_pop_first. Also added list
** scanning functions to allow the user to go through the list, and look
** at the data contained in it. 05 February 2004.
**
** cuvlist.c cuvlist.h: changed most functions to make the list behave
** like a circle--instead of using a singular clever function to go to
** the top of the list if you're at the bottom, this circular list
** is fully maintained as a circle in every function that changes the
** circular list. All functions renamed according to the new naming
** scheme. In addition, the `element' type is now a pointer to an
** element, rather than an element itself.  All functions were updated
** to handle this.  (Just a change to keep object references of both
** lists and elements similar.)  Functions altered for circular list
** abilities are: cuvlist_free_shallow, cuvlist_free_deep,
** cuvlist_insert_beginning, cuvlist_insert_end, cuvlist_display,
** cuvlist_copy_shallow, cuvlist_copy_deep, cuvlist_find,
** cuvlist_find_id, cuvlist_remove_element_shallow,
** cuvlist_remove_element_deep, and cuvlist_pop_first. 02 February 2004 
**
** uvlist3.c uvlist3.h: optimized the copy, free, and remove functions
** by splitting the each function into two, so that at each specified list
** element looked at, we wouldn't have to decide if we are doing a shallow
** or deep operation. 25 Jan 2004
**
** uvlist2.c uvlist2.h: added more element functions, changed the programming
** interface for using the list, added more understanding of memory control
** from the exported uvlist2 functions. 19 Jan 2004
**
** uvlist.c uvlist.h: initial implementation. 15 Jan 2004
*/

#ifndef _CIRCULAR_UNORDERED_VOID_LIST
#define _CIRCULAR_UNORDERED_VOID_LIST

struct _cuv_element {
    void * data;
    void * next;
};
typedef struct _cuv_element cuv_element;
typedef struct _cuv_element* element;

struct _circular_unordered_void_list {
    element head;
    element foot;
    int size;
    void (*el_free)();
    void (*el_display)();
    int (*el_compare)();
    void* (*el_copy)();
    int (*el_compare_id)();
};
typedef struct _circular_unordered_void_list circular_unordered_void_list;
typedef struct _circular_unordered_void_list* cuvlist;

/* Memory allocation/free */
cuvlist cuvlist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void cuvlist_free_shallow (cuvlist);
void cuvlist_insert_beginning (cuvlist, void *);
void cuvlist_insert_end (cuvlist, void *);
cuvlist cuvlist_copy_shallow (cuvlist);
void cuvlist_remove_element_shallow (cuvlist, void *);
void* cuvlist_pop_first (cuvlist);

/* Deep (element memory-responsible functions) */
void cuvlist_free_deep (cuvlist);
cuvlist cuvlist_copy_deep (cuvlist);
void cuvlist_remove_element_deep (cuvlist, void *);

/* Non-memory creating */
void cuvlist_init (cuvlist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void cuvlist_display (cuvlist);
int cuvlist_isempty (cuvlist);
int cuvlist_size(cuvlist);
int cuvlist_ismember (cuvlist, void *);
void* cuvlist_find (cuvlist, void *);
void* cuvlist_find_id (cuvlist, int);

/* List scanning functions (Also non-memory-changing) */
void* cuvlist_ptr_to_first_element (cuvlist);
void* cuvlist_data_at_ptr (void*);
void* cuvlist_ptr_to_next_element (void*);
#endif
