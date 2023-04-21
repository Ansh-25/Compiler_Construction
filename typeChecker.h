/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "astDef.h"

extern char * arr[];
extern bool print_error;

void printSymbolTable();

void typeChecker(ASTNode *astNode);

void printAllArrays();

void calcActRecordSize();

#endif
