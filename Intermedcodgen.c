#include "IntCodeGenDef.h"

Quadruple* Intermediate_code;

void concatList(QuadNode *a, QuadNode *b){
    if(a==NULL)
        return b;
    if(b==NULL) return a;
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

//can make this return void, not necessary to return anything
//just go up the AST  filling up .code, at the end we return root->code;
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

        case MODULEDECLARATIONS: 
            return NULL;
            break;

        case OTHERMODULES1: 
            for(curr=astNode->child;curr!=NULL;curr=curr->sibling)
                IntCodeGen(curr);
            for(curr=astNode->child;curr->sibling!=NULL;curr=curr->sibling){
                concatList(curr->code,curr->sibling->code);
            }
            astNode->code = astNode->child->code;
            return astNode->code;
            break;

        case OTHERMODULES2: 
            for(curr=astNode->child;curr!=NULL;curr=curr->sibling)
                IntCodeGen(curr);
            for(curr=astNode->child;curr->sibling!=NULL;curr=curr->sibling){
                concatList(curr->code,curr->sibling->code);
            }
            astNode->code = astNode->child->code;
            return astNode->code;
            break;

        case DRIVERMODULE:
            for(curr=astNode->child;curr!=NULL;curr=curr->sibling)
                IntCodeGen(curr);
            for(curr=astNode->child;curr->sibling!=NULL;curr=curr->sibling){
                concatList(curr->code,curr->sibling->code);
            }
            astNode->code = astNode->child->code;
            return astNode->code;
            break;

        case MODULE:
            astNode->code = astNode->child->sibling->sibling->sibling->code;
            return astNode->code;

        case INTEGER_:
	//don't need        

        case REAL_:
	//don't need        

        case BOOLEAN_:
	//dont need        

        case ARRAY_DTYPE:
            //don't need
        case INPUT:

        case OUTPUT:
       

        case ASSIGN:
		//create for child
            IntCodeGen(astNode->child->sibling);
            QuadNode* assignment = (QuadNode*)malloc(sizeof(QuadNode));
            assignment.op = EQUALS;
            assignment.res.var = ;
            assignment.first = EQUALS;
            assignment.second = EQUALS;
            concatList(astNode->child->sibling->code, assignment);
            astNode->code = astNode->child->sibling->code;
            return astNode->code;

        case ARR_ASSIGN:
        
        case ARR_INDEX1:
        
        case ARR_INDEX2:
        
        case DECLARE:
           //don't need

        case UNARY_PLUS:
        

        case UNARY_MINUS:
       

        case ID:
        

        case NUM:
        

        case RNUM:
        

        case TRUE:
        

        case FALSE:
        

        case ARRAY:
        

        case PLUS:
        

        case MINUS:
        

        case MUL:
        

        case DIV:
        
        case AND:

        case OR:
        
        case LT:
        
        case LE:
        
        case GT:
        
        case GE:
       
 
        case EQ:
    
    
        case NE:
    
    
        case RANGE_WHILE:
        
        case ITER_FOR:
        

        case RANGE_FOR:
        
        case CASE_STMT:


        case CASE:
        
        case MODULE_REUSE:

        default:
            break;
	
    }
}