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
    if(astNode==NULL)
        return;
    int c = astNode->label;
    ASTNode* current = astNode->child;
    for (ASTNode* current = astNode->child; current != NULL; current = current -> sibling) typechecker(current);
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

        case ARRAY_DTYPE:
            typechecker(astNode->child->sibling);
            astNode->type.is_primitive = false;
            astNode->type.pt = astNode->child->sibling->type.pt;
            astNode->type.lower_bound = INT_MIN;
            astNode->type.upper_bound = INT_MIN;
            ASTNode* left = astNode->child->child;
            ASTNode* right = astNode->child->child->sibling;
            if (left->label == UNARY_MINUS) {
                if (left->child->label == NUM)
                    astNode->type.lower_bound = (-1) * left->child->tk->val.integer;
                else
                    printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n",left->child->tk->lineNo, left->child->tk->val.identifier);
            }
            else if (left->label == UNARY_PLUS) {
                if (left->child->label == NUM)
                    astNode->type.lower_bound = left->child->tk->val.integer;
                else
                    printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n",left->child->tk->lineNo, left->child->tk->val.identifier);
            }
            else if (left->label = NUM)
                astNode->type.lower_bound = left->child->tk->val.integer;
            else
                printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n",left->tk->lineNo, left->tk->val.identifier);
            if (right->label == UNARY_MINUS) {
                if (right->child->label == NUM)
                    astNode->type.lower_bound = (-1) * right->child->tk->val.integer;
                else
                    printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n",right->child->tk->lineNo, right->child->tk->val.identifier);
            }
            else if (right->label == UNARY_PLUS) {
                if (right->child->label == NUM)
                    astNode->type.lower_bound = right->child->tk->val.integer;
                else
                    printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n",right->child->tk->lineNo, right->child->tk->val.identifier);
            }
            else if (right->label = NUM)
                astNode->type.lower_bound = right->child->tk->val.integer;
            else
                printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n",right->tk->lineNo, right->tk->val.identifier);
            if (astNode->type.upper_bound != INT_MIN && astNode->type.lower_bound != INT_MIN && astNode->type.upper_bound < astNode->type.lower_bound)
                printf("Error: lower bound of array should be less than or equal to upper bound\n");
            break;

        case INPUT:
            ModuleTableEntry* var = searchVar(curr, astNode->tk->val.identifier);
            if (var == NULL)
                printf("Error at line %d: identifier\"%s\" not recognized\n",astNode->tk->lineNo, astNode->tk->val.identifier);
            else if (var->t.is_primitive == false)
                printf("Error at line %d: cannot take array as input\n",astNode->tk->lineNo);
            break;

        case OUTPUT:
            if (astNode->child->label == ID) {
                ModuleTableEntry* var = searchVar(curr, astNode->child->tk->val.identifier);
                if (var == NULL)
                    printf("Error at line %d: identifier\"%s\" not recognized\n",astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
                else if (var->t.is_primitive == false)
                    printf("Error at line %d: cannot print an array\n",astNode->child->tk->lineNo);
            }
            else if (astNode->child->label = ARR_OUTPUT) {
                ModuleTableEntry* var = searchVar(curr, astNode->child->child->tk->val.identifier);
                if (var == NULL)
                    printf("Error at line %d: identifier\"%s\" not recognized\n",astNode->child->child->tk->lineNo, astNode->child->child->tk->val.identifier);
                else if (var->t.is_primitive == true)
                    printf("Error at line %d: %s is not an array\n",astNode->child->child->tk->lineNo, astNode->child->child->tk->val.identifier);
                else {
                    ASTNode* index = astNode->child->child->sibling;
                    if (index == UNARY_MINUS) {
                        if (index->child->label == NUM && var->t.lower_bound != INT_MIN && var->t.upper_bound != INT_MIN) {
                            int num = (-1) * index->child->tk->val.integer;
                            if (num < var->t.lower_bound || num > var->t.upper_bound)
                                printf("Error at line %d: Array index out of bounds\n",index->child->tk->lineNo);
                        }
                        else {
                            ModuleTableEntry* arr_ind = searchVar(curr, index->child->tk->val.identifier);
                            if (arr_ind == NULL)
                                printf("Error at line %d: indentifier \"%s\" not recognized\n", index->child->tk->lineNo, index->child->tk->val.identifier);
                            else if (arr_ind->t.pt != INTEGER || arr_ind->t.is_primitive != true)
                                printf("Error at line %d: array index must be an integer", index->child->tk->lineNo);
                        }
                    }
                    else if (index == UNARY_PLUS) {
                        if (index->child->label == NUM && var->t.lower_bound != INT_MIN && var->t.upper_bound != INT_MIN) {
                            int num = index->child->tk->val.integer;
                            if (num < var->t.lower_bound || num > var->t.upper_bound)
                                printf("Error at line %d: Array index out of bounds\n",index->child->tk->lineNo);
                        }
                        else {
                            ModuleTableEntry* arr_ind = searchVar(curr, index->child->tk->val.identifier);
                            if (arr_ind == NULL)
                                printf("Error at line %d: indentifier \"%s\" not recognized\n", index->child->tk->lineNo, index->child->tk->val.identifier);
                            else if (arr_ind->t.pt != INTEGER || arr_ind->t.is_primitive != true)
                                printf("Error at line %d: array index must be an integer", index->child->tk->lineNo);
                        }
                    }
                    else if (index->label == NUM && (index->tk->val.integer < var->t.lower_bound || index->tk->val.integer > var->t.upper_bound))
                        printf("Error at line %d: Array index out of bounds\n",index->tk->lineNo);
                    else {
                        ModuleTableEntry* arr_ind = searchVar(curr, index->tk->val.identifier);
                        if (arr_ind == NULL)
                            printf("Error at line %d: indentifier \"%s\" not recognized\n", index->tk->lineNo, index->tk->val.identifier);
                        else if (arr_ind->t.pt != INTEGER || arr_ind->t.is_primitive != true)
                            printf("Error at line %d: array index must be an integer", index->tk->lineNo);
                    }
                }
            }
            break;

        case ASSIGN: //case assign
            ASTNode* rhs = astNode->child->sibling;
            if(astNode->child->label==ARR_ASSIGN){ //case arr assign
                ASTNode* lhs = astNode->child->child;
                if(searchVar(curr,lhs->tk->val.identifier)==NULL){
                    //Error:variable not found
                }
                else if(searchVar(curr,lhs->tk->val.identifier)->t.pt!=rhs->type.pt){
                    //Type Error:types don't match at line no. lhs->tk->lineno.
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


        case DECLARE: //case declare
            typechecker(astNode->child->sibling);
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

        case UNARY_PLUS:
            typechecker(astNode->child);
            astNode->type = astNode->child->type;
            break;
        
        case UNARY_MINUS:
            typechecker(astNode->child);
            astNode->type = astNode->child->type;
            break;

        case ID:
            if(searchVar(curr,astNode->tk->val.identifier)==NULL){
                //Error:variable not found
                //astNode->type = error;
            }
            else{
                astNode->type = searchVar(curr,astNode->tk->val.identifier)->t;
            }
            break;

        case NUM:
            astNode->type.is_primitive = true;
            astNode->type.pt = INTEGER;
            break;

        case RNUM:
            astNode->type.is_primitive = true;
            astNode->type.pt = REAL;
            break;

        case TRUE:
            astNode->type.is_primitive = true;
            astNode->type.pt = BOOLEAN;
            break;

        case FALSE:
            astNode->type.is_primitive = true;
            astNode->type.pt = BOOLEAN;
            break;
        
        case ARRAY:
            typechecker(astNode->child->sibling->child);
            astNode->type.is_primitive = false;
            ModuleTableEntry* var = searchVar(curr, astNode->child->tk->val.identifier);
            if(var==NULL){
                printf("Error at line %d: Array not declared\n",astNode->child->tk->lineNo);
            }
            else if(astNode->child->sibling->child->type.pt!=INTEGER){
                printf("Error at line %d: Array index can only be of integer type\n",astNode->child->tk->lineNo);
            }
            else{
                astNode->type.pt = var->t.pt;
            }
            break;

        case PLUS:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in addition\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in addition\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==BOOLEAN || right_op->type.pt==BOOLEAN){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in addition\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in addition\n",right_op->tk->lineNo);
            }
            else if(left_op->type.pt!=BOOLEAN && left_op->type.pt!=ERROR && compare_Datatype(left_op->type,right_op->type)==true){
                astNode->type = left_op->type;
            }
            else if(left_op->type.pt==ERROR || right_op->type.pt==ERROR){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in addition\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in addition\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in addition\n");
                }
            }
            break;

        case MINUS:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in subtraction\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in subtraction\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==BOOLEAN || right_op->type.pt==BOOLEAN){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in subtraction\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in subtraction\n",right_op->tk->lineNo);
            }
            else if(left_op->type.pt!=BOOLEAN && left_op->type.pt!=ERROR && compare_Datatype(left_op->type,right_op->type)==true){
                astNode->type = left_op->type;
            }
            else if(left_op->type.pt==ERROR || right_op->type.pt==ERROR){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in subtraction\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in subtraction\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in subtraction\n");
                }
            }
            break;

        case MUL:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in multiplication\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in multiplication\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==BOOLEAN || right_op->type.pt==BOOLEAN){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in multiplication\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in multiplication\n",right_op->tk->lineNo);
            }
            else if(left_op->type.pt!=BOOLEAN && left_op->type.pt!=ERROR && compare_Datatype(left_op->type,right_op->type)==true){
                astNode->type = left_op->type;
            }
            else if(left_op->type.pt==ERROR || right_op->type.pt==ERROR){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in multiplication\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in multiplication\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in multiplication\n");
                }
            }
            break;

        case DIV:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in division\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in division\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==BOOLEAN || right_op->type.pt==BOOLEAN){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in division\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Boolean operand found in division\n",right_op->tk->lineNo);
            }
            else if(compare_Datatype(left_op->type,right_op->type)==true){
                astNode->type = left_op->type;
            }
            else if((left_op->type.pt==INTEGER && right_op->type.pt==REAL)||(right_op->type.pt==INTEGER && left_op->type.pt==REAL)){
                astNode->type.is_primitive = true;
                astNode->type.pt = REAL;
            }
            else if(left_op->type.pt==ERROR || right_op->type.pt==ERROR){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in division\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in division\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in division\n");
                }
            }
            break;

        case AND:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in logical operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in logical operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt!=BOOLEAN || right_op->type.pt!=BOOLEAN){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected both operands of boolean type in logical operation\n");
                }
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            break;

        case OR:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in logical operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in logical operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt!=BOOLEAN || right_op->type.pt!=BOOLEAN){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected both operands of boolean type in logical operation\n");
                }
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            break;
            
        case LT:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==INTEGER && right_op->type.pt==INTEGER){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else if(left_op->type.pt==REAL && right_op->type.pt==REAL){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in relational operation\n");
                }
            }
            break;
            
        case LE:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==INTEGER && right_op->type.pt==INTEGER){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else if(left_op->type.pt==REAL && right_op->type.pt==REAL){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in relational operation\n");
                }
            }
            break;
            

        case GT:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==INTEGER && right_op->type.pt==INTEGER){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else if(left_op->type.pt==REAL && right_op->type.pt==REAL){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in relational operation\n");
                }
            }
            break;

        case GE:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==INTEGER && right_op->type.pt==INTEGER){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else if(left_op->type.pt==REAL && right_op->type.pt==REAL){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in relational operation\n");
                }
            }
            break;

        case EQ:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==INTEGER && right_op->type.pt==INTEGER){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else if(left_op->type.pt==REAL && right_op->type.pt==REAL){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in relational operation\n");
                }
            }
            break;

        case NE:
            typechecker(astNode->child);
            typechecker(astNode->child->sibling);
            ASTNode* left_op = astNode->child;
            ASTNode* right_op = astNode->child->sibling;
            if(left_op->type.is_primitive==false || right_op->type.is_primitive==false){
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",left_op->child->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Array operand found in relational operation\n",right_op->child->tk->lineNo);
            }
            else if(left_op->type.pt==INTEGER && right_op->type.pt==INTEGER){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else if(left_op->type.pt==REAL && right_op->type.pt==REAL){
                astNode->type.is_primitive = true;
                astNode->type.pt = BOOLEAN;
            }
            else{
                astNode->type.is_primitive = true;
                astNode->type.pt = ERROR;
                if(left_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",left_op->tk->lineNo);
                else if(right_op->tk!=NULL)
                    printf("Type Error at line %d: Expected operands of similar type in relational operation\n",right_op->tk->lineNo);
                else{
                    printf("Expected operands of similar type in relational operation\n");
                }
            }
            break;
        
        case RANGE_WHILE:
            ASTNode* current = astNode->child;
            for (ASTNode* current = astNode->child; current != NULL; current = current -> sibling) typechecker(current);
            if(astNode->child->type.pt!=BOOLEAN || astNode->child->type.is_primitive!=1){
                printf("TYPE ERROR: line:= %d, Module \"%s\" has already been defined\n",astNode->tk->lineNo, astNode->tk->val.identifier);
            } 
            break;
        
        case RANGE_FOR:
            for (ASTNode* current = astNode->child; current != NULL; current = current -> sibling)
                typechecker(current);
            break;
    }
}