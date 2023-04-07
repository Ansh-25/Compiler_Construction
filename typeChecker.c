// #include "symbolTableDef.h"

// MainTableEntry** SymbolTable;
// ModuleTableEntry** curr; //set curr at every module node

// /* how to handle if module reuse before module definition */

// typechecker(ASTNode* astNode){
//     int c = astNode->label;
//     switch(c){

//         case 35: //case assign
//             ASTNode* rhs = astNode->child->sibling;
//             if(astNode->child->label==ARR_ASSIGN){ //case arr assign
//                 ASTNode* lhs = astNode->child->child;
//                 if(searchVar(curr,lhs->tk->val.identifier)==NULL){
//                     //Error:variable not found
//                 }
//                 else if(searchVar(curr,lhs->tk->val.identifier)->t.t.at.pt!=rhs->type.t.pt){
//                     //Type Error:datatypes don't match at line no. lhs->tk->lineno.
//                 }
//             }
//             else{
//                 ASTNode* lhs = astNode->child;
//                 if(searchVar(curr,lhs->tk->val.identifier)==NULL){
//                 //Error:variable not found
//                 }
//                 else if(compare_Datatype(searchVar(curr,lhs->tk->val.identifier)->t,rhs->type)!=true){
//                     //Type Error:datatypes don't match at line no. lhs->tk->lineno.
//                 }
//             }          
//             typechecker(astNode->sibling);
//             break;


//         case 38: //case declare
//             DataType d = astNode->child->sibling->type;
//             ASTNode* idList = astNode->child->child;
//             while(idList!=NULL){
//                 ModuleTableEntry* new_entry = (ModuleTableEntry*)malloc(sizeof(ModuleTableEntry));
//                 new_entry->identifier = idList->tk->val.identifier;
//                 new_entry->t = d;
//                 insertVar(curr,new_entry);
//                 idList = idList->sibling;
//             }
//             typechecker(astNode->sibling);
//             break;

        

//     }
// }