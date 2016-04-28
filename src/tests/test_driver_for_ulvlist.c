#include <stdio.h>
#include <stdlib.h>
#include "test_data_type.h"
#include "ulvlist.h"

int main (void)
{
    t_id_name badge1, badge2, badge3, badge4;
    ulvlist l1, l2;
    void * myptr;
    int i;

    badge1 = id_name_init();
    badge2 = id_name_init();
    badge3 = id_name_init();
    
    id_name_setid(badge1, 3);
    id_name_setname(badge1, "Michael");
    id_name_setid(badge2, 6);
    id_name_setname(badge2, "Hoffman");
    id_name_setid(badge3, 9);
    id_name_setname(badge3, "Cronin");

    printf("The badges are:\n");
    id_name_display(badge1);
    id_name_display(badge2);
    id_name_display(badge3);

    printf("Putting badges into a list.\n");
    l1 = ulvlist_constructor(id_name_free, id_name_display, id_name_compare, id_name_copy, NULL);
    ulvlist_insert_beginning(l1, badge3);
    ulvlist_insert_beginning(l1, badge2);
    ulvlist_insert_beginning(l1, badge1);

    printf("Displaying the badges of list 1 my own way.\n");
    myptr = ulvlist_ptr_to_first_element (l1);
    for(i=0;i<200;i++) {
	printf("%d: ", i);
	id_name_display(ulvlist_data_at_ptr(myptr));
	myptr = ulvlist_ptr_to_next_element(l1,myptr);
    }

    printf("Displaying the list.\n");
    ulvlist_display(l1);

    if (ulvlist_isempty(l1)) {
	printf("The list is empty.\n");
    } else {
	printf("The list has at least one element.\n");
    }

    printf("Making a deep copy of the list.\n");
    l2 = ulvlist_copy_deep(l1);
    
    printf("Are they the same?\n");
    printf("List 1:\n");
    ulvlist_display(l1);
    printf("List 2:\n");
    ulvlist_display(l2);

    printf("Is badge 2 in list 1?\n");
    if (ulvlist_ismember(l1,badge2) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    printf("Pulling badge 1 out of list 1\n");
    ulvlist_remove_element_shallow(l1, badge1);

    
    printf("Now, is badge 1 in list 1?\n");
    if (ulvlist_ismember(l1,badge1) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    printf("Popping first element from list 1\n");
    badge4 = (t_id_name) ulvlist_pop_first(l1);
    id_name_display(badge4);
    printf("List 1's contents:\n");
    ulvlist_display(l1);
    printf("Is the popped element still in the list?\n");
    if (ulvlist_ismember(l1,badge4) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    printf("Freeing memory.\n");
    ulvlist_free_shallow(l1);
    ulvlist_free_deep(l2);
    id_name_free(badge1);
    id_name_free(badge2);
    id_name_free(badge3);

    return 0;
}
