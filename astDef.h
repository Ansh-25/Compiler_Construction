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

typedef enum {UNARY_PLUS, UNARY_MINUS, ID, NUM, RNUM, ARR_INDEX1, ARR_INDEX2, PLUS, MINUS, MUL, DIV, AND, OR, LT, LE, GT, GE, EQ, NE} Label;

/*typedef struct ASTList {
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

typedef struct arr_node{
    ParseNode* arr_name;
    ParseNode* arr_index;
}arr_node;

typedef union attributes{
    declstmt ds;
    asgnstmt as;
    iostmt is;
    module_node m;
    arr_node a_node;
}attr;*/

typedef struct ASTNode{
    Label label;
    struct token* tk;
    struct ASTNode* child;
    struct ASTNode* sibling;
}ASTNode;


#endif