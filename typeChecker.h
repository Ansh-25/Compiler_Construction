
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
