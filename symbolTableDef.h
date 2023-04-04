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
    DataType t;
    char* identifier;
    struct ParamList* next;
}ParamList;

typedef struct{
    char* identifier;
    DataType t;
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

bool compare_Datatype(Datatype d1, Datatype d2){
    if(d1.is_primitive!=d2.is_primitive)
        return false;
    if(d1.type!=d2.type)
        return false;
    return true;
}
#endif