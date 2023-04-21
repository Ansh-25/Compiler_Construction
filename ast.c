/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "symbolTableDef.h"
#include "typeChecker.h"

ASTNode* astroot;
int scope_start1 = 1;
int scope_start2 = 1;
int scope_end1 = 1e9;
int scope_end2 = 1e9;
int nest_level = 0;

// char* arr[] = {"PROGRAM","ITER_FOR","MODULEDECLARATIONS","OTHERMODULES1","OTHERMODULES2","UNARY_PLUS","UNARY_MINUS", "ID", "NUM", "RNUM", "ARRAY_DTYPE", "ARRAY","ARRAY_RANGE","ARR_INDEX1", "ARR_INDEX2", "PLUS", "MINUS", "MUL", "DIV", "AND", "OR", "LT", "LE", "GT", "GE", "EQ", "NE", "MODULEDECLARATION", "DRIVERMODULE","MODULE_REUSE", "MODULE", "RET", "PARAMETER", "INTEGER_", "REAL_", "BOOLEAN_", "RANGE_WHILE","RANGE_FOR", "STATEMENTS", "INPUT", "OUTPUT", "ARR_OUTPUT", "TRUE", "FALSE", "ASSIGN", "ARR_ASSIGN", "INDEX_ARR", "DECLARE", "ID_LIST", "CASE","CASE_STMT","RANGE", "INPUT_PLIST", "OUTPUT_PLIST","DEFAULT"};

int countASTNodes(ASTNode* root) {
    if(root==NULL) return 0;
    return 1+countASTNodes(root->child)+countASTNodes(root->sibling);
}

void printAST(ASTNode* root){
    if(root==NULL) return;
    printf("%-15s",arr[root->label]);
    if(root->tk!=NULL) {
        if(root->tk->type == TK_NUM){
            printf("Token_val := %d",root->tk->val.integer);
        }
        else if(root->tk->type == TK_RNUM){
            printf("Token_val := %-15.11lf",root->tk->val.decimal);
        }
        else{
            printf("Token_val := %s",root->tk->val.identifier);
        }
    }
    printf("\n");
    ASTNode* temp = root->child;
    while(temp!=NULL){
        printAST(temp);
        temp=temp->sibling;
    }
}

ASTNode* makeNode(Label label_1,struct Token* token_1,struct ASTNode* child_1,struct ASTNode* sibling_1){
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    newNode->label = label_1;
    newNode->tk = token_1;
    newNode->child = child_1;
    newNode->sibling = sibling_1;
    return newNode;
}

void makeAST(struct ParseNode* parserNode){
    ASTNode *newNode = NULL, *newNode1 = NULL;
    ParseNode *c1=NULL,*c2=NULL,*c3=NULL;
    // printf("%d ",parserNode->ruleno + 1);
    // printf("%s\n",mapnttostring(parserNode->val.nt));
    switch(parserNode->ruleno) {

        case 0:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            astroot = makeNode(PROGRAM,NULL,NULL,NULL);
            astroot->child = makeNode(MODULEDECLARATIONS,NULL,parserNode->child->addr,NULL);
            astroot->child->sibling = makeNode(OTHERMODULES1,NULL,parserNode->child->sibling->addr,NULL);
            astroot->child->sibling->sibling = parserNode->child->sibling->sibling->addr;
            astroot->child->sibling->sibling->sibling = makeNode(OTHERMODULES2,NULL,parserNode->child->sibling->sibling->sibling->addr,NULL);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            free(parserNode);
            break;

        case 1:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr; 
            parserNode->child->addr->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 2:
            parserNode->addr = NULL;
            break;
        
        case 3:
            parserNode->addr = makeNode(MODULEDECLARATION,parserNode->child->sibling->sibling->val.t,NULL,NULL);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 4:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->addr->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 5:
            parserNode->addr = NULL;
            break;

        case 6:
            scope_start1 = parserNode->child->val.t->lineNo;
            scope_end1 = parserNode->child->sibling->sibling->sibling->sibling->child->sibling->sibling->val.t->lineNo;
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = DRIVERMODULE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = NULL;
            parserNode->addr->scope_begin = scope_start1;
            parserNode->addr->scope_end = scope_end1;
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 7:
            scope_start1 = parserNode->child->val.t->lineNo;
            scope_end1 = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->sibling->sibling->val.t->lineNo;
            c1 = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
            c2 = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
            c3 = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
            makeAST(c1);
            makeAST(c2);
            makeAST(c3);
            parserNode->addr = makeNode(MODULE,NULL,NULL,NULL);
            parserNode->addr->scope_begin = scope_start1;
            parserNode->addr->scope_end = scope_end1;
            parserNode->addr->child = makeNode(ID,parserNode->child->sibling->sibling->val.t,NULL,c1->addr);
            if(c1->addr==NULL) parserNode->addr->child->sibling = c2->addr;
            else parserNode->addr->child->sibling->sibling = c2->addr; 
            if(c2->addr==NULL) {
                if(c1->addr==NULL) parserNode->addr->child->sibling = c3->addr;
                else parserNode->addr->child->sibling->sibling = c3->addr;
            }else{
                c2->addr->sibling = c3->addr;
            }
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 8:
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 9:
            parserNode->addr = NULL;
            break;
        
        case 10:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INPUT_PLIST;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = PARAMETER;
            parserNode->addr->child->tk = parserNode->child->val.t;
            parserNode->addr->child->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->child->scope_begin = scope_start1;
            parserNode->addr->child->scope_end = scope_end1;
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 11:
            makeAST(parserNode->child->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = PARAMETER;
            parserNode->addr->tk = parserNode->child->sibling->val.t;
            parserNode->addr->child = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->scope_begin = scope_start1;
            parserNode->addr->scope_end = scope_end1;
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 12:
            parserNode->addr = NULL;
            break;

        case 13:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = OUTPUT_PLIST;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = PARAMETER;
            parserNode->addr->child->tk = parserNode->child->val.t;
            parserNode->addr->child->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->child->scope_begin = scope_start1;
            parserNode->addr->child->scope_end = scope_end1;
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 14:
            makeAST(parserNode->child->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = PARAMETER;
            parserNode->addr->tk = parserNode->child->sibling->val.t;
            parserNode->addr->child = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->scope_begin = scope_start1;
            parserNode->addr->scope_end = scope_end1;
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 15:
            parserNode->addr = NULL;
            break;

        case 16:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INTEGER_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 17:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = REAL_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 18:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = BOOLEAN_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 19:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ARRAY_DTYPE;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 20:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ARRAY_RANGE;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = parserNode->child->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling); 
            free(parserNode->child);
            break;

        case 21:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INTEGER_;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 22:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = REAL_;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 23:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = BOOLEAN_;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 24:
            nest_level++;
            scope_end2 = scope_end1;
            scope_start2 = scope_start1;
            makeAST(parserNode->child->sibling);
            scope_end1 = scope_end2;
            scope_start1 = scope_start2;
            nest_level--;
            parserNode->addr = makeNode(STATEMENTS,NULL,parserNode->child->sibling->addr,NULL);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 25:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->addr->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 26:
            parserNode->addr = NULL;
            break;

        case 27:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 28:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 29:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;
        
        case 30:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;
        
        case 31:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 32:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INPUT;
            parserNode->addr->tk = parserNode->child->sibling->sibling->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 33:
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = OUTPUT;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 34:
            makeAST(parserNode->child->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->sibling = NULL;
            
            if (parserNode->child->sibling->addr == NULL) {
                parserNode->addr->label = ID;
                parserNode->addr->tk = parserNode->child->val.t;
                parserNode->addr->child = NULL;
            }
            else {
                parserNode->addr->label = ARR_OUTPUT;
                parserNode->addr->tk = NULL;
                parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
                parserNode->addr->child->label = ID;
                parserNode->addr->child->tk = parserNode->child->val.t;
                parserNode->addr->child->child = NULL;
                parserNode->addr->child->sibling = parserNode->child->sibling->addr;
            }
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 35:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = NUM;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 36:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = RNUM;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 37:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 38:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = TRUE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            free(parserNode->child);
            break;

        case 39:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = FALSE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 40:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
            
        case 41:
            parserNode->addr = NULL;
            break;

        case 42:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 43:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 44:
            parserNode->child->sibling->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->child->sibling->addr->label = ID;
            parserNode->child->sibling->addr->tk = parserNode->child->val.t;
            parserNode->child->sibling->addr->child = NULL;
            parserNode->child->sibling->addr->sibling = NULL;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 45:
            parserNode->child->addr = parserNode->addr;
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 46:
            parserNode->child->addr = parserNode->addr;
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 47:
            makeAST(parserNode->child->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ASSIGN;
            newNode->tk = NULL;
            newNode->child = parserNode->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 48:
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_ASSIGN;
            newNode->tk = NULL;
            newNode->child = parserNode->addr;
            newNode->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            newNode->child->child = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 49:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INDEX_ARR;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            if (parserNode->child->addr != NULL) {
                parserNode->addr->child = parserNode->child->addr;
                parserNode->addr->child->child = parserNode->child->sibling->addr;
            }
            else
                parserNode->addr->child = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 50:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = NUM;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 51:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ID;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 52:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = UNARY_PLUS;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 53:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = UNARY_MINUS;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 54:
            parserNode->addr = NULL;
            break;

        
        // ANSH
        case 55:
            c1= parserNode->child;
            c2= c1->sibling->sibling->sibling;
            c3= c2->sibling->sibling->sibling;
            makeAST(c1);
            makeAST(c3);
            newNode = makeNode(MODULE_REUSE,c2->val.t,c1->addr,NULL);
            if(c1->addr!=NULL) newNode->child->sibling = c3->addr;
            else newNode->child = c3->addr;
            parserNode->addr = newNode;
            free(c3->sibling->val.t); 
            free(c3->sibling); 
            free(c3);
            free(c2->sibling->sibling->val.t);
            free(c2->sibling->sibling);
            free(c2->sibling->val.t);
            free(c2->sibling);
            free(c2);
            free(c1->sibling->sibling->val.t);
            free(c1->sibling->sibling);
            free(c1->sibling->val.t);
            free(c1->sibling);
            free(c1);
            break;
        
        case 56:
            c1= parserNode->child;
            c2= c1->sibling;
            c3= c2->sibling;
            makeAST(c1); 
            makeAST(c2);
            makeAST(c3);
            newNode = makeNode(ID_LIST,NULL,c1->addr,NULL);
            if(c1->addr!=NULL){
                newNode->child->sibling = c2->addr;
                newNode->child->sibling->sibling = c3->addr;
            }else{
                newNode->child = c2->addr;
                newNode->child->sibling = c3->addr;
            }
            parserNode->addr = newNode;
            free(c3); 
            free(c2);
            free(c1);
            break;
        
        case 57:
            c1= parserNode->child->sibling;
            c2= c1->sibling;
            c3= c2->sibling;
            makeAST(c1); 
            makeAST(c2);
            makeAST(c3);
            if(c1->addr==NULL){
                parserNode->addr = c2->addr;
                c2->addr->sibling = c3->addr;
            }else{
                parserNode->addr = c1->addr;
                c1->addr->sibling = c2->addr;
                c2->addr->sibling = c3->addr;
            }
            free(c3);
            free(c2); 
            free(c1);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 58:
            parserNode->addr = NULL;
            break;
        
        case 59:
            parserNode->addr = makeNode(NUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child);
            break;

        case 60:
            parserNode->addr = makeNode(RNUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child);
            break;

        case 61:
            c1= parserNode->child;
            makeAST(c1); 
            newNode = makeNode(BOOLEAN_,NULL,c1->addr,NULL);
            free(c1);
            break;

        case 62:
            c1= parserNode->child->sibling;
            makeAST(c1); 
            newNode = makeNode(ID,parserNode->child->val.t,NULL,NULL);
            if(c1->addr==NULL){
                parserNode->addr = newNode;
            }else{
                parserNode->addr = makeNode(ARRAY,NULL,newNode,NULL);
                newNode->sibling = c1->addr;
            }
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;
        
        case 63:
            c1= parserNode->child->sibling;
            makeAST(c1); 
            parserNode->addr = makeNode(ASSIGN,NULL,c1->addr,NULL);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(c1);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 64:
            parserNode->addr = NULL;
            break;

        case 65:
            c1= parserNode->child;
            c2= c1->sibling;
            makeAST(c2); 
            newNode1 = makeNode(ID,c1->val.t,NULL,c2->addr);
            parserNode->addr = makeNode(ID_LIST,NULL,newNode1,NULL);
            free(c2);
            free(c1);
            break;
        
        case 66:
            c1 = parserNode->child->sibling;
            c2 = c1->sibling;
            makeAST(c2); 
            parserNode->addr = makeNode(ID,c1->val.t,NULL,c2->addr);
            free(c2);
            free(c1);
            free(parserNode->child);
            break;

        case 67:
            parserNode->addr = NULL;
            break;

        //TOSHIT
        case 68:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 69:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;
        
        case 70:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->child->addr->child = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 71:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 72:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 73:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = UNARY_PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;
        
        case 74:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = UNARY_MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 75:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 76:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 77:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 78:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 79:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 80:
            // parserNode->addr->True = newLabel1;
            // parserNode->addr->False = newLabel2;
            break;

        case 81:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 82:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 83:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 84:
            break;

        case 85:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 86:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 87:
            break;

        case 88:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 89:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 90:
            break;

        case 91:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 92:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 93:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 94:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 95:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            makeAST(parserNode->child->sibling);
            if (parserNode->child->sibling->addr != NULL) {
                newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
                newNode1->label = ARRAY;
                newNode1->tk = NULL;
                newNode1->child = newNode;
                newNode1->sibling = NULL;
                newNode1->child->sibling = parserNode->child->sibling->addr;
                parserNode->addr = newNode1;
            }
            else
                parserNode->addr = newNode;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 96:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child); 
            break;

        case 97:
            parserNode->addr = NULL; //no index case
            break;

        case 98:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_INDEX1;
            if(parserNode->child->addr!=NULL){
                newNode->child = parserNode->child->addr;
                newNode->child->child = parserNode->child->sibling->addr;
            }
            else    
                newNode->child = parserNode->child->sibling->addr;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 99:
            makeAST(parserNode->child);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_INDEX2;
            newNode->child = parserNode->child->addr;
            newNode->sibling = NULL;
            newNode->tk = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 100:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 101:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 102:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 103:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 104:
            break;

        case 105:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 106:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 107:
            break;

        case 108:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 109:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 110:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 111:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 112:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 113:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 114:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = MUL;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 115:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = DIV;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 116:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = AND;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 117:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = OR;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 118:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = LT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 119:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = LE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 120:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = GT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 121:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = GE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 122:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = EQ;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 123:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;     

        // ANSH
        case 124:
            // scope_start1 = parserNode->child->sibling->sibling->sibling->sibling->val.t->lineNo;
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = makeNode(DECLARE,NULL,parserNode->child->sibling->addr,NULL);
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            ASTNode* curr = parserNode->addr->child->child;
            while(curr!=NULL){
                curr->scope_begin = scope_start1;
                curr->scope_end = scope_end1;
                curr->nest_level = nest_level;
                curr = curr->sibling;
            }
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 125:
            c1= parserNode->child->sibling->sibling->sibling->sibling->sibling;
            c2= c1->sibling; 
            //int line  = c2->sibling->val.t->lineNo;
            makeAST(c2);
            c1->addr = c2->addr;
            makeAST(c1);
            parserNode->addr = makeNode(CASE_STMT,parserNode->child->sibling->sibling->val.t,c1->addr,NULL);
            parserNode->addr->scope_end = c2->sibling->val.t->lineNo;
            free(c2->sibling->val.t);
            free(c2->sibling);
            free(c2);
            free(c1);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 126:
            c1= parserNode->child->sibling;
            c2= c1->sibling->sibling;
            c3= c2->sibling->sibling->sibling;
            makeAST(c1); 
            nest_level++;
            scope_start1 = parserNode->child->val.t->lineNo;
            scope_end1 = c2->sibling->val.t->lineNo;
            makeAST(c2);
            nest_level--;
            c3->addr = parserNode->addr;
            makeAST(c3);
            newNode = makeNode(CASE,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            if(c3->addr==NULL)
                newNode->sibling = parserNode->addr;
            else
                newNode->sibling = c3->addr;
            parserNode->addr = newNode;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 127:
            c1= parserNode->child->sibling;
            c2= c1->sibling->sibling;
            c3= c2->sibling->sibling->sibling;
            nest_level++;
            scope_start1 = parserNode->child->val.t->lineNo;
            scope_end1 = c2->sibling->val.t->lineNo;
            makeAST(c1); 
            makeAST(c2);
            nest_level--;
            c3->addr = parserNode->addr;
            makeAST(c3);
            newNode = makeNode(CASE,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            if(c3->addr==NULL)
                newNode->sibling = parserNode->addr;
            else
                newNode->sibling = c3->addr;
            parserNode->addr = newNode;
            //if(c3->addr==NULL) newNode->child->sibling->sibling = c3->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 128:
            //parserNode->addr = NULL;
            break;

        case 129:
            parserNode->addr = makeNode(NUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child);
            break;

        case 130:
            parserNode->addr = makeNode(TRUE,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child);
            break;

        case 131:
            parserNode->addr = makeNode(FALSE,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child);
            break;
        
        case 132:
            c1 = parserNode->child->sibling->sibling;
            //struct Token* tk = parserNode->child->val.t;
            int line = parserNode->child->val.t->lineNo;
            makeAST(c1); 
            parserNode->addr = makeNode(DEFAULT,NULL,c1->addr,NULL);
            parserNode->addr->scope_begin = line;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling); 
            free(parserNode->child->sibling->val.t); 
            free(parserNode->child->sibling); 
            free(parserNode->child->val.t); 
            free(parserNode->child); 
            break;

        case 133:
            parserNode->addr = NULL;
            break;

        case 134:
            // iterator stored in range_for node
            c1= parserNode->child->sibling->sibling->sibling->sibling;
            c2= c1->sibling->sibling->sibling;
            nest_level++;
            scope_end2 = scope_end1;
            scope_start2 = scope_start1;
            scope_start1 = parserNode->child->val.t->lineNo;
            scope_end1 = c2->sibling->val.t->lineNo;
            makeAST(c1);
            makeAST(c2);
            scope_end1 = scope_end2;
            scope_start1 = scope_start2;
            nest_level--;
            newNode = makeNode(ITER_FOR,parserNode->child->sibling->sibling->val.t,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            newNode->scope_begin = parserNode->child->sibling->sibling->val.t->lineNo;
            newNode->scope_end = c2->sibling->val.t->lineNo;
            newNode->nest_level = nest_level+1;
            parserNode->addr = newNode;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling); 
            free(parserNode->child->sibling->val.t); 
            free(parserNode->child->sibling); 
            free(parserNode->child->val.t); 
            free(parserNode->child); 
            break;

        case 135:
            c1= parserNode->child->sibling->sibling;
            c2= c1->sibling->sibling->sibling;
            nest_level++;
            scope_start2 = scope_start1;
            scope_end2 = scope_end1;
            scope_end1 = c2->sibling->val.t->lineNo;
            scope_start1 = parserNode->child->val.t->lineNo;
            makeAST(c1); 
            makeAST(c2);
            scope_end1 = scope_end2;
            scope_start1 = scope_start2;
            nest_level--;
            newNode = makeNode(RANGE_WHILE,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            newNode->scope_begin = parserNode->child->val.t->lineNo;
            newNode->scope_end = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->val.t->lineNo;
            parserNode->addr = newNode;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling); 
            free(parserNode->child->sibling->val.t); 
            free(parserNode->child->sibling); 
            free(parserNode->child->val.t); 
            free(parserNode->child); 
            break;
        
        case 136:
            c1= parserNode->child;
            c2= c1->sibling->sibling;
            makeAST(c1); 
            makeAST(c2);
            newNode = makeNode(RANGE_FOR,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            parserNode->addr = newNode;
            free(c2); 
            free(c1->sibling->val.t);
            free(c1->sibling);
            free(c1);
            break;
        
        case 137:
            c1= parserNode->child;
            c2= c1->sibling;
            makeAST(c1); 
            makeAST(c2);
            if(parserNode->child->addr!=NULL){ 
                parserNode->addr = c1->addr;
                c1->addr->child = c2->addr;
            }
            else parserNode->addr = c2->addr;
            free(c2); 
            free(c1);
            break;

        case 138:
            parserNode->addr = makeNode(NUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child);
            break;

        default:
            printf("ERROR: No such rule exist\n");
    }
}

ASTNode* AST(){
    ptr = fopen("testcase7.txt","r");
    ptr = initLexer(ptr, 32);
    loadgrammar("grammar.txt");
    computefirstandfollow();
    createParseTable();
	ParseNode* parserNode = parse();
    // FILE* fp = fopen("checktree.txt","w");
    // printf("Printing AST ruleno\n");
    // printTree(parserNode,fp);
    // fflush(fp); fclose(fp);
	makeAST(parserNode);
    // printf("\nPrinting AST\n\n");
    // printAST(astroot);
    printf("\nTypeChecking AST ...\n\n");
    typeChecker(astroot);
    printSymbolTable();
	return astroot;
}
