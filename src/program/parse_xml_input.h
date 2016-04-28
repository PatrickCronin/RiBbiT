/* parse_xml_input.h
 ** Copyright 21 March 2004 by Patrick Cronin
** Parse the input file, and fill the internal program structures
*/

#ifndef PARSE_XML_INPUT
#define PARSE_XML_INPUT

#include <stdio.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "../data_types/misc_structures/data.h"
#include "../data_types/school_structures/meetings.h"

void parse_input_file(program_data);
void parseModules(xmlDocPtr, xmlNodePtr, program_data);
void parseWorkdays(xmlDocPtr, xmlNodePtr, program_data);
void parsePeriods(xmlDocPtr, xmlNodePtr, program_data);
void parseSubjects(xmlDocPtr, xmlNodePtr, program_data);
void parseClasses(xmlDocPtr, xmlNodePtr, program_data);
void parseTeachers(xmlDocPtr, xmlNodePtr, program_data);
char ** parseTeachersTokenizeList(char *, const char);
void parseRooms(xmlDocPtr, xmlNodePtr, program_data);
void parseClassMeetings(xmlDocPtr, xmlNodePtr, program_data);
void parseModuleAttributes_General(xmlNodePtr, int, program_data, int);
void parseModuleAttributes_ClassMeetings(meetings, xmlNodePtr, program_data);

#endif
