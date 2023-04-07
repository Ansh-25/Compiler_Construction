#include "symbolTableDef.h"

MainTableEntry** SymbolTable;
ModuleTableEntry** curr; //set curr at every module node

void createMainTable(int size) {
    SymbolTable = (MainTableEntry**)malloc(size * sizeof(MainTableEntry*));
    for (int i = 0; i < size; i ++)
        SymbolTable[i] = NULL;
}

ModuleTableEntry** createModuleTable(int size) {
    ModuleTableEntry** newTable = (ModuleTableEntry*)malloc(size * sizeof(ModuleTableEntry*));
    for (int i = 0; i < size; i ++)
        newTable[i] = NULL;
}

MainTableEntry* createModule(char* name, ParamList* inputList, ParamList* outputList, ModuleTableEntry** table) {
    MainTableEntry* newModule = malloc(sizeof(MainTableEntry));
    newModule->module_name = name;
    newModule->inputList = inputList;
    newModule->outputList = outputList;
    newModule->moduleTable = table;
    return newModule;
}

typechecker(ASTNode* astNode){
    int c = astNode->label;
    switch(c){
        case PROGRAM:
            for (ASTNode* current = astNode->child; current != NULL; current = current -> sibling)
                typechecker(current);
            break;

        case MODULEDECLARATIONS:
            for (ASTNode* curr = astNode->child; curr != NULL; curr = curr -> sibling)
                typechecker(curr);
            break;
        
        case OTHERMODULES1:
            for (ASTNode* current = astNode->child; current != NULL; current = current -> sibling)
                typechecker(current);
            break;

        case OTHERMODULES2:
            for (ASTNode* current = astNode->child; current != NULL; current = current -> sibling)
                typechecker(current);
            break;

        case MODULEDECLARATION:
            if (searchModule(SymbolTable,astNode->tk->val.identifier))
                printf("ERROR at line %d: Module \"%s\" has already been declared\n",astNode->tk->lineNo, astNode->tk->val.identifier);
            else 
                insertModule(SymbolTable,createModule(astNode->tk->val.identifier,NULL,NULL,NULL));
            break;

        case DRIVERMODULE:
            curr = createModuleTable(20);
            insertModule(SymbolTable,createModule("driver",NULL,NULL,curr));
            typechecker(astNode->child);
            break;

        case MODULE:
            MainTableEntry* searched = searchModule(SymbolTable, astNode->child->tk->val.identifier);
            if (searched != NULL && searched->moduleTable != NULL)
                printf("ERROR at line %d: Module \"%s\" has already been defined\n",astNode->tk->lineNo, astNode->tk->val.identifier);
            else {
                curr = createModuleTable(20);
                if (searched == NULL)
                    insertModule(SymbolTable,createModule(astNode->child->tk->val.identifier,NULL,NULL,curr));
                else
                    searched->moduleTable = curr;
                ASTNode* current = astNode->child->sibling;
                while (current != NULL) {
                    if (current->label == INPUT_PLIST) {
                        ASTNode* parameter = current -> child;
                        ParamList* input_plist = NULL;
                        while (parameter != NULL) {
                            ParamList* newnode = (ParamList*)malloc(sizeof(ParamList));
                            newnode->identifier = parameter->tk->val.identifier;
                            typechecker(parameter->child);
                            newnode->t = parameter->child->type;
                            newnode->next = NULL;
                            input_plist = insertlast(input_plist, newnode);
                            parameter = parameter->sibling;
                        }
                        searched->inputList = input_plist;
                    }
                    else if (current->label = OUTPUT_PLIST) {
                        ASTNode* parameter = current -> child;
                        ParamList* output_plist = NULL;
                        while (parameter != NULL) {
                            ParamList* newnode = (ParamList*)malloc(sizeof(ParamList));
                            newnode->identifier = parameter->tk->val.identifier;
                            typechecker(parameter->child);
                            newnode->t = parameter->child->type;
                            newnode->next = NULL;
                            output_plist = insertlast(input_plist, newnode);
                            parameter = parameter->sibling;
                        }
                        searched->outputList = output_plist;
                    }
                    else {
                        for (ASTNode* stmt = current->child; stmt != NULL; stmt = stmt->sibling)
                            typechecker(stmt);
                    }
                    current = current -> sibling;
                }
            }
            break;

        case INTEGER_:
            astNode->type.is_primitive = true;
            astNode->type.pt = INTEGER;
            break;

        case REAL_:
            astNode->type.is_primitive = true;
            astNode->type.pt = REAL;
            break;

        case BOOLEAN_:
            astNode->type.is_primitive = true;
            astNode->type.pt = BOOLEAN;
            break;

        case ARRAY:
            typechecker(astNode->child->sibling);
            astNode->type.is_primitive = false;
            astNode->type.pt = astNode->child->sibling->type;
            break;

        case 35: //case assign
            ASTNode* rhs = astNode->child->sibling;
            if(astNode->child->label==ARR_ASSIGN){ //case arr assign
                ASTNode* lhs = astNode->child->child;
                if(searchVar(curr,lhs->tk->val.identifier)==NULL){
                    //Error:variable not found
                }
                else if(searchVar(curr,lhs->tk->val.identifier)->t.t.at.pt!=rhs->type.t.pt){
                    //Type Error:datatypes don't match at line no. lhs->tk->lineno.
                }
            }
            else{
                ASTNode* lhs = astNode->child;
                if(searchVar(curr,lhs->tk->val.identifier)==NULL){
                //Error:variable not found
                }
                else if(compare_Datatype(searchVar(curr,lhs->tk->val.identifier)->t,rhs->type)!=true){
                    //Type Error:datatypes don't match at line no. lhs->tk->lineno.
                }
            }          
            typechecker(astNode->sibling);
            break;


        case 38: //case declare
            DataType d = astNode->child->sibling->type;
            ASTNode* idList = astNode->child->child;
            while(idList!=NULL){
                ModuleTableEntry* new_entry = (ModuleTableEntry*)malloc(sizeof(ModuleTableEntry));
                new_entry->identifier = idList->tk->val.identifier;
                new_entry->t = d;
                insertVar(curr,new_entry);
                idList = idList->sibling;
            }
            typechecker(astNode->sibling);
            break;
    }
}