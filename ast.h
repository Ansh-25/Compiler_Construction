/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#ifndef ast_H
#define ast_H

#include "astDef.h"
#include <stdlib.h>
#include <stdbool.h>

extern ASTNode* astroot;

void printAST(ASTNode* root);

int countASTNodes(ASTNode* root) ;

void makeAST(struct ParseNode* parserNode);

#endif