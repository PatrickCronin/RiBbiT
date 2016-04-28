/* parse_xml_input.c
** Copyright 21 March 2004 by Patrick Cronin
** Parse the input file, and fill the internal program structures
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlerror.h>
#include <libxml/tree.h>
#include "parse_xml_input.h"
#include "memory.h"
#include "module.h"
#include "../data_types/misc_structures/data.h"
#include "../data_types/school_structures/subject.h"
#include "../data_types/school_structures/teacher.h"
#include "../data_types/school_structures/class.h"
#include "../data_types/school_structures/room.h"
#include "../data_types/school_structures/meetings.h"
#include "../data_types/school_structures/day.h"
#include "../data_types/list_structures/ulvlist.h"

//#define DEBUG

void parse_input_file(program_data d)
{
    xmlDocPtr config;
    xmlNodePtr cur;

    xmlLineNumbersDefault(1);

    config = xmlParseFile(d->configfile);
    if (config == NULL) {
	fprintf(stderr,"Configuration file error! [fatal]\n");
	exit(-1);
    }
    cur = xmlDocGetRootElement(config);
    if (cur == NULL) {
	fprintf(stderr,"Your configuration file was empty! [fatal]\n");
	xmlFreeDoc(config);
	exit(-1);
    }
    if (xmlStrcmp(cur->name, (const xmlChar *) "school")) {
	fprintf(stderr,"Configuration file error!\nProvided configuration file was of the wrong type! The root node must be \"school\"\n");
	xmlFreeDoc(config);
	exit(-1);
    }

    /* Go into a parsing loop for the different structures in the input file */
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((xmlStrcmp(cur->name, (const xmlChar *) "modules")) == 0) {
	    parseModules(config, cur, d);
	}
	else if ((xmlStrcmp(cur->name, (const xmlChar *) "workdays")) == 0) {
	    parseWorkdays(config, cur, d);
	}
	else if ((xmlStrcmp(cur->name, (const xmlChar *) "periods")) == 0) {
	    parsePeriods(config, cur, d);
	}
	else if ((xmlStrcmp(cur->name, (const xmlChar *) "subjects")) == 0) {
	    parseSubjects(config, cur, d);
	}
	else if ((xmlStrcmp(cur->name, (const xmlChar *) "classes")) == 0) {
	    parseClasses(config, cur, d);
	}
	else if ((xmlStrcmp(cur->name, (const xmlChar *) "teachers")) == 0) {
	    parseTeachers(config, cur, d);
	}
	else if ((xmlStrcmp(cur->name, (const xmlChar *) "rooms")) == 0) {
	    parseRooms(config, cur, d);
	}
	else if ((xmlStrcmp(cur->name, (const xmlChar *) "class-meetings")) == 0){
	    parseClassMeetings(config, cur, d);
	}
/*	else {
	    fprintf(stderr,"Your configuration file contains something that I don't know about: `%s'\n", cur->name);
	    exit(-1);
	}
*/
	cur = cur->next;
    }
    xmlFreeDoc(config);
}

void parseModules(xmlDocPtr doc, xmlNodePtr cur, program_data d)
{
    xmlChar * module_name;
    module m;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((xmlStrcmp(cur->name, (const xmlChar *) "module")) == 0) {
	    module_name = xmlGetProp(cur, "name");
	    if (module_name != NULL) {
		m = module_constructor(module_name);
#ifdef DEBUG
		printf("Inserted module: %s\n", module_get_name(m));
#endif
		sandndlist_insert(d->modules, m);
		xmlFree(module_name);
		m = NULL;
	    } else {
		fprintf(stderr,"Config error (line %ld): every <module> must have a `name=\"myname\"' defined.\n", xmlGetLineNo(cur));
		exit(-1);
	    }
	}
	cur = cur->next;
    }

}

void parseWorkdays(xmlDocPtr doc, xmlNodePtr cur, program_data pd)
{
    xmlChar * workdays;
    day d;
    char day_name[9];
    int inserted_id;
    int num_days_defined = 0;
    
    workdays = xmlGetProp(cur, "days-in-cycle");
    if (workdays != NULL) {
	NUM_DAYS = atoi(workdays);
#ifdef DEBUG
	printf("Set NUM_DAYS to %d\n", NUM_DAYS);
#endif
	xmlFree(workdays);
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
	    if ((xmlStrcmp(cur->name, (const xmlChar *) "workday")) == 0) {
		workdays = xmlGetProp(cur, "name");
		if (workdays != NULL) {
		    
		    num_days_defined++;
		    
		    d = day_constructor();
		    day_set_name(d, workdays);
#ifdef DEBUG
		    printf("Inserted day: %s\n", day_get_name(d));
#endif
		    inserted_id = sandndlist_insert(pd->days, d);
		    xmlFree(workdays);
		} else {
		    fprintf(stderr,"Config error (line %ld): every <workday> tag must have a `name=\"myname\"' defined.\n", xmlGetLineNo(cur));	  
		    exit(-1);
		}
	    }
	    cur = cur->next;
	}
    } else {
	fprintf(stderr,"Config error (line %ld): every <workday> tag must have a `days-in-cycle=\"number\"' defined.\n", xmlGetLineNo(cur));
	exit(-1);
    }
    
    /* For each day that wasn't named, give it a name */
    for( ; num_days_defined < NUM_DAYS; num_days_defined++) {
	d = day_constructor();
	sprintf(day_name, "Day %d", num_days_defined + 1);
	printf("Made day name %s\n", day_name);
	day_set_name(d, day_name);
#ifdef DEBUG
	printf("Inserted day: %s\n", day_get_name(d));
#endif
	inserted_id = sandndlist_insert(pd->days, d);
    }
}

void parsePeriods(xmlDocPtr doc, xmlNodePtr cur, program_data d)
{
    xmlChar * periods;
    periods = xmlGetProp(cur, "periods-in-day");
    if (periods != NULL) {
	NUM_PERIODS = atoi(periods);
#ifdef DEBUG
	printf("Set NUM_PERIODS to %d\n", NUM_PERIODS);
#endif
	xmlFree(periods);
    }
}

void parseSubjects(xmlDocPtr doc, xmlNodePtr cur, program_data d)
{
    xmlChar * subject_name;
    subject s;
    int inserted_id;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((xmlStrcmp(cur->name, (const xmlChar *) "subject")) == 0) {
	    subject_name = xmlGetProp(cur, "name");
	    if (subject_name != NULL) {
		s = subject_constructor();
		subject_set_name(s, subject_name);
#ifdef DEBUG
		printf("Inserted subject: %s\n", subject_get_name(s));
#endif
		inserted_id = sandndlist_insert(d->subjects, s);
		xmlFree(subject_name);

		parseModuleAttributes_General(cur, inserted_id, d, MOD_FOR_SUBJECTS);
	    } else {
		fprintf(stderr,"Config error (line %ld): every <subject> tag must have a `name=\"myname\"' defined.\n", xmlGetLineNo(cur));
		exit(-1);
	    }
	}
	
	cur = cur->next;
    }
}

void parseClasses(xmlDocPtr doc, xmlNodePtr cur, program_data d)
{
    xmlChar * class_name;
    class c;
    int inserted_id;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((xmlStrcmp(cur->name, (const xmlChar *) "class")) == 0) {
	    class_name = xmlGetProp(cur, "name");
	    if (class_name != NULL) {
		c = class_constructor();
		class_set_name(c, class_name);
#ifdef DEBUG
		printf("Inserted class: %s\n", class_get_name(c));
#endif
		inserted_id = sandndlist_insert(d->classes, c);
		xmlFree(class_name);

		parseModuleAttributes_General(cur, inserted_id, d, MOD_FOR_CLASSES);
	    } else {
		fprintf(stderr,"Config error (line %ld): every <class> must have a `name=\"myname\"' defined.\n", xmlGetLineNo(cur));
		exit(-1);
	    }
	}
	cur = cur->next;
    }
}

void parseTeachers(xmlDocPtr doc, xmlNodePtr cur, program_data d)
{
    xmlChar * teacher_name;
    teacher t;
    int inserted_id;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((xmlStrcmp(cur->name, (const xmlChar *) "teacher")) == 0) {
	    teacher_name = xmlGetProp(cur, "name");
	    if (teacher_name != NULL) {
		t = teacher_constructor();
		teacher_set_name(t, teacher_name);
#ifdef DEBUG
		printf("Inserted teacher: %s\n", teacher_get_name(t));
#endif
		inserted_id = sandndlist_insert(d->teachers, t);
		xmlFree(teacher_name);

		parseModuleAttributes_General(cur, inserted_id, d, MOD_FOR_TEACHERS);
	    } else {
		fprintf(stderr,"Config error (line %ld): every <teacher> must have a `name=\"myname\"' defined.\n", xmlGetLineNo(cur));
		exit(-1);
	    }
	}
	cur = cur->next;
    }
}

void parseRooms(xmlDocPtr doc, xmlNodePtr cur, program_data d)
{
    xmlChar * room_name;
    room r;
    int inserted_id;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
	if ((xmlStrcmp(cur->name, (const xmlChar *) "room")) == 0) {
	    room_name = xmlGetProp(cur, "name");
	    if (room_name != NULL) {
		r = room_constructor();
		room_set_name(r, room_name);
#ifdef DEBUG
		printf("Inserted room: %s\n", room_get_name(r));
#endif
		inserted_id = sandndlist_insert(d->rooms, r);
		xmlFree(room_name);

		parseModuleAttributes_General(cur, inserted_id, d, MOD_FOR_ROOMS);
	    } else {
		fprintf(stderr,"Config error (line %ld): every <room> must have a `name=\"myname\"' defined.\n", xmlGetLineNo(cur));
		exit(-1);
	    }
	}
	cur = cur->next;
    }
}

void parseClassMeetings(xmlDocPtr doc, xmlNodePtr cur, program_data d)
{
    xmlChar * class_name;
    xmlChar * subject_name;
    char * teacher_name;
    char ** teachers_list;
    xmlChar * room_name;
    xmlChar * hours_per_week;
    char * periods_arrangement;
    char ** periods_arrangement_list;
    int class_id;
    int subject_id;
    int teacher_id;
    int room_id;
    int i;
    meetings m;

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {

	/* initialize variables */
	class_name = NULL;
	subject_name = NULL;
	teacher_name = NULL;
	teachers_list = NULL;
	room_name = NULL;
	hours_per_week = NULL;
	periods_arrangement = NULL;
	periods_arrangement_list = NULL;
	class_id = -1;
	subject_id = -1;
	teacher_id = -1;
	room_id = -1;
	m = NULL;

	if ((xmlStrcmp(cur->name, (const xmlChar *) "meetings")) == 0) {
	    class_name = xmlGetProp(cur, "class");
	    subject_name = xmlGetProp(cur, "subject");
	    teacher_name = (char *) xmlGetProp(cur, "teacher");
	    room_name = xmlGetProp(cur, "room");
	    hours_per_week = xmlGetProp(cur, "hours-per-week");
	    periods_arrangement = (char *) xmlGetProp(cur, "periods-arrangement");

	    /* Make sure that the required elements are included */
	    if (class_name == NULL) {
		fprintf(stderr,"Config error (line %ld): every <meetings> must have a `class=\"myclas\"' defined [fatal].\n", xmlGetLineNo(cur));
		exit(-1);
	    } else if (subject_name == NULL) {
		fprintf(stderr,"Config error (line %ld): every <meetings> must have a `subject=\"mysubject\"' defined [fatal].\n", xmlGetLineNo(cur));
		exit(-1);
	    } else if (teacher_name == NULL) {
		fprintf(stderr,"Config error (line %ld): every <meetings> must have a `teacher=\"myteacher\"' defined [fatal].\n", xmlGetLineNo(cur));
		exit(-1);
	    } else if (hours_per_week == NULL) {
		fprintf(stderr,"Config error (line %ld): every <meetings> must have a 'hours-per-week=\"number\"' defined [fatal].\n", xmlGetLineNo(cur));
		exit(-1);
	    }

	    /* Look up the names of the subject, teacher, and class to make
	       sure that there is not a typo in the input file */
	    subject_id = sandndlist_find_id_by_name(d->subjects, subject_name);
	    if (subject_id == -1) {
		fprintf(stderr,"Config error (line %ld): Unknown subject `%s' [fatal].\n", xmlGetLineNo(cur), subject_name);
		exit(-1);
	    }
	    class_id = sandndlist_find_id_by_name(d->classes, class_name);
	    if (class_id == -1) {
		fprintf(stderr,"Config error (line %ld): Unknown class `%s' [fatal].\n", xmlGetLineNo(cur), class_name);
		exit(-1);
	    }

	    /* If the room is specified, then we can get its id */
	    if (room_name != NULL) {
		room_id = sandndlist_find_id_by_name(d->rooms, room_name);
		if (room_id == -1) {
		    fprintf(stderr,"Config error (line %ld): Unknown room `%s' [fatal].\n", xmlGetLineNo(cur), room_name);
		    exit(-1);
		}
	    } else {
		room_id = -1;
	    }
	    
	    /* Put things in a meetings struct */
	    m = meetings_constructor();
	    meetings_set_all(m, class_id, subject_id, room_id, atoi(hours_per_week));

	    /* teachers are special, because there can be more than one! */
	    teachers_list = parseTeachersTokenizeList(teacher_name, ',');
	    i = 0;
	    while (teachers_list[i] != NULL) {
		teacher_name = teachers_list[i];
		teacher_id = sandndlist_find_id_by_name(d->teachers, teacher_name);
		if (teacher_id == -1) {
		    fprintf(stderr,"Config error (line %ld): Unknown teacher `%s' [fatal].\n", xmlGetLineNo(cur), teacher_name);
		    fprintf(stderr,"(If you are trying to write multiple teachers, do not use any space between their names and the commas between them.)\n");
		    exit(-1);
		}
		
		meetings_add_teacher(m, teacher_id);
		i++;
	    }
	    free(teachers_list);

	    /* periods_arrangement is also special, because it can be many */
	    if (periods_arrangement != NULL) {
		periods_arrangement_list = parseTeachersTokenizeList(periods_arrangement, '+');
		i = 0;
		while (periods_arrangement_list[i] != NULL) {
		    periods_arrangement = periods_arrangement_list[i];
		    meetings_add_period_arrangement(m, atoi(periods_arrangement));
		    i++;
		}
		free(periods_arrangement_list);
	    } else {
		/* implement the default of 1 meeting per day */
		for (i = 0; i < atoi(hours_per_week); i++) {
		    meetings_add_period_arrangement(m, 1);
		}
	    }

	    /* check for any module's attributes */
	    /* note that m in this function is a meeting, not a module! */
	    parseModuleAttributes_ClassMeetings(m, cur, d);

	    /* Put the newly created meeting into our list */
	    ulvlist_insert_end(d->meetingses, m);
#ifdef DEBUG
	    meetings_display(m);
#endif

	    /* Free the xmlChars if they were allocated */
	    if (class_name != NULL) {
		xmlFree(class_name);
	    }
	    if (subject_name != NULL) {
		xmlFree(subject_name);
	    }
	    if (room_name != NULL) {
		xmlFree(room_name);
	    }
	    if (hours_per_week != NULL) {
		xmlFree(hours_per_week);
	    }
	}
	cur = cur->next;
    }
}

char ** parseTeachersTokenizeList(char *teacher_name, const char delim)
{
    char ** name_vector;
    int nv_size = 0;

    if (teacher_name) {
	    name_vector = (char **) check_malloc (sizeof (char *) * (nv_size + 1));
	    nv_size++;
	    name_vector[0] = teacher_name;
	    while (*teacher_name != '\0') {
		if (*teacher_name == delim) {
		    name_vector = (char **) check_realloc (name_vector, sizeof (char *) * (nv_size + 1));
		    name_vector[nv_size] = teacher_name + 1;
		    nv_size++;
		    *teacher_name = '\0';
		}
		teacher_name++;
	    }
	    name_vector = (char **) check_realloc (name_vector, sizeof (char *) * (nv_size + 1));
	    name_vector[nv_size] = NULL;
	    nv_size++;
	    return(name_vector);
    }
    return (NULL);
}


void parseModuleAttributes_General(xmlNodePtr cur, int inserted_id, program_data d, int mod_type)
{
    int i;
    module m;
    xmlChar * attribute_value;

    /* search for other possible attribute values from
    ** our module list and register them in the responsible
    ** module if necessary */
    i = 0;
    while (i < sandndlist_size(d->modules)) {
	m = sandndlist_find_element_by_id(d->modules, i);
	if (module_get_type(m) & mod_type) {
	    attribute_value = xmlGetProp(cur, (xmlChar *) module_get_attribute(m));
	    if (attribute_value) {
		m->mod_reg_id(mod_type, inserted_id, (char *) attribute_value);
		xmlFree(attribute_value);
#ifdef DEBUG
		printf("Registered something %d for attributes %s in some tags\n", inserted_id, module_get_attribute(m));
#endif
	    }
	}
	i++;
    }
}

void parseModuleAttributes_ClassMeetings(meetings meet, xmlNodePtr cur, program_data d)
{
   int i;
   module mod;
   xmlChar * attribute_value;
   
   /* search for other possible attribute values from
   ** our module list and register them in the responsible
   ** module if necessary */

   i = 0;
   while (i < sandndlist_size(d->modules)) {
       mod = sandndlist_find_element_by_id(d->modules, i);
       if (module_get_type(mod) & MOD_FOR_MEETINGS) {
	   attribute_value = xmlGetProp(cur, (xmlChar *) module_get_attribute(mod));
	   if (attribute_value) {
	       /* put the module's ptr and value in the meetings's smlist
	       ** for module_attributes */
	       meetings_insert_module(meet, mod, (char *) attribute_value);
	       xmlFree(attribute_value);
#ifdef DEBUG
	       printf("Registered class-meetings for attributes %s in meetings tags\n", module_get_attribute(mod));
#endif
	   }
       }
       i++;
    }
}
