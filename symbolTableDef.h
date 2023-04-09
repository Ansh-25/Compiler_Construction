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
#include "hash.h"

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
    ModuleTableEntry* moduleTable;
}MainTableEntry;

void insertModule(MainTableEntry** table, MainTableEntry* new_module) {
    int ind = sumchars(new_module->module_name);
    while(table[ind] != NULL) {ind++; ind %=47;}
    table[ind] = new_module;
}

MainTableEntry* searchModule(MainTableEntry** table, char* moduleName) {
    int ind = sumchars(moduleName);
    while(table[ind]!=NULL){
        if(strcmp(moduleName, table[ind] -> module_name)==0) return table[ind];
        ind++;
        ind %= 47;
    }
    return NULL;
}

void insertVar(ModuleTableEntry** table, ModuleTableEntry* new_var) {
    int ind = sumchars(new_var->identifier);
    while(table[ind] != NULL) {ind++; ind %=47;}
    table[ind] = new_var;
}

ModuleTableEntry* searchVar(ModuleTableEntry** table, char* varName) {
    int ind = sumchars(varName);
    while(table[ind]!=NULL){
        if(strcmp(varName, table[ind] -> identifier)==0) return table[ind];
        ind++;
        ind %= 47;
    }
    return NULL;
}

ParamList* insertlast (ParamList* head, ParamList* newNode) {
    newNode -> next = NULL;
    if (head == NULL)
        head = newNode;
    else {
        ParamList* curr = head;
        while (curr -> next != NULL)
            curr = curr -> next;
        curr -> next = newNode;
    }
    return head;
}

bool compare_Datatype(TypeInfo d1, TypeInfo d2){
    if(d1.datatype!=d2.datatype)
        return false;
    if(d1.primtype!=d2.primtype)
        return false;
    return true;
}
#endif