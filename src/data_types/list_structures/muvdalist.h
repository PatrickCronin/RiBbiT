/* muvdalist.h */
/* copyright (c) Patrick Cronin 17 April 2004 */

#ifndef _MARKED_UNORDERED_VOID_DIRECT_ACCESS_LIST
#define _MARKED_UNORDERED_VOID_DIRECT_ACCESS_LIST

/* Define a Markable Element */
struct _melement {
    void * el;
    char marked;
};
typedef struct _melement melement_struct;
typedef struct _melement* melement;

/* Define a MUVDAList */
struct _marked_unordered_void_direct_access_list {
    int size;
    int num_marked;
    melement * list;
    int position;
    void (*el_free)();
    void (*el_display)();
    int (*el_compare)();
    void* (*el_copy)();
    int (*el_compare_id)();
};
typedef struct _marked_unordered_void_direct_access_list muvdalist_struct;
typedef struct _marked_unordered_void_direct_access_list* muvdalist;

/* Memory allocating/freeing functions */
muvdalist muvdalist_constructor (void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void muvdalist_free_shallow (muvdalist);
void muvdalist_insert (muvdalist, void *);
muvdalist muvdalist_copy_shallow (muvdalist);
void muvdalist_remove_element_shallow (muvdalist, void *);
void muvdalist_remove_element_by_ptr_shallow(muvdalist, void *);
void* muvdalist_pop_first (muvdalist);

/* Deep (element memory-responsible) functions */
void muvdalist_free_deep (muvdalist);
muvdalist muvdalist_copy_deep (muvdalist);
void muvdalist_remove_element_deep (muvdalist, void *);

/* Non-memory-changing functions */
inline void muvdalist_init (muvdalist l, void (*el_free)(), void (*el_display)(), int (*el_compare)(), void* (*el_copy)(), int (*el_compare_id)());
void muvdalist_display (muvdalist);
inline int muvdalist_isempty (muvdalist);
inline int muvdalist_size(muvdalist);
int muvdalist_ismember (muvdalist, void *);
void* muvdalist_find (muvdalist, void *);
void* muvdalist_find_id (muvdalist, int);

/* List scanning functions (Also non-memory-changing) */
void* muvdalist_ptr_to_first_element (muvdalist);
void* muvdalist_data_at_ptr (void*);
void* muvdalist_ptr_to_next_element (muvdalist, void*);

/* More list scanning function (Also non-memory-changing) */
inline int muvdalist_init_pos (muvdalist);
inline int muvdalist_move_pos (muvdalist);
inline void* muvdalist_data_at_pos (muvdalist);

/* Other new functions, some involving marking */
inline int muvdalist_get_num_unmarked (muvdalist);
inline void* muvdalist_get_element_n (muvdalist, int);
inline int muvdalist_is_marked (muvdalist, int);
inline void muvdalist_mark_element (muvdalist, int);
inline void muvdalist_unmark_element (muvdalist, int);
inline int muvdalist_init_pos_gn (muvdalist);
inline void muvdalist_set_pos (muvdalist, int);
inline int muvdalist_get_pos (muvdalist);
inline int muvdalist_get_next_marked_element_position (muvdalist);
inline int muvdalist_get_next_unmarked_element_position (muvdalist);
inline void muvdalist_check_loc(muvdalist, int);

void * muvdalist_get_unmarked(muvdalist);

#endif
