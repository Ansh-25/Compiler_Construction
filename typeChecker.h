
#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "astDef.h"

extern char * arr[];

void printSymbolTable();

void typeChecker(ASTNode *astNode);

#endif
