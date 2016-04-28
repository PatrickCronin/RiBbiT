/* test_silist_driver.c
** Testing silist.c and silist.h
** 27 February 2004
** Copyright Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include "../data_types/list_structures/silist.h"

int main (void)
{
    silist mylist = silist_constructor();

    printf("Inserting elements...\n");
    silist_insert(mylist, 10);
    silist_insert(mylist, 20);
    silist_insert(mylist, 30);
    silist_insert(mylist, 40);
    
    printf("The size of the list is %d\n", silist_get_size(mylist));
    
    printf("Displaying the list...\n");
    silist_display(mylist);
    
    printf("Element Check: 0: %d\n", silist_get_element_n(mylist,0));
    printf("Element Check: 1: %d\n", silist_get_element_n(mylist,1));
    printf("Element Check: 2: %d\n", silist_get_element_n(mylist,2));
    printf("Element Check: 3: %d\n", silist_get_element_n(mylist,3));
    printf("Element Check: 4: %d\n", silist_get_element_n(mylist,4));
    
    printf("Freeing the list...\n");
    silist_free(mylist);
    
    printf("Exiting...\n");
    
    return(1);
}
