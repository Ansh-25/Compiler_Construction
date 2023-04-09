/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#ifndef symbolTableDef_H
#define symbolTableDef_H

#include "astDef.h"

typedef struct ParamList{
    TypeInfo type;
    char* identifier;
    struct ParamList* next;
}ParamList;

typedef struct{
    char* identifier;
    int nesting_lvl;
    int scope_begin;
    int scope_end;
    int width;
    int offset;
    TypeInfo type;
}ModuleTableEntry;

typedef struct{
    char* module_name;
    ParamList* inputList;
    ParamList* outputList;
    ModuleTableEntry** moduleTable;
}MainTableEntry;

#endif