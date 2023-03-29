/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#ifndef astDef_H
#define astDef_H

#include "parserDef.h"
#include <stdlib.h>

typedef enum {PROGRAM_NODE} label;

typedef struct ASTList {
    ASTNode* Node;
    ASTList* next;
}ASTList;

typedef struct programNode {
    moduleDeclarationsList* moduleDecList;
    ASTList* moduleList;
}programNode;

typedef struct otherModulesNode {
    ASTList* moduleList;
}otherModulesNode;

typedef struct moduleDeclarationsList {
    
}moduleDeclarationsList;

typedef struct iostmt{
    ASTNode* ID;
}iostmt;

typedef struct declstmt {
    int a,b;
} declstmt;

typedef struct itrstmt {
    ASTNode* ID;
    ASTNode* range;
    ASTNode* statements;
} itrstmt;

typedef struct asgnstmt {
    int a,b;
} asgnstmt;

typedef struct module_node{
    ASTNode* ID;
    ASTList* param_list;
}module_node;

typedef union attr{
    declstmt ds;
    asgnstmt as;
    iostmt is;
    module_node m;
}attr;

typedef struct ASTNode {
    type t;
    int ruleno; //reqd?
    label l; //value? label and type both not req'd
    attr attributes; //better keep child sibling
}ASTNode;

#endif