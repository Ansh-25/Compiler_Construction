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
    switch(parserNode->ruleno) {
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
            ASTNode* newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
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
