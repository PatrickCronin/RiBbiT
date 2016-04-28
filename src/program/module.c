/* module.c
** Code to find, load, and run modules
** Copyright (C) 27 March 2004 by Patrick Cronin
*/


#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "../data_types/list_structures/ulvpclist.h"
#include "../data_types/list_structures/sandndlist.h"
#include "../data_types/misc_structures/bigthing.h"
#include "memory.h"
#include "module.h"

module module_constructor(char * modname)
{
    module m = (module) check_malloc (sizeof(module_struct));
    module_init (m, modname);
    return(m);
}

void module_init(module m, char * modname)
{
    m->name = NULL;
    m->attribute = NULL;
    m->type = -1;

    m->name = (char *) check_malloc (sizeof(char) * (strlen(modname) + 1));
    strncpy(m->name, modname, strlen(modname) + 1);

    m->handle = dlopen(modname, RTLD_NOW);
    if (!m->handle) {
	fprintf(stderr, "Could not open module %s: %s\n", modname, dlerror());
	exit(-1);
    }

    m->mod_init = dlsym(m->handle, "mod_initialize");
    m->mod_reg_id = dlsym(m->handle, "mod_register_id");
    m->mod_pregen = dlsym(m->handle, "mod_pre_generation");
    m->mod_check_stc = dlsym(m->handle, "mod_check_stc");
    m->mod_check_stc_day = dlsym(m->handle, "mod_check_stc_day");
    m->mod_finish = dlsym(m->handle, "mod_finish");
    
    if (m->mod_check_stc == NULL) {
	fprintf(stderr, "Error loading module %s: no mod_check_stc function found!\n", m->name);
	exit(-1);
    }

    /* if the module has a mod_init function, then let's call it */
    if (m->mod_init != NULL) {
	m->mod_init((void *)m);
    }

    if (m->type < 0) {
	fprintf(stderr, "Module %s did not declare its type!\n", modname);
	exit(-1);
    } else if ((m->type > 1) && (m->attribute == NULL)) {
	fprintf(stderr, "Module %s did not declare its attribute string!\n", modname);
	exit(-1);
    }
}

char * module_get_name(module m)
{
    return (m->name);
}

int module_compare_names(module m1, module m2)
{
    return (strcmp(module_get_name(m1),module_get_name(m2)) == 0);
}

void module_display(module m)
{
    printf("module: %s\n", module_get_name(m));
}

void module_free(module m)
{
    dlclose(m->handle);
    free(m->name);
    free(m->attribute);
    free(m);
}

char * module_get_attribute(module m)
{
    return (m->attribute);
}

int module_get_type(module m)
{
    return (m->type);
}

void module_run_pregens(bigthing b) {
    int i, listsize;
    module m;

    listsize = sandndlist_size(b->pd->modules);
    for(i=0; i<listsize; i++) {
	m = sandndlist_find_element_by_id(b->pd->modules, i);
	if (m->mod_pregen != NULL) m->mod_pregen(b);
    }
    
}
