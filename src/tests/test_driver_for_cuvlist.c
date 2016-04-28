#include <stdio.h>
#include <stdlib.h>
#include "test_data_type.h"
#include "cuvlist.h"

#DEFINE LOOP_MAX 1000

int main (void)
{
    t_id_name badge1, badge2, badge3, badge4, badge5;
    cuvlist l1, l2;
    void * myptr;
    int i;

    badge1 = id_name_init();
    badge2 = id_name_init();
    badge3 = id_name_init();
    badge4 = id_name_init();
    
    id_name_setid(badge1, 1);
    id_name_setname(badge1, "Patrick");
    id_name_setid(badge2, 2);
    id_name_setname(badge2, "Michael");
    id_name_setid(badge3, 3);
    id_name_setname(badge3, "Hoffman");
    id_name_setid(badge4, 4);
    id_name_setname(badge4, "Cronin");

    printf("The badges are:\n");
    id_name_display(badge1);
    id_name_display(badge2);
    id_name_display(badge3);
    id_name_display(badge4);

    printf("Putting badges into a list.\n");
    /* Tests for:
    ** cuvlist_constructor
    ** cuvlist_init
    */
    l1 = cuvlist_constructor(id_name_free, id_name_display, id_name_compare, id_name_copy, NULL);
    /* Tests for:
    ** cuvlist_insert_beginning
    */
    cuvlist_insert_beginning(l1, badge4);
    cuvlist_insert_beginning(l1, badge3);
    cuvlist_insert_beginning(l1, badge2);
    cuvlist_insert_beginning(l1, badge1);

    /* Tests for:
    ** cuvlist_ptr_to_first_element
    ** cuvlist_ptr_to_next_element
    ** cuvlist_data_at_ptr
    */
    printf("Displaying the badges of list 1 my own way.\n");
    myptr = cuvlist_ptr_to_first_element(l1);
    if (myptr != NULL) {
	for(i=0;i<LOOP_MAX;i++) {
	    printf("%d: ", i);
	    id_name_display(cuvlist_data_at_ptr(myptr));
	    myptr = cuvlist_ptr_to_next_element(myptr);
	}
    } else {
	printf("List 1 is empty!\n");
    }

    /* Tests for:
    ** cuvlist_display
    */
    printf("Displaying the list.\n");
    cuvlist_display(l1);

    /* Tests for: 
    ** cuvlist_isempty
    */
    if (cuvlist_isempty(l1)) {
	printf("The list is empty.\n");
    } else {
	printf("The list has at least one element.\n");
    }

    /* Tests for:
    ** cuvlist_copy_deep
    */
    printf("Making a deep copy of the list.\n");
    l2 = cuvlist_copy_deep(l1);
    
    printf("Are they the same?\n");
    printf("List 1:\n");
    cuvlist_display(l1);
    printf("List 2:\n");
    cuvlist_display(l2);

    /* Tests for:
    ** cuvlist_ismember
    */
    printf("Is badge 2 in list 1?\n");
    if (cuvlist_ismember(l1,badge2) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    /* Tests for:
    ** cuvlist_remove_element_shallow
    */
    printf("Pulling badge 1 out of list 1\n");
    cuvlist_remove_element_shallow(l1, badge1);

    /* Tests for:
    ** cuvlist_ismember
    */
    printf("Now, is badge 1 in list 1?\n");
    if (cuvlist_ismember(l1,badge1) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    /* Tests for:
    ** cuvlist_pop_first
    */
    printf("Popping first element from list 1\n");
    badge4 = (t_id_name) cuvlist_pop_first(l1);
    id_name_display(badge4);
    printf("List 1's contents:\n");
    cuvlist_display(l1);
    printf("Is the popped element still in the list?\n");
    if (cuvlist_ismember(l1,badge4) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    /* Tests for:
    ** cuvlist_free_shallow
    ** cuvlist_free_deep
    */
    printf("Freeing memory.\n");
    cuvlist_free_shallow(l1);
    cuvlist_free_deep(l2);
    id_name_free(badge1);
    id_name_free(badge2);
    id_name_free(badge3);

    return 0;
}
