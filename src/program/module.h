/* module.h
** Code to find, load, and run modules
** Copyright (C) 27 March 2004 by Patrick Cronin
*/


#ifndef MODULE
#define MODULE

#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/school_structures/stc.h"
#include "../data_types/list_structures/sandndlist.h"

#define MOD_GENERAL      1
#define MOD_FOR_SUBJECTS 2
#define MOD_FOR_CLASSES  4
#define MOD_FOR_TEACHERS 8
#define MOD_FOR_ROOMS    16
#define MOD_FOR_MEETINGS 32

struct _module {
    char * name;
    char * attribute;
    int type;
    void *handle;
    int (*mod_init)(void*); /* really should be a module pointer, not void*  */
    int (*mod_reg_id)(int type, int id, char * value);
    int (*mod_pregen)(bigthing);
    int (*mod_check_stc)(bigthing, stc);
    int (*mod_finish)(void);
};
typedef struct _module module_struct;
typedef struct _module* module;

module module_constructor(char *);
void module_init(module, char *);
char * module_get_name(module);
int module_compare_names(module, module);
void module_display(module);
void module_free(module);
char * module_get_attribute(module);
int module_get_type(module);
void module_run_pregens(bigthing);
#endif


