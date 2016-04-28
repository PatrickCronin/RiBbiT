/* sslist.c
** A simple list structure for strings.
** Best for small lists -- the list is stored in consecutive
** memory addresses.
** Copyright 27 February 2004 Patrick Cronin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../program/memory.h"
#include "sslist.h"

sslist sslist_constructor(void)
{
    sslist newsslist = (sslist) check_malloc (sizeof(sslist_struct));
    sslist_init (newsslist);
    return (newsslist);
}

void sslist_init(sslist s)
{
    s->size = 0;
    s->list = NULL;
}

int sslist_find(sslist s, char * str)
{
    int c=0;
    
    while (c < s->size) {
	if (s->list[c] == str) {
	    return(c);
	}
	c++;
    }
    /* not found */
    return(-1);
}

int sslist_get_size(sslist s)
{
    return (s->size);
}

char * sslist_get_element_n(sslist s, int n)
{
    if (n >= s->size) {
	fprintf(stderr, "sslist: a non-existant element was requested. (non-fatal)\n");
	return (NULL);
    }
    return (s->list[n]);
}

void sslist_insert(sslist s, char * str)
{
    s->list = (char **) check_realloc (s->list, sizeof(char*) * (s->size + 1));
    s->list[s->size] = (char *) check_malloc (sizeof(char) * (strlen(str) + 1));
    strncpy(s->list[s->size], str, strlen(str) + 1);
    s->size++;
}

void sslist_free(sslist s)
{
    int i;

    if (s->list != NULL) {
	for (i=0; i<s->size; i++) {
	    free(s->list[i]);
	}
	free (s->list);
    }
    free(s);
}

void sslist_display(sslist s)
{
    int c;
    printf("list size: %d\n", s->size);
    for(c=0; c<s->size; c++) {
	printf("element %d: %s\n", c, s->list[c]);
    }
}
