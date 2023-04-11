#include "symbolTableDef.h"
#include "hash.h"
#include "typeChecker.h"

//size of hash table??

MainTableEntry **SymbolTable;
MainTableEntry *curr; // set curr at every module node
int offset = 0;

char* arr[] = {"PROGRAM","ITER_FOR","MODULEDECLARATIONS","OTHERMODULES1","OTHERMODULES2","UNARY_PLUS","UNARY_MINUS", "ID", "NUM", "RNUM", "ARRAY_DTYPE", "ARRAY","ARRAY_RANGE","ARR_INDEX1", "ARR_INDEX2", "PLUS", "MINUS", "MUL", "DIV", "AND", "OR", "LT", "LE", "GT", "GE", "EQ", "NE", "MODULEDECLARATION", "DRIVERMODULE","MODULE_REUSE", "MODULE", "RET", "PARAMETER", "INTEGER_", "REAL_", "BOOLEAN_", "RANGE_WHILE","RANGE_FOR", "STATEMENTS", "INPUT", "OUTPUT", "ARR_OUTPUT", "TRUE", "FALSE", "ASSIGN", "ARR_ASSIGN", "INDEX_ARR", "DECLARE", "ID_LIST", "CASE","CASE_STMT","RANGE", "INPUT_PLIST", "OUTPUT_PLIST","DEFAULT"};
char* prim_type_arr[] =  {"INTEGER", "REAL", "BOOLEAN", "Semantic Error"} ;

char* data_type_arr[] =  {"ARRAY_STATIC", "ARRAY_DYNAMIC", "PRIMITIVE"} ;

void printSymbolTable() {
    ParamList* current = NULL;
    ModuleTableEntry** currModule;
    for (int i = 0; i < 40; i ++) {
        if (SymbolTable[i] != NULL) {
            printf("\nMODULE: %s\n", SymbolTable[i]->module_name);
            printf("Input Plist:\n");
            for (current = SymbolTable[i]->inputList; current != NULL; current = current->next){
                printf("Name: %s, DataType: %s, PrimType = %s", current->identifier, data_type_arr[current->type.datatype], prim_type_arr[current->type.primtype]);
                if (current->type.datatype == ARRAY_STATIC)
                    printf(", LB = %d, UB = %d",current->type.lower_bound.static_bound, current->type.upper_bound.static_bound);
                if (current->type.datatype == ARRAY_DYNAMIC){
                    if(current->type.lb_static==true){
                        printf(", LB = %d, UB = %s",current->type.lower_bound.static_bound, current->type.upper_bound.dynamic_bound);
                    }
                    else{
                        printf(", LB = %s, UB = %d",current->type.lower_bound.dynamic_bound, current->type.upper_bound.static_bound);
                    }
                }
                printf("\n");
            }
            printf("Output Plist:\n");
            for (current = SymbolTable[i]->outputList; current != NULL; current = current->next){
                printf("Name: %s, DataType: %s, PrimType = %s", current->identifier, data_type_arr[current->type.datatype], prim_type_arr[current->type.primtype]);
                if (current->type.datatype == ARRAY_STATIC)
                    printf(", LB = %d, UB = %d",current->type.lower_bound.static_bound, current->type.upper_bound.static_bound);
                if (current->type.datatype == ARRAY_DYNAMIC){
                    if(current->type.lb_static==true){
                        printf(", LB = %d, UB = %s",current->type.lower_bound.static_bound, current->type.upper_bound.dynamic_bound);
                    }
                    else{
                        printf(", LB = %s, UB = %d",current->type.lower_bound.dynamic_bound, current->type.upper_bound.static_bound);
                    }
                }
                printf("\n");
            }
            printf("\nModule's symbol table\n");
            currModule = SymbolTable[i]->moduleTable;
            printf("Name    Scope_begin    Scope_end    DataType      PrimitiveType    LowerBound     UpperBound    Offset    Width    NestingLvl\n");
            for (int j = 0; j < 40; j ++) {
                if (currModule[j] != NULL) {
                    if(currModule[j]->type.datatype==ARRAY_DYNAMIC){
                        if(currModule[j]->type.lb_static==true)
                            printf("%-12s %-12d %-9d %-16s %-13s %-14d %-16s %-8d %-10d %d\n",currModule[j]->identifier,currModule[j]->scope_begin,currModule[j]->scope_end,data_type_arr[currModule[j]->type.datatype],prim_type_arr[currModule[j]->type.primtype],currModule[j]->type.lower_bound.static_bound,currModule[j]->type.upper_bound.dynamic_bound,currModule[j]->offset,currModule[j]->width,currModule[j]->nesting_lvl);
                        else    
                            printf("%-12s %-12d %-9d %-16s %-13s %-14s %-16d %-8d %-10d %d\n",currModule[j]->identifier,currModule[j]->scope_begin,currModule[j]->scope_end,data_type_arr[currModule[j]->type.datatype],prim_type_arr[currModule[j]->type.primtype],currModule[j]->type.lower_bound.dynamic_bound,currModule[j]->type.upper_bound.static_bound,currModule[j]->offset,currModule[j]->width,currModule[j]->nesting_lvl);
                    }
                    else
                        printf("%-12s %-12d %-9d %-16s %-13s %-14d %-16d %-8d %-10d %d\n",currModule[j]->identifier,currModule[j]->scope_begin,currModule[j]->scope_end,data_type_arr[currModule[j]->type.datatype],prim_type_arr[currModule[j]->type.primtype],currModule[j]->type.lower_bound.static_bound,currModule[j]->type.upper_bound.static_bound,currModule[j]->offset,currModule[j]->width,currModule[j]->nesting_lvl);
                }
            }
        }
    }
}

void insertModule(MainTableEntry* new_module) {
    int ind=0;
    for(int i=0;new_module->module_name[i]!='\0';++i){
        ind = (ind+new_module->module_name[i])%40;
    }
    while(SymbolTable[ind] != NULL) {ind++; ind %=40;}
    SymbolTable[ind] = new_module;
}

MainTableEntry* searchModule(MainTableEntry** table, char* moduleName) {
    int ind=0;
    for(int i=0;moduleName[i]!='\0';++i){
        ind = (ind+moduleName[i])%40;
    }
    while(table[ind]!=NULL){
        if(strcmp(moduleName, table[ind] -> module_name)==0) return table[ind];
        ind++;
        ind %= 40;
    }
    return NULL;
}

void insertVar(ModuleTableEntry** table, ModuleTableEntry* new_var) {
    int ind=0;
    for(int i=0;new_var->identifier[i]!='\0';++i){
        ind = (ind+new_var->identifier[i])%40;
    }
    while(table[ind] != NULL) {ind++; ind %=40;}
    table[ind] = new_var;
}

ModuleTableEntry* searchVar(ModuleTableEntry** table, char* varName, int lineNo) {
    int ind=0, bestNest = 0;
    ModuleTableEntry* newEntry = NULL;
    for(int i=0;varName[i]!='\0';++i){
        ind = (ind+varName[i])%40;
    }
    while(table[ind]!=NULL){
        if(strcmp(varName, table[ind] -> identifier)==0 && table[ind]->nesting_lvl > bestNest && table[ind]->scope_begin <= lineNo && table[ind]->scope_end >= lineNo) {
            bestNest = table[ind]->nesting_lvl;
            newEntry = table[ind];
        }
        ind++;
        ind %= 40;
    }
    return newEntry;
}

ParamList* insertLast (ParamList* head, ParamList* newNode) {
    newNode -> next = NULL;
    if (head == NULL)
        head = newNode;
    else {
        ParamList* curr = head;
        while (curr -> next != NULL)
            curr = curr -> next;
        curr -> next = newNode;
    }
    return head;
}

bool compare_Datatype(TypeInfo d1, TypeInfo d2){
    if((d1.ub_static==false && d1.lb_static==false) || (d2.ub_static==false && d2.lb_static==false)){
        return true;
    }
    if(d1.datatype==ARRAY_DYNAMIC && d2.datatype==ARRAY_DYNAMIC){
        if((d1.lb_static==true && d2.ub_static==true && d2.upper_bound.static_bound<d1.lower_bound.static_bound)||(d2.lb_static==true && d1.ub_static==true && d1.upper_bound.static_bound<d2.lower_bound.static_bound))
            return false;
        return true;
    }
    if(d1.datatype==ARRAY_DYNAMIC){
        if((d1.lb_static==true && d2.upper_bound.static_bound<d1.lower_bound.static_bound)||(d1.lb_static==true && d1.upper_bound.static_bound<d2.lower_bound.static_bound))
            return false;
        return true;
    }
    if(d2.datatype==ARRAY_DYNAMIC){
        if((d2.lb_static==true && d1.upper_bound.static_bound<d2.lower_bound.static_bound)||(d2.lb_static==true && d2.upper_bound.static_bound<d1.lower_bound.static_bound))
            return false;
        return true;
    }
    if(d1.datatype!=d2.datatype || d1.primtype!=d2.primtype || d1.lower_bound.static_bound!=d2.lower_bound.static_bound || d1.upper_bound.static_bound!=d2.upper_bound.static_bound)
        return false;
    return true;
}

void createMainTable(int size)
{
    SymbolTable = (MainTableEntry **)malloc(size * sizeof(MainTableEntry *));
    for (int i = 0; i < size; i++)
        SymbolTable[i] = NULL;
}

ModuleTableEntry **createModuleTable(int size)
{
    ModuleTableEntry **newTable = (ModuleTableEntry **)malloc(size * sizeof(ModuleTableEntry *));
    for (int i = 0; i < size; i++)
        newTable[i] = NULL;
    return newTable;
}

MainTableEntry *createModule(char *name, ParamList *inputList, ParamList *outputList, ModuleTableEntry **table)
{
    MainTableEntry *newModule = malloc(sizeof(MainTableEntry));
    newModule->module_name = name;
    newModule->inputList = inputList;
    newModule->outputList = outputList;
    newModule->moduleTable = table;
    return newModule;
}

void isChanged(int line){
    if(curr==NULL){
        return;
    }
    ParamList* itr = curr->outputList;
    int flag = 0;
    ModuleTableEntry* var;
    while(itr!=NULL){
        var = searchVar(curr->moduleTable,itr->identifier,line);
        if(var->is_changed==0){
            printf("Semantic Error at line %d: Output Parameter %s has not been modified\n",line,itr->identifier);
        }
        itr = itr->next;
    }
}

WhileCondListNode* insertWhileNode (WhileCondListNode* head, ASTNode* tree_node) {
    ModuleTableEntry* newEntry = searchVar(curr->moduleTable, tree_node->tk->val.identifier, tree_node->tk->lineNo);
    if (newEntry == NULL) return head;
    WhileCondListNode* newNode = (WhileCondListNode*)malloc(sizeof(WhileCondListNode));
    newNode->var = newEntry;
    newNode->isChangedBeforeWhile = newNode->var->is_changed;
    newNode->var->is_changed = false;
    newNode->next = NULL;
    if (head == NULL) return newNode;
    WhileCondListNode* current = head;
    for (;current->next != NULL; current = current->next);
    current->next = newNode;
    return head;
}

WhileCondListNode* getWhileList (WhileCondListNode* head, ASTNode* root) {
    if (root == NULL) return head;
    if (root->label == ID)
        head = insertWhileNode(head, root);
    for (ASTNode* i = root->child; i != NULL; i = i->sibling)
        getWhileList(head,i);
    return head;
}

void printAllArrays(){
    printf("\nModuleName         Scope     ArrayName       Static/Dynamic        Range              Element Type\n");
    ModuleTableEntry** currModule;
    for (int i = 0; i < 40; i ++){
        if (SymbolTable[i] != NULL){
            currModule = SymbolTable[i]->moduleTable;
            for (int j = 0; j < 40; j ++) {
                ModuleTableEntry* currVar = currModule[j];
                if (currVar != NULL && currVar->type.datatype!=PRIMITIVE){
                    if(currVar->type.datatype==ARRAY_STATIC)
                        printf("%-18s[%d-%d]\t%-13s %-20s [%d,%d]\t\t%-12s\n",SymbolTable[i]->module_name,currVar->scope_begin,currVar->scope_end,currVar->identifier,data_type_arr[currVar->type.datatype],currVar->type.lower_bound.static_bound,currVar->type.upper_bound.static_bound,prim_type_arr[currVar->type.primtype]);
                    else if(currVar->type.lb_static==true && currVar->type.ub_static==true)
                        printf("%-18s[%d-%d]\t%-13s %-20s [%s,%s]\t\t%-12s\n",SymbolTable[i]->module_name,currVar->scope_begin,currVar->scope_end,currVar->identifier,data_type_arr[currVar->type.datatype],currVar->type.lower_bound.dynamic_bound,currVar->type.upper_bound.dynamic_bound,prim_type_arr[currVar->type.primtype]);
                    else if(currVar->type.ub_static==true)
                        printf("%-18s[%d-%d]\t%-13s %-20s [%s,%d]\t\t%-12s\n",SymbolTable[i]->module_name,currVar->scope_begin,currVar->scope_end,currVar->identifier,data_type_arr[currVar->type.datatype],currVar->type.lower_bound.dynamic_bound,currVar->type.upper_bound.static_bound,prim_type_arr[currVar->type.primtype]);
                    else    
                        printf("%-18s[%d-%d]\t%-13s %-20s [%d,%s]\t\t%-12s\n",SymbolTable[i]->module_name,currVar->scope_begin,currVar->scope_end,currVar->identifier,data_type_arr[currVar->type.datatype],currVar->type.lower_bound.static_bound,currVar->type.upper_bound.dynamic_bound,prim_type_arr[currVar->type.primtype]);
                }
            }
        }
    }
    printf("\n\n");
}

void populateIOLists() {
    ASTNode* curr_supernode = astroot->child->sibling;
    ASTNode* curr_module = NULL;
    bool found = false;
    while(curr_supernode!= NULL && !found) {
        if (curr_supernode->label == OTHERMODULES1 || curr_supernode->label == OTHERMODULES2) {
            for (curr_module = curr_supernode->child; curr_module != NULL; curr_module = curr_module->sibling) {
                if (strcmp(curr_module->child->tk->val.identifier,curr->module_name) == 0) {
                    found = true;
                    ASTNode *current = curr_module->child->sibling;
                    while (current != NULL)
                    {
                        if (current->label == INPUT_PLIST)
                        {
                            ASTNode *parameter = current->child;
                            ParamList *input_plist = NULL;
                            while (parameter != NULL)
                            {
                                ParamList* currentParam = input_plist;
                                while(currentParam!= NULL){
                                    if (strcmp(currentParam->identifier,parameter->tk->val.identifier) == 0)
                                        break;
                                    currentParam = currentParam -> next;
                                }
                                if (currentParam != NULL) {
                                    printf("Semantic Error at line %d: Identifier %s has already been declared\n", parameter->tk->lineNo, parameter->tk->val.identifier);
                                    compile_error = true;
                                }
                                else {
                                    ParamList *newnode = (ParamList *)malloc(sizeof(ParamList));
                                    newnode->identifier = parameter->tk->val.identifier;
                                    typeChecker(parameter->child);
                                    newnode->type = parameter->child->type;
                                    newnode->next = NULL;
                                    input_plist = insertLast(input_plist, newnode);
                                }
                                parameter = parameter->sibling;
                            }
                            curr->inputList = input_plist;
                        }
                        else if (current->label == OUTPUT_PLIST)
                        {
                            ASTNode *parameter = current->child;
                            ParamList *output_plist = NULL;
                            while (parameter != NULL)
                            {
                                ParamList* currentParam = curr->inputList;
                                while(currentParam!= NULL){
                                    if (strcmp(currentParam->identifier,parameter->tk->val.identifier) == 0)
                                        break;
                                    currentParam = currentParam -> next;
                                }
                                if (currentParam != NULL) {
                                    printf("Semantic Error at line %d: Identifier %s has already been declared\n", parameter->tk->lineNo, parameter->tk->val.identifier);
                                    compile_error = true;
                                }
                                else {
                                    currentParam = output_plist;
                                    while(currentParam!= NULL){
                                        if (strcmp(currentParam->identifier,parameter->tk->val.identifier) == 0)
                                            break;
                                        currentParam = currentParam -> next;
                                    }
                                    if (currentParam != NULL) {
                                        printf("Semantic Error at line %d: Identifier %s has already been declared\n", parameter->tk->lineNo, parameter->tk->val.identifier);
                                        compile_error = true;
                                    }
                                    else {
                                        ParamList *newnode = (ParamList *)malloc(sizeof(ParamList));
                                        newnode->identifier = parameter->tk->val.identifier;
                                        typeChecker(parameter->child);
                                        newnode->type = parameter->child->type;
                                        newnode->next = NULL;
                                        output_plist = insertLast(output_plist, newnode);
                                    }
                                }
                                parameter = parameter->sibling;
                            }
                            curr->outputList = output_plist;
                        }
                        current = current->sibling;
                    }

                    break;
                }
            }
        }

        curr_supernode = curr_supernode->sibling;
    }
    if (!found) {
        free(curr);
        curr = NULL;
    }
    printSymbolTable();
}

void typeChecker(ASTNode *astNode)
{
    if (astNode == NULL)
        return;
    int c = astNode->label;
    ASTNode *current = NULL;
    MainTableEntry *searched = NULL;
    ModuleTableEntry *newEntry = NULL;
    ModuleTableEntry *searchedVar = NULL;
    ASTNode *left_op = NULL;
    ASTNode *right_op = NULL;
    switch (c)
    {
    case PROGRAM:
        createMainTable(40);
        curr = NULL;
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typeChecker(current);
        break;

    case MODULEDECLARATIONS:
        for (ASTNode *curr = astNode->child; curr != NULL; curr = curr->sibling)
            typeChecker(curr);
        break;

    case OTHERMODULES1:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typeChecker(current);
        break;

    case OTHERMODULES2:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typeChecker(current);
        break;

    case MODULEDECLARATION:
        if (searchModule(SymbolTable, astNode->tk->val.identifier)) {
            printf("Semantic Error at line %d: Module %s has already been declared\n", astNode->tk->lineNo, astNode->tk->val.identifier);
            compile_error = true;
        }
        else {
            curr = createModule(astNode->tk->val.identifier, NULL, NULL, NULL);
            insertModule(curr);
            populateIOLists();
        }
        break;

    case DRIVERMODULE:
        curr = createModule("driver", NULL, NULL, createModuleTable(40));
        insertModule(curr);
        for (ASTNode *stmt = astNode->child->child; stmt != NULL; stmt = stmt->sibling)
            typeChecker(stmt);
        isChanged(astNode->scope_end);
        break;

    case MODULE:
        searched = searchModule(SymbolTable, astNode->child->tk->val.identifier);
        if (searched != NULL && searched->moduleTable != NULL) {
            printf("Semantic Error at line %d: Module %s has already been defined\n", astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
            compile_error = true;
        }
        else
        {
            curr = createModule(astNode->child->tk->val.identifier, NULL, NULL, createModuleTable(40));
            insertModule(curr);
            ASTNode *current = astNode->child->sibling;
            while (current->label != STATEMENTS)
            {
                if (current->label == INPUT_PLIST)
                {
                    ASTNode *parameter = current->child;
                    curr->inputList = NULL;
                    while (parameter != NULL)
                    {
                        ParamList* currentParam = curr->inputList;
                        while(currentParam!= NULL){
                            if (strcmp(currentParam->identifier,parameter->tk->val.identifier) == 0)
                                break;
                            currentParam = currentParam -> next;
                        }
                        if (currentParam != NULL) {
                            printf("Semantic Error at line %d: Identifier %s has already been declared\n", parameter->tk->lineNo, parameter->tk->val.identifier);
                            compile_error = true;
                        }
                        else {
                            ParamList *newnode = (ParamList *)malloc(sizeof(ParamList));
                            newnode->identifier = parameter->tk->val.identifier;
                            typeChecker(parameter->child);
                            newnode->type = parameter->child->type;
                            newnode->next = NULL;
                            curr->inputList = insertLast(curr->inputList, newnode);
                            ModuleTableEntry *newEntry = (ModuleTableEntry*)malloc(sizeof(ModuleTableEntry));
                            newEntry->identifier = newnode->identifier;
                            newEntry->nesting_lvl = 1;
                            newEntry->offset = offset;
                            newEntry->scope_begin = parameter->scope_begin;
                            newEntry->scope_end = parameter->scope_end;
                            newEntry->type = newnode->type;
                            newEntry->is_changed = false;
                            newEntry->vartype = INPUT_VAR;
                            if (newEntry->type.primtype == BOOLEAN)
                                newEntry->width = 1;
                            else if (newEntry->type.primtype == INTEGER)
                                newEntry->width = 2; 
                            else
                                newEntry->width = 4;
                            if (newEntry->type.datatype == ARRAY_STATIC)
                                newEntry->width = (newEntry->width * (newEntry->type.upper_bound.static_bound - newEntry->type.lower_bound.static_bound + 1)) + 1;
                            else if (newEntry->type.datatype == ARRAY_DYNAMIC)
                                newEntry->width = 5;
                            insertVar(curr->moduleTable, newEntry);
                            offset+=newEntry->width;
                        }
                        parameter = parameter->sibling;
                    }
                }
                else
                {
                    ASTNode *parameter = current->child;
                    curr->outputList = NULL;
                    while (parameter != NULL)
                    {
                        ParamList* currentParam = curr->inputList;
                        while(currentParam!= NULL){
                            if (strcmp(currentParam->identifier,parameter->tk->val.identifier) == 0)
                                break;
                            currentParam = currentParam -> next;
                        }
                        if (currentParam != NULL) {
                            printf("Semantic Error at line %d: Identifier %s has already been declared\n", parameter->tk->lineNo, parameter->tk->val.identifier);
                            compile_error = true;
                        }
                        else {
                            currentParam = curr->outputList;
                            while(currentParam!= NULL){
                                if (strcmp(currentParam->identifier,parameter->tk->val.identifier) == 0)
                                    break;
                                currentParam = currentParam -> next;
                            }
                            if (currentParam != NULL) {
                                printf("Semantic Error at line %d: Identifier %s has already been declared\n", parameter->tk->lineNo, parameter->tk->val.identifier);
                                compile_error = true;
                            }
                            else {
                                ParamList *newnode = (ParamList *)malloc(sizeof(ParamList));
                                newnode->identifier = parameter->tk->val.identifier;
                                typeChecker(parameter->child);
                                newnode->type = parameter->child->type;
                                newnode->next = NULL;
                                curr->outputList = insertLast(curr->outputList, newnode);
                                ModuleTableEntry *newEntry = (ModuleTableEntry*)malloc(sizeof(ModuleTableEntry));
                                newEntry->identifier = newnode->identifier;
                                newEntry->nesting_lvl = 1;
                                newEntry->offset = offset;
                                newEntry->scope_begin = parameter->scope_begin;
                                newEntry->scope_end = parameter->scope_end;
                                newEntry->type = newnode->type;
                                newEntry->is_changed = false;
                                newEntry->vartype = OUTPUT_VAR;
                                if (newEntry->type.primtype == BOOLEAN)
                                    newEntry->width = 1;
                                else if (newEntry->type.primtype == INTEGER)
                                    newEntry->width = 2;
                                else
                                    newEntry->width = 4;
                                insertVar(curr->moduleTable, newEntry);
                                offset+=newEntry->width;
                            }
                        }
                        parameter = parameter->sibling;
                    }
                }
                current = current->sibling;
            }
            if (current != NULL) {
                for (current = current->child; current != NULL; current = current->sibling)
                    typeChecker(current);
            }
            isChanged(astNode->scope_end);
        }
        break;

    case INTEGER_:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = INTEGER;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        break;

    case REAL_:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = REAL;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        break;

    case BOOLEAN_:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = BOOLEAN;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        break;

    case ARRAY_DTYPE:
        typeChecker(astNode->child->sibling);
        astNode->type.primtype = astNode->child->sibling->type.primtype;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        astNode->type.datatype = ARRAY_STATIC;
        ASTNode *left = astNode->child->child->child;
        ASTNode *right = astNode->child->child->sibling->child;
        if (left->label == UNARY_MINUS) {
            if (left->child->label == NUM){
                astNode->type.lower_bound.static_bound = (-1) * left->child->tk->val.integer;
                astNode->type.lb_static = true;
            }
            else {
                astNode->type.lb_static = false;
                astNode->type.datatype = ARRAY_DYNAMIC;
                typeChecker(left->child);
                astNode->type.lower_bound.dynamic_bound = left->child->tk->val.identifier;
                if (left->child->type.datatype != PRIMITIVE || left->child->type.primtype != INTEGER){
                    printf("Semantic Error at line %d: Array index has to be an integer\n",left->child->tk->lineNo);
                    compile_error = true;
                }
            }
        }
        else if (left->label == UNARY_PLUS) {
            if (left->child->label == NUM){
                astNode->type.lower_bound.static_bound = left->child->tk->val.integer;
                astNode->type.lb_static = true;
            }
            else {
                astNode->type.lb_static = false;
                astNode->type.datatype = ARRAY_DYNAMIC;
                typeChecker(left->child);
                astNode->type.lower_bound.dynamic_bound = left->child->tk->val.identifier;
                if (left->child->type.datatype != PRIMITIVE || left->child->type.primtype != INTEGER){
                    printf("Semantic Error at line %d: Array index has to be an integer\n",left->child->tk->lineNo);
                    compile_error = true;
                }
            }
        }
        else if (left->label == NUM){
            astNode->type.lb_static = true;
            astNode->type.lower_bound.static_bound = left->tk->val.integer;
        }
        else {
            astNode->type.lb_static = false;
            astNode->type.datatype = ARRAY_DYNAMIC;
            typeChecker(left);
            astNode->type.lower_bound.dynamic_bound = left->tk->val.identifier;
            if (left->type.datatype != PRIMITIVE || left->type.primtype != INTEGER){
                printf("Semantic Error at line %d: Array index has to be an integer\n",left->child->tk->lineNo);
                compile_error = true;
            }
        }
        if (right->label == UNARY_MINUS){
            if (right->child->label == NUM){
                astNode->type.upper_bound.static_bound = (-1) * right->child->tk->val.integer;
                astNode->type.ub_static = true;
            }
            else {
                astNode->type.ub_static = false;
                astNode->type.datatype = ARRAY_DYNAMIC;
                typeChecker(right->child);
                astNode->type.upper_bound.dynamic_bound = right->child->tk->val.identifier;
                if (right->child->type.datatype != PRIMITIVE || right->child->type.primtype != INTEGER){
                    printf("Semantic Error at line %d: Array index has to be an integer\n",right->child->tk->lineNo);
                    compile_error = true;
                }
            }
        }
        else if (right->label == UNARY_PLUS){
            if (right->child->label == NUM){
                astNode->type.upper_bound.static_bound = right->child->tk->val.integer;
                astNode->type.ub_static = true;
            }
            else {
                astNode->type.ub_static = false;
                astNode->type.datatype = ARRAY_DYNAMIC;
                typeChecker(right->child);
                astNode->type.upper_bound.dynamic_bound = right->child->tk->val.identifier;
                if (right->child->type.datatype != PRIMITIVE || right->child->type.primtype != INTEGER){
                    printf("Semantic Error at line %d: Array index has to be an integer\n",right->child->tk->lineNo);
                    compile_error = true;
                }
            }
        }
        else if (right->label == NUM){
            astNode->type.upper_bound.static_bound = right->tk->val.integer;
            astNode->type.ub_static = true;
        }
        else {
            astNode->type.ub_static = false;
            astNode->type.datatype = ARRAY_DYNAMIC;
            typeChecker(right);
            astNode->type.upper_bound.dynamic_bound = right->tk->val.identifier;
            if (right->type.datatype != PRIMITIVE || right->type.primtype != INTEGER){
                printf("Semantic Error at line %d: Array index has to be an integer\n", right->tk->lineNo);
                compile_error = true;
            }
        }
        if (astNode->type.upper_bound.static_bound!= -1e9 && astNode->type.lower_bound.static_bound != -1e9 && astNode->type.upper_bound.static_bound < astNode->type.lower_bound.static_bound){
            printf("Semantic Error: lower bound of array should be less than or equal to upper bound\n");
            compile_error = true;
        }
        break;

    case INPUT:
        newEntry = searchVar(curr->moduleTable, astNode->tk->val.identifier, astNode->tk->lineNo);
        if (newEntry == NULL){
            printf("Semantic Error at line %d: variable %s not declared in this scope\n", astNode->tk->lineNo, astNode->tk->val.identifier);
            compile_error = true;
        }
        else if (newEntry->vartype == FOR_LOOP_VAR){
            printf("Semantic Error at lien %d: cannot modify for loop variable\n", astNode->tk->lineNo);
            compile_error = true;
        }
        else
            newEntry->is_changed = true;
        break;

    case OUTPUT:
        if (astNode->child->label == ID)
        {
            ModuleTableEntry *newEntry = searchVar(curr->moduleTable, astNode->child->tk->val.identifier, astNode->child->tk->lineNo);
            if (newEntry == NULL){
                printf("Semantic Error at line %d: variable %s has not been declared in this scope\n", astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
                compile_error = true;
            }
            else if (newEntry->type.datatype != PRIMITIVE){
                compile_error = true;
                printf("Semantic Error at line %d: cannot print an array\n", astNode->child->tk->lineNo);
            }
        }
        else if (astNode->child->label == ARR_OUTPUT)
        {
            ModuleTableEntry *newEntry = searchVar(curr->moduleTable, astNode->child->child->tk->val.identifier, astNode->child->child->tk->lineNo);
            if (newEntry == NULL){
                printf("Semantic Error at line %d: variable %s vas not been declared in this scope\n", astNode->child->child->tk->lineNo, astNode->child->child->tk->val.identifier);
                compile_error = true;
            }
            else if (newEntry->type.datatype == PRIMITIVE){
                printf("Semantic Error at line %d: %s is not an array\n", astNode->child->child->tk->lineNo, astNode->child->child->tk->val.identifier);
                compile_error = true;
            }
            else
            {
                ASTNode *index = astNode->child->child->sibling->child;
                if (index->label == UNARY_MINUS)
                {
                    if (index->child->label == NUM && newEntry->type.datatype == ARRAY_STATIC)
                    {
                        int num = (-1) * index->child->tk->val.integer;
                        if (num < newEntry->type.lower_bound.static_bound || num > newEntry->type.upper_bound.static_bound){
                            printf("Semantic Error at line %d: Array index out of bounds\n", index->child->tk->lineNo);
                            compile_error = true;
                        }
                    }
                    else if (index->child->label == ID)
                    {
                        ModuleTableEntry *arr_ind = searchVar(curr->moduleTable, index->child->tk->val.identifier, index->child->tk->lineNo);
                        if (arr_ind == NULL){
                            printf("Semantic Error at line %d: variable %s has not been declared in this scope\n", index->child->tk->lineNo, index->child->tk->val.identifier);
                            compile_error = true;
                        }
                        else if (arr_ind->type.primtype != INTEGER || arr_ind->type.datatype != PRIMITIVE){
                            printf("Semantic Error at line %d: array index must be an integer", index->child->tk->lineNo);
                            compile_error = true;
                        }
                    }
                }
                else if (index->label == UNARY_PLUS)
                {
                    if (index->child->label == NUM && newEntry->type.datatype == ARRAY_STATIC)
                    {
                        int num = index->child->tk->val.integer;
                        if (num < newEntry->type.lower_bound.static_bound || num > newEntry->type.upper_bound.static_bound){
                            printf("Semantic Error at line %d: Array index out of bounds\n", index->child->tk->lineNo);
                            compile_error = true;
                        }
                    }
                    else if (index->child->label == ID)
                    {
                        ModuleTableEntry *arr_ind = searchVar(curr->moduleTable, index->child->tk->val.identifier, index->child->tk->lineNo);
                        if (arr_ind == NULL){
                            printf("Semantic Error at line %d: variable %s has not been declared in this scope\n", index->child->tk->lineNo, index->child->tk->val.identifier);
                            compile_error = true;
                        }
                        else if (arr_ind->type.primtype != INTEGER || arr_ind->type.datatype != PRIMITIVE){
                            printf("Semantic Error at line %d: array index must be an integer", index->child->tk->lineNo);
                            compile_error = true;
                        }
                    }
                }
                else if (index->label == NUM) {
                    if (newEntry->type.datatype == ARRAY_STATIC && (index->tk->val.integer < newEntry->type.lower_bound.static_bound || index->tk->val.integer > newEntry->type.upper_bound.static_bound)){
                        printf("Semantic Error at line %d: Array index out of bounds\n", index->tk->lineNo);
                        compile_error = true;
                    }
                }
                else
                {
                    ModuleTableEntry *arr_ind = searchVar(curr->moduleTable, index->tk->val.identifier, index->tk->lineNo);
                    if (arr_ind == NULL){
                        printf("Semantic Error at line %d: variable %s has not been declared in this scope\n", index->tk->lineNo, index->tk->val.identifier);
                        compile_error = true;
                    }
                    else if (arr_ind->type.primtype != INTEGER || arr_ind->type.datatype != PRIMITIVE){
                        printf("Semantic Error at line %d: array index must be an integer", index->tk->lineNo);
                        compile_error = true;
                    }
                }
            }
        }
        break;

    case ASSIGN:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        TypeInfo t1 = astNode->child->type;
        TypeInfo t2 = astNode->child->sibling->type;
        // printf("\n%d",t1.datatype);
        // printf("\n%d",t1.datatype);
        if(searchVar(curr->moduleTable,astNode->child->tk->val.identifier,astNode->child->tk->lineNo)!=NULL){
            searchVar(curr->moduleTable,astNode->child->tk->val.identifier,astNode->child->tk->lineNo)->is_changed = true;
        }
        if (t1.primtype == ERROR || t2.primtype == ERROR){
            // printf("hi");
            break;
        }

        else if (t1.datatype != t2.datatype || t1.primtype != t2.primtype)
        {
            if (astNode->child->child == NULL){
                printf("Semantic Error at line %d: Operand types don't match in assignment operation\n", astNode->child->tk->lineNo);
                compile_error = true;
            }
            else
            {
                compile_error = true;
                printf("Semantic Error at line %d: Operand types don't match in assignment operation\n", astNode->child->child->tk->lineNo);
            }
        }
        else if (t1.datatype == ARRAY_STATIC && t2.datatype == ARRAY_STATIC && t1.upper_bound.static_bound-t1.lower_bound.static_bound!=t2.upper_bound.static_bound-t2.lower_bound.static_bound)
        { 
            compile_error = true;
            printf("Semantic Error at line %d: Arrays are not structurally equivalent in assignment operation\n", astNode->child->tk->lineNo);
        }
        else if (t1.datatype == PRIMITIVE){
            newEntry = searchVar(curr->moduleTable, astNode->child->tk->val.identifier, astNode->child->tk->lineNo);
            if (newEntry->vartype == FOR_LOOP_VAR)
                {
                    compile_error = true;
                    printf("Semantic Error at line %d: For loop variable cannot be modified\n",astNode->child->tk->lineNo);
                }
            else
                newEntry->is_changed = true;
        }
        // dynamic type checking
        ModuleTableEntry* var = searchVar(curr->moduleTable,astNode->child->tk->val.identifier,astNode->child->tk->lineNo);
        if(var!=NULL){
            var->is_changed = true;
        } 
        break;

    case ARR_ASSIGN:
        typeChecker(astNode->child);
        typeChecker(astNode->child->child);
        typeChecker(astNode->child->sibling);
        Prim_type pt1 = astNode->child->type.primtype;
        Prim_type pt2 = astNode->child->sibling->type.primtype;
        Prim_type pt3 = astNode->child->child->type.primtype;
        newEntry = searchVar(curr->moduleTable,astNode->child->tk->val.identifier,astNode->child->tk->lineNo);
        if(newEntry!=NULL){
            newEntry->is_changed = true;
        }
        if(pt1==ERROR || pt2==ERROR) break;
        if(pt3!=INTEGER){
            compile_error = true;
            printf("Semantic Error at line %d: Index of array variable %s found to be of non-integer type\n",astNode->child->tk->lineNo,astNode->child->tk->val.identifier);
        }
        // else if((astNode->child->child->label==ID || astNode->child->child->label==NUM) && (astNode->child->child->tk->val.integer<astNode->child->type.lower_bound || astNode->child->child->tk->val.integer>astNode->child->type.upper_bound)){
        //     printf("Semantic Error at line %d: Array index out of bounds for array variable %s",astNode->child->tk->lineNo,astNode->child->tk->val.identifier);
        // }
        if(pt1!=pt2){
            compile_error = true;
            printf("Semantic Error at line %d: Operand types don't match in assignment operation\n", astNode->child->tk->lineNo);
        }
        ASTNode* index = astNode->child->child->child;
        if (index->label == NUM && newEntry->type.datatype == ARRAY_STATIC && (index->tk->val.integer < newEntry->type.lower_bound.static_bound || index->tk->val.integer > newEntry->type.upper_bound.static_bound))
            { compile_error=true; { compile_error=true; printf("Semantic Error at line %d: Array index out of bounds\n",index->tk->lineNo);}}
        else if (index->label == UNARY_PLUS && index->child->label == NUM && newEntry->type.datatype == ARRAY_STATIC && (index->child->tk->val.integer < newEntry->type.lower_bound.static_bound || index->child->tk->val.integer > newEntry->type.upper_bound.static_bound))
            { compile_error=true; { compile_error=true; printf("Semantic Error at line %d: Array index out of bounds\n",index->tk->lineNo);}}
        else if (index->label == UNARY_MINUS && index->child->label == NUM && newEntry->type.datatype == ARRAY_STATIC && (((-1) * index->child->tk->val.integer) <  newEntry->type.lower_bound.static_bound || ((-1) * index->child->tk->val.integer) > newEntry->type.upper_bound.static_bound))
            { compile_error=true; { compile_error=true; printf("Semantic Error at line %d: Array index out of bounds\n",index->tk->lineNo);}}
        break;

    case ARR_INDEX1:
        typeChecker(astNode->child);
        astNode->type = astNode->child->type;
        break;

    case ARR_INDEX2:
        typeChecker(astNode->child);
        astNode->type = astNode->child->type;
        break;

    case DECLARE:
        typeChecker(astNode->child->sibling);
        TypeInfo d = astNode->child->sibling->type;
        int width = 0;
        if (d.datatype == ARRAY_DYNAMIC) {
            width = 1;
        }
        else if (d.primtype == BOOLEAN)
            width = 1;
        else if (d.primtype == INTEGER)
            width = 2;
        else if (d.primtype == REAL)
            width = 4;
        else if (d.datatype == ARRAY_STATIC)
        {
            width = (d.upper_bound.static_bound - d.lower_bound.static_bound + 1) * width + 1;
        }
        ASTNode *idList = astNode->child->child;
        while (idList != NULL)
        {
            char *s = idList->tk->val.identifier;
            newEntry = searchVar(curr->moduleTable,s,idList->tk->lineNo);
            if (newEntry != NULL && newEntry->vartype == OUTPUT_VAR){
                compile_error = true;
                printf("Semantic error at line %d: Output paramater %s cannot be shadowed\n",idList->tk->lineNo, idList->tk->val.identifier);
            }
            else if(newEntry!=NULL && idList->scope_begin==newEntry->scope_begin && idList->scope_end==newEntry->scope_end){
                if (newEntry->vartype != INPUT_VAR){
                    compile_error = true;
                    printf("Semantic Error at line %d: Variable %s has already been declared in this scope\n",idList->tk->lineNo,idList->tk->val.identifier);
                }
                else {
                    newEntry->type = d;
                    newEntry->is_changed = false;
                    newEntry->vartype = NORMAL_VAR;
                    newEntry->width = width;
                    newEntry->offset = offset;
                    newEntry->nesting_lvl = idList->nest_level;
                    offset += width;
                }
            }
            else{
                ModuleTableEntry *new_entry = (ModuleTableEntry *)malloc(sizeof(ModuleTableEntry));
                new_entry->identifier = s;
                new_entry->type = d;
                new_entry->scope_begin = idList->scope_begin;
                new_entry->scope_end = idList->scope_end;
                new_entry->is_changed = false;
                new_entry->vartype = NORMAL_VAR;
                if (d.datatype != ARRAY_DYNAMIC)
                {
                    new_entry->width = width;
                }
                new_entry->offset = offset;
                new_entry->nesting_lvl = idList->nest_level;
                insertVar(curr->moduleTable, new_entry);
                offset += width;
            }
            idList = idList->sibling;
        }
        break;

    case UNARY_PLUS:
        typeChecker(astNode->child);
        astNode->type = astNode->child->type;
        break;

    case UNARY_MINUS:
        typeChecker(astNode->child);
        astNode->type = astNode->child->type;
        break;

    case ID:
        newEntry = searchVar(curr->moduleTable, astNode->tk->val.identifier, astNode->tk->lineNo);
        if (newEntry == NULL)
        {
            compile_error = true;
            printf("Semantic Error at line %d: Variable %s not declared in this scope\n", astNode->tk->lineNo, astNode->tk->val.identifier);
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        // else if (newEntry) != NULL)
        // {
        //     printf("Scope Error at line %d: Variable %s has been used out of scope\n Previous declaration is at line %d\n", astNode->tk->lineNo, astNode->tk->val.identifier, searchVar(curr->moduleTable, astNode->tk->val.identifier)->scope_begin);
        //     astNode->type.datatype = PRIMITIVE;
        //     astNode->type.primtype = ERROR;
        // }
        else
        {
            astNode->type = newEntry->type;
        }
        break;

    case NUM:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = INTEGER;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        break;

    case RNUM:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = REAL;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        break;

    case TRUE:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = BOOLEAN;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        break;

    case FALSE:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = BOOLEAN;
        astNode->type.lower_bound.static_bound = -1e9;
        astNode->type.upper_bound.static_bound = -1e9;
        break;

    case ARRAY:
        newEntry = searchVar(curr->moduleTable, astNode->child->tk->val.identifier, astNode->child->tk->lineNo);
        if (newEntry == NULL)
        {
            printf("Semantic Error at line %d: Array variable %s not declared\n", astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            compile_error = true;
        }
        else if (astNode->child->sibling->child != NULL)
        {
            typeChecker(astNode->child->sibling->child);
            if (astNode->child->sibling->child->type.datatype != PRIMITIVE || astNode->child->sibling->child->type.primtype != INTEGER)
            {
                printf("Semantic Error at line %d: Index of array variable %s has been found to be of non-integer type\n", astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
                astNode->type.datatype = PRIMITIVE;
                astNode->type.primtype = ERROR;
                compile_error = true;
            }
            else {
                astNode->type.primtype = newEntry->type.primtype;
                astNode->type.datatype = PRIMITIVE;
                astNode->type.lower_bound.static_bound = -1e9;
                astNode->type.upper_bound.static_bound = -1e9;
                if (astNode->child->sibling->child->label == NUM && newEntry->type.datatype == ARRAY_STATIC && (newEntry->type.lower_bound.static_bound > astNode->child->sibling->child->tk->val.integer || newEntry->type.upper_bound.static_bound < astNode->child->sibling->child->tk->val.integer))
                    {compile_error = true;printf("Semantic Error at line %d: Array index out of bounds\n",astNode->child->sibling->child->tk->lineNo);}
                else if (astNode->child->sibling->child->label == UNARY_MINUS && astNode->child->sibling->child->child->label == NUM && newEntry->type.datatype == ARRAY_STATIC && (newEntry->type.lower_bound.static_bound > ((-1) * astNode->child->sibling->child->child->tk->val.integer) || newEntry->type.upper_bound.static_bound < ((-1) * astNode->child->sibling->child->child->tk->val.integer)))
                    {compile_error = true;printf("Semantic Error at line %d: Array index out of bounds\n",astNode->child->sibling->child->tk->lineNo);}
                else if (astNode->child->sibling->child->label == UNARY_PLUS && astNode->child->sibling->child->child->label == NUM && newEntry->type.datatype == ARRAY_STATIC && (newEntry->type.lower_bound.static_bound > astNode->child->sibling->child->child->tk->val.integer || newEntry->type.upper_bound.static_bound < astNode->child->sibling->child->child->tk->val.integer))
                    {compile_error = true;printf("Semantic Error at line %d: Array index out of bounds\n",astNode->child->sibling->child->tk->lineNo);}
            }
        }
        else
        {
            astNode->type = newEntry->type;
        }
        break;

    case PLUS:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        Prim_type p1 = left_op->type.primtype;
        Prim_type p2 = right_op->type.primtype;
        if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in addition\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in addition\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in addition\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in addition\n", right_op->tk->lineNo);}
        }
        else if (p1==p2)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = p1;
        } 
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in addition\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in addition\n", right_op->tk->lineNo);}
            else
            {
                {compile_error = true;printf("Expected operands of similar type in addition\n");}
            }
        }
        break;

    case MINUS:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
         p1 = left_op->type.primtype;
         p2 = right_op->type.primtype;
        if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in subtraction\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in subtraction\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in subtraction\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in subtraction\n", right_op->tk->lineNo);}
        }
        else if (p1==p2)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = p1;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in subtraction\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in subtraction\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in subtraction\n");
            }
        }
        break;

    case MUL:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
         p1 = left_op->type.primtype;
         p2 = right_op->type.primtype;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in multiplication\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in multiplication\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in multiplication\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in multiplication\n", right_op->tk->lineNo);}
        }
        else if (p1==p2)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = p1;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in multiplication\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in multiplication\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in multiplication\n");
            }
        }
        break;

    case DIV:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
         p1 = left_op->type.primtype;
         p2 = right_op->type.primtype;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in division\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in division\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in division\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Boolean operand found in division\n", right_op->tk->lineNo);}
        }
        else if (p1==p2)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = REAL;
        }
        else if ((left_op->type.primtype == INTEGER && right_op->type.primtype == REAL) || (right_op->type.primtype == INTEGER && left_op->type.primtype == REAL))
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = REAL;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in division\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in division\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in division\n");
            }
        }
        break;

    case AND:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in logical operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in logical operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype != BOOLEAN || right_op->type.primtype != BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected both operands of boolean type in logical operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected both operands of boolean type in logical operation\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected both operands of boolean type in logical operation\n");
            }
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        break;

    case OR:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in logical operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in logical operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype != BOOLEAN || right_op->type.primtype != BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected both operands of boolean type in logical operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected both operands of boolean type in logical operation\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected both operands of boolean type in logical operation\n");
            }
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        break;

    case LT:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == INTEGER && right_op->type.primtype == INTEGER)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else if (left_op->type.primtype == REAL && right_op->type.primtype == REAL)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case LE:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == INTEGER && right_op->type.primtype == INTEGER)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else if (left_op->type.primtype == REAL && right_op->type.primtype == REAL)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL){
                printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);
                compile_error = true;
            }
            else if (right_op->tk != NULL){
                printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);
                compile_error = true;
            }
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case GT:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == INTEGER && right_op->type.primtype == INTEGER)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else if (left_op->type.primtype == REAL && right_op->type.primtype == REAL)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case GE:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == INTEGER && right_op->type.primtype == INTEGER)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else if (left_op->type.primtype == REAL && right_op->type.primtype == REAL)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case EQ:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == INTEGER && right_op->type.primtype == INTEGER)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else if (left_op->type.primtype == REAL && right_op->type.primtype == REAL)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case NE:
        typeChecker(astNode->child);
        typeChecker(astNode->child->sibling);
        left_op = astNode->child;
        right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Array operand found in relational operation\n", right_op->tk->lineNo);}
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (left_op->type.primtype == INTEGER && right_op->type.primtype == INTEGER)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else if (left_op->type.primtype == REAL && right_op->type.primtype == REAL)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = BOOLEAN;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);}
            else if (right_op->tk != NULL)
                {compile_error = true;printf("Semantic Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);}
            else
            {
                compile_error = true;
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case RANGE_WHILE:
        current = astNode->child;
        typeChecker(current);
        WhileCondListNode* whileIDs = NULL;
        whileIDs = getWhileList(whileIDs,current);
        if (astNode->child->type.primtype != BOOLEAN || astNode->child->type.datatype != PRIMITIVE)
        {
            compile_error = true;
            printf("Semantic Error at line %d: Condition of WHILE must be boolean\n", astNode->tk->lineNo);
        }
        current = current -> sibling;
        while (current != NULL) {
            typeChecker(current);
            current = current -> sibling;
        }
        bool modified = false;
        for (WhileCondListNode* whileNode = whileIDs; whileNode != NULL; whileNode = whileNode->next) {
            modified = modified || whileNode->var->is_changed;
            whileNode->var->is_changed = whileNode->var->is_changed || whileNode->isChangedBeforeWhile;
        }
        if (!modified)
            {
                compile_error = true;
                printf("Semantic error at line %d: At least one variable in the while loop condition must be modified within the body of the loop\n",astNode->scope_end);
            }
        break;

    case ITER_FOR:
        searchedVar = searchVar(curr->moduleTable, astNode->tk->val.identifier, astNode->tk->lineNo);
        if (searchedVar != NULL && searchedVar->vartype == OUTPUT_VAR)
            {
                compile_error = true;
                printf("Semaintic Error at line %d: Output parameter %s can never be shadowed\n",astNode->tk->lineNo, astNode->tk->val.identifier);
            }
        else {
            newEntry = (ModuleTableEntry*)malloc(sizeof(ModuleTableEntry));
            newEntry->identifier = astNode->tk->val.identifier;
            newEntry->nesting_lvl = astNode->nest_level;
            newEntry->offset = offset;
            newEntry->width = 2;
            offset += 2;
            newEntry->scope_begin = astNode->scope_begin;
            newEntry->scope_end = astNode->scope_end;
            newEntry->is_changed = false;
            newEntry->vartype = FOR_LOOP_VAR;
            newEntry->type.datatype = PRIMITIVE;
            newEntry->type.lower_bound.static_bound = -1e9;
            newEntry->type.upper_bound.static_bound = -1e9;
            newEntry->type.primtype = INTEGER;
            insertVar(curr->moduleTable, newEntry);
        }
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typeChecker(current);
        break;

    case RANGE_FOR:
        if (astNode->child->tk->type == TK_NUM)
        {
            left_op = astNode->child;
        }
        else
            left_op = astNode->child->child;
        if (astNode->child->sibling->tk->type == TK_NUM)
        {
            right_op = astNode->child->sibling;
        }
        else
            right_op = astNode->child->sibling->child;
        if (!(left_op != NULL && right_op != NULL && left_op->tk->type == right_op->tk->type && left_op->tk->type == TK_NUM))
        {
            compile_error = true;
            printf("Semantic Error: at line %d, iterator range bounds must be integer\n", astNode->tk->lineNo);
        }
        break;

    case CASE_STMT:
        newEntry = searchVar(curr->moduleTable, astNode->tk->val.identifier,astNode->tk->lineNo);
        if (newEntry == NULL)
        {
            compile_error = true;
            printf("Semantic Error: at line %d, identifier not declared previously\n", astNode->tk->lineNo);
            break;
        }
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typeChecker(current);
        break;

    case CASE:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typeChecker(current);
        break;

    case DEFAULT:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typeChecker(current);
        break;

    // <moduleReuseStmt> <optional> ID <actual_para_list>
    case MODULE_REUSE:
        searched = searchModule(SymbolTable, astNode->tk->val.identifier);
        bool flag = true;
        if (searched == NULL)
        {
            compile_error = true;
            printf("Semantic Error: at line %d, Module not found\n", astNode->tk->lineNo);
            break;
        }
        if (astNode->child->label == ASSIGN)
        {
            ASTNode *formal_out = astNode->child->child->child;
            ASTNode *formal_in = astNode->child->sibling->child;
            ParamList *real_in = searched->inputList;
            ParamList *real_out = searched->outputList;
            while(formal_out!=NULL) {
                typeChecker(formal_out);
                formal_out = formal_out->sibling;
            }
            while(formal_in!=NULL) {
                typeChecker(formal_in);
                formal_in = formal_in->sibling;
            }
            formal_out = astNode->child->child->child;
            formal_in = astNode->child->sibling->child;
            while (formal_out != NULL)
            {
                if (real_out == NULL || !compare_Datatype(formal_out->type, real_out->type))
                {
                    flag = false;
                    break;
                }
                formal_out = formal_out->sibling;
                real_out = real_out->next;
            }
            while (formal_in != NULL)
            {
                if (real_in == NULL || !compare_Datatype(formal_in->type, real_in->type))
                {
                    flag = false;
                    break;
                }
                formal_in = formal_in->sibling;
                real_in = real_in->next;
            }
        }
        else
        {
            ASTNode *actual_in = astNode->child->child;
            ParamList *formal_in = searched->inputList;
            while (formal_in != NULL)
            {
                if (actual_in == NULL || !compare_Datatype(actual_in->type, formal_in->type))
                {
                    flag = false;
                    break;
                }
                actual_in = actual_in->sibling;
                formal_in = formal_in->next;
            }
        }
        if (!flag)
        {
            compile_error = true;
            printf("Semantic Error at line %d: Module reuse parameters mismatch\n", astNode->tk->lineNo);
        }
        break;

    default:
        break;
    }
}