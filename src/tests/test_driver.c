#include <stdio.h>
#include <stdlib.h>
#include "test_data_type.h"
#include "cuvlist.h"

int main (void)
{
    t_id_name badge1, badge2, badge3, badge4;
    cuvlist l1, l2;

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
    l1 = cuvlist_constructor(id_name_free, id_name_display, id_name_compare, id_name_copy, NULL);
    cuvlist_insert_end(l1, badge3);
    cuvlist_insert_end(l1, badge2);
    cuvlist_insert_end(l1, badge1);

    printf("Displaying the list.\n");
    cuvlist_display(l1);

    if (cuvlist_isempty(l1)) {
	printf("The list is empty.\n");
    } else {
	printf("The list has at least one element.\n");
    }

    printf("Making a deep copy of the list.\n");
    l2 = cuvlist_copy_deep(l1);
    
    printf("Are they the same?\n");
    printf("List 1:\n");
    cuvlist_display(l1);
    printf("List 2:\n");
    cuvlist_display(l2);

    printf("Is badge 3 in list 2?\n");
    if (cuvlist_ismember(l2,badge3) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    printf("Pulling badge 3 out of list 2\n");
    cuvlist_remove_element_shallow(l2, badge3);

    
    printf("Now, is badge 3 in list 2?\n");
    if (cuvlist_ismember(l2,badge3) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    printf("Popping first element from list 2\n");
    badge4 = (t_id_name) cuvlist_pop_first(l2);
    id_name_display(badge4);
    printf("List 2's contents:\n");
    cuvlist_display(l2);
    printf("Is the popped element still in the list 2?\n");
    if (cuvlist_ismember(l2,badge4) == 0) {
	printf("it is not.\n");
    } else {
	printf("it is.\n");
    }

    printf("Freeing memory.\n");
    cuvlist_free_shallow(l1);
    cuvlist_free_deep(l2);
    id_name_free(badge1);
    id_name_free(badge2);
    id_name_free(badge3);

    return 0;
}
