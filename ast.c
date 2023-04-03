#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "astDef.h"
#include "parserDef.h"


/*
IMPORTANT THINGS TO REMEMBER

1. make linked list using siblings.


*/

ASTNode* astroot;

/*

ASTList* insertAtEnd(ASTList* head1,ASTList* head2){
    if(head1==NULL) return head1 = head2;
    if(head2 == NULL) return head1;
    while(head1->next != NULL) head1 = head1 -> next;
    head1 -> next = head2;
    return head1;
	//shreekar: wouldn't this return the second last node in the list and not the list itself?
}

void make_ast(parserNode){
    ParseNode* temp = parserNode->child;
    while(temp!=NULL){
        makeAST(temp);
        temp = temp->sibling;
    }
}*/

//rule no for leafnodes is same as that of their parents, change in parser;
void makeAST(struct ParseNode* parserNode){
    ASTNode* newNode = NULL;
    ASTNode* newNode1 = NULL;
    switch(parserNode->ruleno) {

        case 0:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            astroot = (ASTNode*)malloc(sizeof(ASTNode));
            astroot->child = parserNode->child->addr;
            astroot->child->sibling = parserNode->child->sibling->addr;
            astroot->child->sibling->sibling = parserNode->child->sibling->sibling->addr;
            astroot->child->sibling->sibling->sibling = parserNode->child->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            free(parserNode);
            break;

        case 1:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            if (parserNode->child->addr == NULL)
                parserNode->addr = parserNode->child->sibling->addr;
            else {
                ASTNode* curr = parserNode->child->addr;
                while (curr->sibling != NULL)
                    curr = curr -> sibling;
                curr->sibling = parserNode->child->sibling->addr;
            }
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 2:
            parserNode->addr = NULL;
            break;
        
        case 3:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->sibling->sibling->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = MODULEDECLARATION;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = newNode;
            parserNode->addr->sibling = NULL;
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
            if (parserNode->child->addr == NULL)
                parserNode->addr = parserNode->child->sibling->addr;
            else {
                ASTNode* curr = parserNode->child->addr;
                while (curr->sibling != NULL)
                    curr = curr -> sibling;
                curr->sibling = parserNode->child->sibling->addr;
            }
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 5:
            parserNode->addr = NULL;
            break;

        case 6:
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = DRIVERMODULE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->sibling->sibling->addr;
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
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = MODULE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = ID;
            parserNode->addr->child->tk = parserNode->child->sibling->sibling->val.t;
            parserNode->addr->child->child = NULL;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->child->sibling->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->child->sibling->sibling->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->addr;
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
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = RET;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->addr;
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

        case 9:
            parserNode->addr = NULL;
            break;
        
        case 10:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = PARAMETER;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = ID;
            parserNode->addr->child->tk = parserNode->child->val.t;
            parserNode->addr->child->child = NULL;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->addr;
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
            parserNode->addr->tk = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = ID;
            parserNode->addr->child->tk = parserNode->child->sibling->val.t;
            parserNode->addr->child->child = NULL;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
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
            parserNode->addr->label = PARAMETER;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = ID;
            parserNode->addr->child->tk = parserNode->child->val.t;
            parserNode->addr->child->child = NULL;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->addr;
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
            parserNode->addr->tk = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = ID;
            parserNode->addr->child->tk = parserNode->child->sibling->val.t;
            parserNode->addr->child->child = NULL;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
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
            parserNode->addr->label = INTEGER;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 17:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = REAL;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 18:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = BOOLEAN;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 19:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ARRAY;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->t);
            free(parserNode->child->sibling);
            free(parserNode->child->t);
            free(parserNode->child);
            break;

        case 20:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = RANGE;
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
            parserNode->addr->label = INTEGER;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 22:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = REAL;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 23:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = BOOLEAN;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 24:
            makeAST(parserNode->child->sibling);
            parserNode->addr =  parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 25:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            newNode = parserNode->child->addr;
            newNode->sibling = parserNode->addr->child;
            parserNode->addr->child = newNode;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 26:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = STATEMENTS;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
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
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = ID;
            parserNode->addr->child->tk = parserNode->child->sibling->sibling->val.t;
            parserNode->addr->child->child = NULL;
            parserNode->addr->child->sibling = NULL;
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
            parserNode->sibling = NULL;
            if (parserNode->child->sibling->addr == NULL) {
                parserNode->addr->label = ID;
                parserNode->addr->tk = parserNode->child->addr;
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
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 39:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = FALSE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
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
            newNode->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ASSIGN;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = newNode;
            parserNode->addr->sibling = NULL;
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
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 53:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = UNARY_MINUS;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 54:
            parserNode->addr = NULL;
            break;

        /*case 0:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            astroot = (ASTNode*)malloc(sizeof(ASTNode));
            astroot->label = PROGRAM_NODE;
            astroot->moduleDeclarationsList = parserNode->child->addr;
            astroot->moduleList = parserNode->child->sibling->addr;
            insertAtEnd(astroot->moduleList, parserNode->child->sibling->sibling->addr);
            insertAtEnd(astroot->moduleList, parserNode->child->sibling->sibling->sibling->addr);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            free(parserNode);
            break;


        //Toshit

        case 33:
            // newNode = (ASTNode*)malloc(sizeof(ASTNode));
            // newNode->node_type->pnode = parserNode->child->sibling->sibling;
            //newNode->val = parserNode->child->sibling->sibling->val;
            parserNode->addr = parserNode->child->sibling->sibling;
            free(RHS-ID);
            break;

        case 34:
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(RHS);
            break;

        case 35: 
            makeAST(parserNode->child->sibling);
            //newNode = (ASTNode*)malloc(sizeof(ASTNode));
            //newNode->node_type->pnode = parserNode->child;
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR;
            newNode1->attr.arr_name = parserNode->child;
            newNode1->attr.arr_index = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 36:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 37:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 38:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(RHS);
            break;

        case 39:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = TRUE;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 40:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = FALSE;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 41:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(RHS);
            break;

        case 42:
            parserNode->addr = NULL;
            free(RHS);
            break;

        case 43:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(RHS);
            break;

        case 44:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(RHS);
            break;

        case 45: 
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            parserNode->child->sibling->addr = newNode;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(RHS);
            break;

        case 46:
            parserNode->child->addr = parserNode->addr;
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(RHS);
            break;

        case 47:
            parserNode->child->addr = parserNode->addr;
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(RHS);
            break;

        case 48:
            makeAST(parserNode->child->sibling); //for <expression>.addr
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->l = lhs_var;
            newNode1->child = parserNode->addr;
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = assignop;
            newNode->child = newNode1;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 49:
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->l = lhs_arr;
            newNode1->child = parserNode->addr;
            newNode1->child->sibling = parserNode->child->sibling->addr;
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = assignop;
            newNode->child = newNode1;
            newNode->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 50:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = arr_index;
            newNode->child = parserNode->child->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 51:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            parserNode->addr = newNode;
            free(RHS);
            break;
        
        case 52:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 53:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = PLUS;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 54:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = MINUS;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 55:
            parserNode->addr = NULL;
            free(RHS);
            break;

        case 56: 
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID; //parserNode->child->sibling->sibling->sibling
            parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->addr = newNode;
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling);
            parserNode->child->addr = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->addr;
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(RHS);
            break;

        case 57: 
            makeAST(parserNode->child); //<K>
            makeAST(parserNode->child->sibling); //<N_12>
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->l = u_param;
            newNode1->child = parserNode->child->addr;
            parserNode->list = insertatFront(parserNode->child->sibling->list,newNode1);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->l = module;
            newNode->attr->m.ID = parserNode->addr;
            newNode->attr->m.param_list = parserNode->list;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 58:
            makeAST(parserNode->child);//<sign>
            makeAST(parserNode->child->sibling); //<K>
            makeAST(parserNode->child->sibling->sibling); //<N_12>
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->l = s_param;
            newNode1->child = parserNode->child->addr;
            newNode1->child->sibling = parserNode->child->sibling->addr;
            parserNode->list = insertatFront(parserNode->child->sibling->sibling->list,newNode1);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->l = module;
            newNode->attr->m.ID = parserNode->addr;
            newNode->attr->m.param_list = parserNode->list;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 59:
            makeAST(parserNode->child->sibling);//<sign>
            makeAST(parserNode->child->sibling->sibling); //<K>
            makeAST(parserNode->child->sibling->sibling->sibling); //<N_12>
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            if(parserNode->child->sibling->addr!=NULL){
                newNode1->l = s_param;
                newNode1->child = parserNode->child->sibling->addr;
                newNode1->child->sibling = parserNode->child->sibling->sibling->addr;
            }
            else{
                newNode1->l = u_param;
                newNode1->child = parserNode->child->sibling->sibling->addr;
            }
            parserNode->list = insertatFront(parserNode->child->sibling->sibling->list,newNode1);
            free(RHS);
            break;

                 
        case 60:
            parserNode->list = new ASTList;
            free(RHS);
            break;

        case 61:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 62:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 63:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(RHS);
            break;

        case 64:
            makeAST(parserNode->child->sibling);
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->l = ID;
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->l = arr;
            newNode->child = newNode1;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;
            
        case 65:
            makeAST(parserNode->child->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = Optional1;
            newNode->child = parserNode->child->sibling->addr;
            newNode->child->sibling = parserNode->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 66:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = Optional1;
            newNode->child = parserNode->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 67:
            makeAST(parserNode->child->sibling);
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->l = ID;
            parserNode->addr = insertatFront(parserNode->child->sibling->list,newNode1);
            free(RHS);
            break;

        case 68:
            makeAST(parserNode->child->sibling->sibling);
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->l = ID;
            parserNode->list = insertatFront(parserNode->child->sibling->sibling->list,newNode1);
            free(RHS);
            break;

        case 69:
            parserNode->list = new ASTList;
            free(RHS);
            break;*/


        case 70:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 71:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;
        
        case 72:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->child->addr->child = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 73:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 74:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 75:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = UNARY_PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;
        
        case 76:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = UNARY_MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 77:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 78:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 79:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 80:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 81:
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

        case 82:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 83:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 84:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 85:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 86:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 87:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 88:
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

        case 89:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 90:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 91:
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

        case 92:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 93:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 94:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 95:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 96:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 97:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            makeAST(parserNode->child->sibling);
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->label = ARRAY;
            newNode1->child = newNode;
            newNode1->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode1;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 98:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child); 
            break;

        case 99:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 100:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_INDEX1;
            newNode->child = parserNode->child->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 101:
            makeAST(parserNode->child);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_INDEX2;
            newNode->child = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 102:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 103:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 104:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 105:
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

        case 106:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 107:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 108:
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

        case 109:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 110:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 111:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 112:
            makeAST(parserNode->child);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_INDEX2;
            newNode->child = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 113:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 114:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 115:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 116:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = MUL;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 117:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = DIV;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 118:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = AND;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 119:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = OR;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 120:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = LT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 121:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = LE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 122:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = GT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 123:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = GE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 124:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = EQ;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 125:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;       

        default:
            printf("Bye\n");
    }
    // if (root->ruleno == 1) {
    //     inherited semantic rules
    //     visit required children e.g. makeAST(root -> idList)
    //     synthesized symantic rules
    // }
    // else if (root->ruleno == 2) {
    //     inherited semantic rules
    //     visit required children e.g. makeAST(root -> idList)
    //     synthesized symantic rules
    // }
    // and so on
}

ASTNode* AST(){
	ParseNode* parserNode = parse();
	makeAST(parserNode);
	return astroot;
}

int main(){
    AST();
    printf("hi\n");
    return 0;
}
