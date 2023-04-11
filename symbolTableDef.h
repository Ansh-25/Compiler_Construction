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
#include "ast.h"

typedef enum {INPUT_VAR, OUTPUT_VAR, FOR_LOOP_VAR, NORMAL_VAR} typeOfVar;

typedef struct WhileCondListNode WhileCondListNode;

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
    bool is_changed;
    typeOfVar vartype;
}ModuleTableEntry;

typedef struct{
    char* module_name;
    ParamList* inputList;
    ParamList* outputList;
    ModuleTableEntry** moduleTable;
    bool is_reused;
}MainTableEntry;

struct WhileCondListNode{
    ModuleTableEntry* var;
    bool isChangedBeforeWhile;
    struct WhileCondListNode* next;
};

#endif