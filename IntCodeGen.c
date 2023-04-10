#include "IntCodeGenDef.h"

Quadruple* Intermediate_code;

void concatList(QuadNode *a, QuadNode *b){
    if(a==NULL)
        return b;
    QuadNode* temp = a;
    while(temp->next!=NULL){
        temp = temp->next;
    }
    temp->quad.op = b->quad.op;
    temp->quad.res = b->quad.res;
    temp->quad.first = b->quad.first;
    temp->quad.second = b->quad.second;
    temp->next = b->next;
}

QuadNode* IntCodeGen(ASTNode* astNode){
    if (astNode == NULL)
        return;
    int c = astNode->label;
    ASTNode* curr = NULL;
    switch(c){
        case PROGRAM:
            for(curr=astNode->child;curr!=NULL;curr=curr->sibling)
                IntCodeGen(curr);
            for(curr=astNode->child;curr->sibling!=NULL;curr=curr->sibling){
                concatList(curr->code,curr->sibling->code);
            }
            astNode->code = astNode->child->code;
            return astNode->code;
            break;

        default:
            break;

        

            

    }
}