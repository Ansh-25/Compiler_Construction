#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "astDef.h"
//#include "parserDef.h"

/*
IMPORTANT THINGS TO REMEMBER

1. make linked list using siblings.


*/

func makenode()

ASTNode* astroot;

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
}

//rule no for leafnodes is same as that of their parents, change in parser;
void makeAST(ParseNode* parserNode){
    switch(parserNode->ruleno) {
        case 0:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            astroot = (ASTNode*)malloc(sizseof(ASTNode));
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
            // ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            //ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            //newNode->node_type->pnode = parserNode->child;
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ARR;
            newNode1->attr.arr_name = parserNode->child;
            newNode1->attr.arr_index = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 36:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = NUM;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 37:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = TRUE;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 40:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->l = lhs_var;
            newNode1->child = parserNode->addr;
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = assignop;
            newNode->child = newNode1;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 49:
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->l = lhs_arr;
            newNode1->child = parserNode->addr;
            newNode1->child->sibling = parserNode->child->sibling->addr;
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = assignop;
            newNode->child = newNode1;
            newNode->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 50:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = arr_index;
            newNode->child = parserNode->child->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 51:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = NUM;
            parserNode->addr = newNode;
            free(RHS);
            break;
        
        case 52:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ID;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 53:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = PLUS;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 54:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = MINUS;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 55:
            parserNode->addr = NULL;
            free(RHS);
            break;

        case 56: 
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->l = u_param;
            newNode1->child = parserNode->child->addr;
            parserNode->list = insertatFront(parserNode->child->sibling->list,newNode1);
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->l = s_param;
            newNode1->child = parserNode->child->addr;
            newNode1->child->sibling = parserNode->child->sibling->addr;
            parserNode->list = insertatFront(parserNode->child->sibling->sibling->list,newNode1);
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = NUM;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 62:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
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
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->l = ID;
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->l = arr;
            newNode->child = newNode1;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;
            
        case 65:
            makeAST(parserNode->child->sibling);
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = Optional1;
            newNode->child = parserNode->child->sibling->addr;
            newNode->child->sibling = parserNode->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 66:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = Optional1;
            newNode->child = parserNode->addr;
            parserNode->addr = newNode;
            free(RHS);
            break;

        case 67:
            makeAST(parserNode->child->sibling);
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->l = ID;
            parserNode->addr = insertatFront(parserNode->child->sibling->list,newNode1);
            free(RHS);
            break;

        case 68:
            makeAST(parserNode->child->sibling->sibling);
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->l = ID;
            parserNode->list = insertatFront(parserNode->child->sibling->sibling->list,newNode1);
            free(RHS);
            break;

        case 69:
            parserNode->list = new ASTList;
            free(RHS);
            break;


        case 70:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 71:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
        
        case 72:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->child->addr->child = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 73:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 74:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 75:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = UNARY_PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
        
        case 76:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = UNARY_MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 77:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 78:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 79:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 80:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

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

        case 82:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 83:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 84:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 85:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 86:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 87:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

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

        case 89:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 90:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

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

        case 92:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 93:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 94:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 95:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 96:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 97:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            makeAST(parserNode->child->sibling);
            ASTNode* newNode1 = (ASTNode*)malloc(sizseof(ASTNode));
            newNode1->label = ARRAY;
            newNode1->child = newNode;
            newNode1->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode1;
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 98:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child); 

        case 99:
            parserNode->addr = NULL;
            free(parserNode->child); 

        case 100:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ARR_INDEX1;
            newNode->child = parserNode->child->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 101:
            makeAST(parserNode->child);
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ARR_INDEX2;
            newNode->child = parserNode->child->addr;
            free(parserNode->child);

        case 102:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 103:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 104:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

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

        case 106:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 107:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);

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

        case 109:
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);

        case 110:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 111:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);

        case 112:
            makeAST(parserNode->child);
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = ARR_INDEX2;
            newNode->child = parserNode->child->addr;
            free(parserNode->child);

        case 113:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 114:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 115:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 116:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = MUL;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 117:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = DIV;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 118:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = AND;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 119:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = OR;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 120:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = LT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 121:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = LE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 122:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = GT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 123:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = GE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 124:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = EQ;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);

        case 125:
            ASTNode* newNode = (ASTNode*)malloc(sizseof(ASTNode));
            newNode->label = NE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);


        

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

AST(){
	ParseNode* parserNode = parse();
	makeAST(parserNode);
	return astRoot;
}

int main(){
    AST();
    printf("hi\n");
    return 0;
}
