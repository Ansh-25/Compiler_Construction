#include "symbolTableDef.h"

MainTableEntry **SymbolTable;
MainTableEntry *curr; // set curr at every module node
int offset = 0;

void insertModule(MainTableEntry** table, MainTableEntry* new_module) {
    int ind = sumchars(new_module->module_name);
    while(table[ind] != NULL) {ind++; ind %=47;}
    table[ind] = new_module;
}

MainTableEntry* searchModule(MainTableEntry** table, char* moduleName) {
    int ind = sumchars(moduleName);
    while(table[ind]!=NULL){
        if(strcmp(moduleName, table[ind] -> module_name)==0) return table[ind];
        ind++;
        ind %= 47;
    }
    return NULL;
}

void insertVar(ModuleTableEntry** table, ModuleTableEntry* new_var) {
    int ind = sumchars(new_var->identifier);
    while(table[ind] != NULL) {ind++; ind %=47;}
    table[ind] = new_var;
}

ModuleTableEntry* searchVar(ModuleTableEntry** table, char* varName) {
    int ind = sumchars(varName);
    while(table[ind]!=NULL){
        if(strcmp(varName, table[ind] -> identifier)==0) return table[ind];
        ind++;
        ind %= 47;
    }
    return NULL;
}

ParamList* insertlast (ParamList* head, ParamList* newNode) {
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
    if(d1.datatype!=d2.datatype || d1.primtype!=d2.primtype || d1.lower_bound!=d2.lower_bound || d1.upper_bound!=d2.upper_bound)
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

MainTableEntry *createModule(char *name, ParamList *inputList, ParamList *outputList, ModuleTableEntry *table)
{
    MainTableEntry *newModule = malloc(sizeof(MainTableEntry));
    newModule->module_name = name;
    newModule->inputList = inputList;
    newModule->outputList = outputList;
    newModule->moduleTable = table;
    return newModule;
}

void typechecker(ASTNode *astNode)
{
    if (astNode == NULL)
        return;
    int c = astNode->label;
    ASTNode *current = astNode->child;
    MainTableEntry *searched = NULL;
    ModuleTableEntry *var = NULL;
    for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
        typechecker(current);
    switch (c)
    {
    case PROGRAM:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        break;

    case MODULEDECLARATIONS:
        for (ASTNode *curr = astNode->child; curr != NULL; curr = curr->sibling)
            typechecker(curr);
        break;

    case OTHERMODULES1:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        break;

    case OTHERMODULES2:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        break;

    case MODULEDECLARATION:
        if (searchModule(SymbolTable, astNode->tk->val.identifier))
            printf("ERROR at line %d: Module \"%s\" has already been declared\n", astNode->tk->lineNo, astNode->tk->val.identifier);
        else
            insertModule(SymbolTable, createModule(astNode->tk->val.identifier, NULL, NULL, NULL));
        break;

    case DRIVERMODULE:
        curr = createModuleTable(20);
        insertModule(SymbolTable, createModule("driver", NULL, NULL, curr));
        typechecker(astNode->child);
        break;

    case MODULE:
        searched = searchModule(SymbolTable, astNode->child->tk->val.identifier);
        if (searched != NULL && searched->moduleTable != NULL)
            printf("ERROR at line %d: Module \"%s\" has already been defined\n", astNode->tk->lineNo, astNode->tk->val.identifier);
        else
        {
            curr = createModuleTable(20);
            if (searched == NULL)
                insertModule(SymbolTable, createModule(astNode->child->tk->val.identifier, NULL, NULL, curr));
            else
                searched = curr;
            ASTNode *current = astNode->child->sibling;
            while (current != NULL)
            {
                if (current->label == INPUT_PLIST)
                {
                    ASTNode *parameter = current->child;
                    ParamList *input_plist = NULL;
                    while (parameter != NULL)
                    {
                        ParamList *newnode = (ParamList *)malloc(sizeof(ParamList));
                        newnode->identifier = parameter->tk->val.identifier;
                        typechecker(parameter->child);
                        newnode->type = parameter->child->type;
                        newnode->next = NULL;
                        input_plist = insertlast(input_plist, newnode);
                        ModuleTableEntry *newEntry;
                        newEntry->identifier = newnode->identifier;
                        newEntry->nesting_lvl = 1;
                        newEntry->offset = offset;
                        newEntry->scope_begin = astNode->scope_begin;
                        newEntry->scope_end = astNode->scope_end;
                        newEntry->type = newnode->type;
                        if (newEntry->type.primtype == BOOLEAN)
                            newEntry->width = 1;
                        else if (newEntry->type.primtype == INTEGER)
                            newEntry->width = 2;
                        else
                            newEntry->width = 4;
                        if (newEntry->type.datatype == ARRAY_STATIC)
                            newEntry->width = (newEntry->width * (newEntry->type.upper_bound - newEntry->type.lower_bound + 1)) - 1;
                        else if (newEntry->type.datatype == ARRAY_DYNAMIC)
                            newEntry->width = 1;
                        offset += newEntry->width;
                        insertVar(curr, newEntry);
                        parameter = parameter->sibling;
                    }
                    searched->inputList = input_plist;
                }
                else if (current->label == OUTPUT_PLIST)
                {
                    ASTNode *parameter = current->child;
                    ParamList *output_plist = NULL;
                    while (parameter != NULL)
                    {
                        ParamList *newnode = (ParamList *)malloc(sizeof(ParamList));
                        newnode->identifier = parameter->tk->val.identifier;
                        typechecker(parameter->child);
                        newnode->type = parameter->child->type;
                        newnode->next = NULL;
                        output_plist = insertlast(output_plist, newnode);
                        ModuleTableEntry *newEntry;
                        newEntry->identifier = newnode->identifier;
                        newEntry->nesting_lvl = 1;
                        newEntry->offset = offset;
                        newEntry->scope_begin = astNode->scope_begin;
                        newEntry->scope_end = astNode->scope_end;
                        newEntry->type = newnode->type;
                        if (newEntry->type.primtype == BOOLEAN)
                            newEntry->width = 1;
                        else if (newEntry->type.primtype == INTEGER)
                            newEntry->width = 2;
                        else
                            newEntry->width = 4;
                        if (newEntry->type.datatype == ARRAY_STATIC)
                            newEntry->width = (newEntry->width * (newEntry->type.upper_bound - newEntry->type.lower_bound + 1)) - 1;
                        else if (newEntry->type.datatype == ARRAY_DYNAMIC)
                            newEntry->width = 1;
                        offset += newEntry->width;
                        insertVar(curr, newEntry);
                        parameter = parameter->sibling;
                    }
                    searched->outputList = output_plist;
                }
                else
                {
                    for (ASTNode *stmt = current->child; stmt != NULL; stmt = stmt->sibling)
                        typechecker(stmt);
                }
                current = current->sibling;
            }
        }
        break;

    case INTEGER_:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = INTEGER;
        break;

    case REAL_:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = REAL;
        break;

    case BOOLEAN_:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = BOOLEAN;
        break;

    case ARRAY_DTYPE:
        typechecker(astNode->child->sibling);
        astNode->type.primtype = astNode->child->sibling->type.primtype;
        astNode->type.lower_bound = INT_MIN;
        astNode->type.upper_bound = INT_MIN;
        ASTNode *left = astNode->child->child;
        ASTNode *right = astNode->child->child->sibling;
        if (left->label == UNARY_MINUS)
        {
            if (left->child->label == NUM)
                astNode->type.lower_bound = (-1) * left->child->tk->val.integer;
            else
                printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n", left->child->tk->lineNo, left->child->tk->val.identifier);
        }
        else if (left->label == UNARY_PLUS)
        {
            if (left->child->label == NUM)
                astNode->type.lower_bound = left->child->tk->val.integer;
            else
                printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n", left->child->tk->lineNo, left->child->tk->val.identifier);
        }
        else if (left->label == NUM)
            astNode->type.lower_bound = left->child->tk->val.integer;
        else
            printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n", left->tk->lineNo, left->tk->val.identifier);
        if (right->label == UNARY_MINUS)
        {
            if (right->child->label == NUM)
                astNode->type.lower_bound = (-1) * right->child->tk->val.integer;
            else
                printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n", right->child->tk->lineNo, right->child->tk->val.identifier);
        }
        else if (right->label == UNARY_PLUS)
        {
            if (right->child->label == NUM)
                astNode->type.lower_bound = right->child->tk->val.integer;
            else
                printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n", right->child->tk->lineNo, right->child->tk->val.identifier);
        }
        else if (right->label == NUM)
            astNode->type.lower_bound = right->child->tk->val.integer;
        else
            printf("Error at line %d: identifier \"%s\" not expected. Expected an integer\n", right->tk->lineNo, right->tk->val.identifier);
        if (astNode->type.upper_bound != INT_MIN && astNode->type.lower_bound != INT_MIN && astNode->type.upper_bound < astNode->type.lower_bound)
            printf("Error: lower bound of array should be less than or equal to upper bound\n");
        if (astNode->type.upper_bound != INT_MIN && astNode->type.lower_bound != INT_MIN)
            astNode->type.datatype = ARRAY_STATIC;
        else
            astNode->type.datatype = ARRAY_DYNAMIC;
        break;

    case INPUT:
        var = searchVar(curr, astNode->tk->val.identifier);
        if (var == NULL)
            printf("Error at line %d: identifier\"%s\" not recognized\n", astNode->tk->lineNo, astNode->tk->val.identifier);
        else if (var->type.datatype != PRIMITIVE)
            printf("Error at line %d: cannot take array as input\n", astNode->tk->lineNo);
        break;

    case OUTPUT:
        if (astNode->child->label == ID)
        {
            ModuleTableEntry *var = searchVar(curr, astNode->child->tk->val.identifier);
            if (var == NULL)
                printf("Error at line %d: identifier\"%s\" not recognized\n", astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
            else if (var->type.datatype != PRIMITIVE)
                printf("Error at line %d: cannot print an array\n", astNode->child->tk->lineNo);
        }
        else if (astNode->child->label == ARR_OUTPUT)
        {
            ModuleTableEntry *var = searchVar(curr, astNode->child->child->tk->val.identifier);
            if (var == NULL)
                printf("Error at line %d: identifier\"%s\" not recognized\n", astNode->child->child->tk->lineNo, astNode->child->child->tk->val.identifier);
            else if (var->type.datatype == PRIMITIVE)
                printf("Error at line %d: %s is not an array\n", astNode->child->child->tk->lineNo, astNode->child->child->tk->val.identifier);
            else
            {
                ASTNode *index = astNode->child->child->sibling;
                if (index->label == UNARY_MINUS)
                {
                    if (index->child->label == NUM && var->type.datatype == ARRAY_STATIC)
                    {
                        int num = (-1) * index->child->tk->val.integer;
                        if (num < var->type.lower_bound || num > var->type.upper_bound)
                            printf("Error at line %d: Array index out of bounds\n", index->child->tk->lineNo);
                    }
                    else if (index->child->label == ID)
                    {
                        ModuleTableEntry *arr_ind = searchVar(curr, index->child->tk->val.identifier);
                        if (arr_ind == NULL)
                            printf("Error at line %d: indentifier \"%s\" not recognized\n", index->child->tk->lineNo, index->child->tk->val.identifier);
                        else if (arr_ind->type.primtype != INTEGER || arr_ind->type.datatype != PRIMITIVE)
                            printf("Error at line %d: array index must be an integer", index->child->tk->lineNo);
                    }
                }
                else if (index == UNARY_PLUS)
                {
                    if (index->child->label == NUM && var->type.datatype == ARRAY_STATIC)
                    {
                        int num = index->child->tk->val.integer;
                        if (num < var->type.lower_bound || num > var->type.upper_bound)
                            printf("Error at line %d: Array index out of bounds\n", index->child->tk->lineNo);
                    }
                    else if (index->child->label == ID)
                    {
                        ModuleTableEntry *arr_ind = searchVar(curr, index->child->tk->val.identifier);
                        if (arr_ind == NULL)
                            printf("Error at line %d: indentifier \"%s\" not recognized\n", index->child->tk->lineNo, index->child->tk->val.identifier);
                        else if (arr_ind->type.primtype != INTEGER || arr_ind->type.datatype != PRIMITIVE)
                            printf("Error at line %d: array index must be an integer", index->child->tk->lineNo);
                    }
                }
                else if (index->label == NUM && var->type.datatype == ARRAY_STATIC && (index->tk->val.integer < var->type.lower_bound || index->tk->val.integer > var->type.upper_bound))
                    printf("Error at line %d: Array index out of bounds\n", index->tk->lineNo);
                else
                {
                    ModuleTableEntry *arr_ind = searchVar(curr, index->tk->val.identifier);
                    if (arr_ind == NULL)
                        printf("Error at line %d: indentifier \"%s\" not recognized\n", index->tk->lineNo, index->tk->val.identifier);
                    else if (arr_ind->type.primtype != INTEGER || arr_ind->type.datatype != PRIMITIVE)
                        printf("Error at line %d: array index must be an integer", index->tk->lineNo);
                }
            }
        }
        break;

    case ASSIGN:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        TypeInfo t1 = astNode->child->type;
        TypeInfo t2 = astNode->child->sibling->type;
        if (t1.primtype == ERROR || t2.primtype == ERROR)
            break;
        else if (t1.datatype != t2.datatype || t1.primtype != t2.primtype)
        {
            if (astNode->child->child == NULL)
                printf("Type Error at line %d: Operand types don't match in assignment operation\n", astNode->child->tk->lineNo);
            else
            {
                printf("Type Error at line %d: Operand types don't match in assignment operation\n", astNode->child->child->tk->lineNo);
            }
        }
        else if (t1.datatype == ARRAY_STATIC && t2.datatype == ARRAY_STATIC && (t1.lower_bound != t2.lower_bound || t1.upper_bound != t2.upper_bound))
        { // static type checking
            printf("Type Error at line %d: Operands of array datatype have different bounds in assignment operation\n", astNode->child->child->tk->lineNo);
        }
        // dynamic type checking
        break;

    case DECLARE:
        typechecker(astNode->child->sibling);
        TypeInfo d = astNode->child->sibling->type;
        int width = 0;
        if (d.primtype == BOOLEAN)
            width = 1;
        else if (d.primtype == INTEGER)
            width = 2;
        else if (d.primtype == REAL)
            width = 4;
        else if (d.datatype == ARRAY_STATIC)
        {
            width = (d.upper_bound - d.lower_bound + 1) * width + 1;
        }
        ASTNode *idList = astNode->child->child;
        while (idList != NULL)
        {
            char *s = idList->tk->val.identifier;
            // if(searchVar(curr,s)!=NULL && idList->scope_begin<=searchVar(curr,s)->scope_end){
            //     printf("Error at line %d: Variable has already been declared at line %d\n",idList->tk->lineNo,searchVar(curr,s)->scope_begin);
            // }

            ModuleTableEntry *new_entry = (ModuleTableEntry *)malloc(sizeof(ModuleTableEntry));
            new_entry->identifier = s;
            new_entry->type = d;
            new_entry->scope_begin = idList->scope_begin;
            new_entry->scope_end = idList->scope_end;
            if (d.datatype != ARRAY_DYNAMIC)
            {
                new_entry->width = width;
            }
            new_entry->offset = offset;
            new_entry->nesting_lvl = idList->nest_level;
            insertVar(curr, new_entry);
            offset += width;
            idList = idList->sibling;
        }
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
        if (searchVar(curr, astNode->tk->val.identifier) == NULL)
        {
            printf("Error at line %d: Variable %s has not been declared\n", astNode->tk->lineNo, astNode->tk->val.identifier);
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (searchVar(curr, astNode->tk->val.identifier) != NULL)
        {
            printf("Scope Error at line %d: Variable %s has been used out of scope\n Previous declaration is at line %d\n", astNode->tk->lineNo, astNode->tk->val.identifier, searchVar(curr, astNode->tk->val.identifier)->scope_begin);
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else
        {
            astNode->type = searchVar(curr, astNode->tk->val.identifier)->type;
        }
        break;

    case NUM:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = INTEGER;
        break;

    case RNUM:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = REAL;
        break;

    case TRUE:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = BOOLEAN;
        break;

    case FALSE:
        astNode->type.datatype = PRIMITIVE;
        astNode->type.primtype = BOOLEAN;
        break;

    case ARRAY:
        var = searchVar(curr, astNode->child->tk->val.identifier);
        if (var == NULL)
        {
            printf("Error at line %d: Array variable %s not declared\n", astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else if (astNode->child->sibling->child != NULL)
        {
            typechecker(astNode->child->sibling->child);
            if (astNode->child->sibling->child->type.datatype != PRIMITIVE || astNode->child->sibling->child->type.primtype != INTEGER)
            {
                printf("Type Error at line %d: Index of array variable %s has been found to be of non-integer type\n", astNode->child->tk->lineNo, astNode->child->tk->val.identifier);
                astNode->type.datatype = PRIMITIVE;
                astNode->type.primtype = ERROR;
            }
        }
        else
        {
            astNode->type = var->type;
        }
        break;

    case PLUS:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in addition\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in addition\n", right_op->child->tk->lineNo);
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in addition\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in addition\n", right_op->tk->lineNo);
        }
        else if (left_op->type.primtype != BOOLEAN && left_op->type.primtype != ERROR && compare_Datatype(left_op->type, right_op->type) == true)
        {
            astNode->type = left_op->type;
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in addition\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in addition\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in addition\n");
            }
        }
        break;

    case MINUS:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in subtraction\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in subtraction\n", right_op->child->tk->lineNo);
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in subtraction\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in subtraction\n", right_op->tk->lineNo);
        }
        else if (left_op->type.primtype != BOOLEAN && left_op->type.primtype != ERROR && compare_Datatype(left_op->type, right_op->type) == true)
        {
            astNode->type = left_op->type;
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in subtraction\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in subtraction\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in subtraction\n");
            }
        }
        break;

    case MUL:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in multiplication\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in multiplication\n", right_op->child->tk->lineNo);
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in multiplication\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in multiplication\n", right_op->tk->lineNo);
        }
        else if (left_op->type.primtype != BOOLEAN && left_op->type.primtype != ERROR && compare_Datatype(left_op->type, right_op->type) == true)
        {
            astNode->type = left_op->type;
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in multiplication\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in multiplication\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in multiplication\n");
            }
        }
        break;

    case DIV:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in division\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in division\n", right_op->child->tk->lineNo);
        }
        else if (left_op->type.primtype == BOOLEAN || right_op->type.primtype == BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in division\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Boolean operand found in division\n", right_op->tk->lineNo);
        }
        else if (compare_Datatype(left_op->type, right_op->type) == true)
        {
            astNode->type = left_op->type;
        }
        else if ((left_op->type.primtype == INTEGER && right_op->type.primtype == REAL) || (right_op->type.primtype == INTEGER && left_op->type.primtype == REAL))
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = REAL;
        }
        else if (left_op->type.primtype == ERROR || right_op->type.primtype == ERROR)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
        }
        else
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in division\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in division\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in division\n");
            }
        }
        break;

    case AND:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in logical operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in logical operation\n", right_op->child->tk->lineNo);
        }
        else if (left_op->type.primtype != BOOLEAN || right_op->type.primtype != BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n", right_op->tk->lineNo);
            else
            {
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
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in logical operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in logical operation\n", right_op->child->tk->lineNo);
        }
        else if (left_op->type.primtype != BOOLEAN || right_op->type.primtype != BOOLEAN)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected both operands of boolean type in logical operation\n", right_op->tk->lineNo);
            else
            {
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
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", right_op->child->tk->lineNo);
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
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case LE:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", right_op->child->tk->lineNo);
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
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case GT:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", right_op->child->tk->lineNo);
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
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case GE:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", right_op->child->tk->lineNo);
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
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case EQ:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", right_op->child->tk->lineNo);
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
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case NE:
        typechecker(astNode->child);
        typechecker(astNode->child->sibling);
        ASTNode *left_op = astNode->child;
        ASTNode *right_op = astNode->child->sibling;
        if (left_op->type.datatype != PRIMITIVE || right_op->type.datatype != PRIMITIVE)
        {
            astNode->type.datatype = PRIMITIVE;
            astNode->type.primtype = ERROR;
            if (left_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", left_op->child->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Array operand found in relational operation\n", right_op->child->tk->lineNo);
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
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", left_op->tk->lineNo);
            else if (right_op->tk != NULL)
                printf("Type Error at line %d: Expected operands of similar type in relational operation\n", right_op->tk->lineNo);
            else
            {
                printf("Expected operands of similar type in relational operation\n");
            }
        }
        break;

    case RANGE_WHILE:
        ASTNode *current = astNode->child;
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        if (astNode->child->type.primtype != BOOLEAN || astNode->child->type.datatype != PRIMITIVE)
        {
            printf("TYPE ERROR: at line:= %d, Module \"%s\" has already been defined\n", astNode->tk->lineNo, astNode->tk->val.identifier);
        }
        break;

    case ITER_FOR:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        ModuleTableEntry *newEntry;
        newEntry->identifier = astNode->tk->val.identifier;
        newEntry->nesting_lvl = astNode->nest_level;
        newEntry->offset = offset;
        offset += 2;
        newEntry->scope_begin = astNode->scope_begin;
        newEntry->scope_end = astNode->scope_end;
        newEntry->type.datatype = PRIMITIVE;
        newEntry->type.primtype = INTEGER;
        insertVar(curr, newEntry);
        break;

    case RANGE_FOR:
        ASTNode *num1, *num2;
        if (astNode->child->type.datatype == PRIMITIVE && astNode->child->type.primtype == INTEGER)
        {
            num1 = astNode->child;
        }
        else
            num1 = astNode->child->child;
        if (astNode->child->sibling->type.datatype == PRIMITIVE && astNode->child->sibling->type.primtype == INTEGER)
        {
            num2 = astNode->child->sibling;
        }
        else
            num2 = astNode->child->sibling->child;
        if (!(num1 != NULL && num2 != NULL && num1->type.datatype == num2->type.datatype && num1->type.primtype == num2->type.primtype && num1->type.datatype == PRIMITIVE && num1->type.primtype == INTEGER))
        {
            printf("TYPE ERROR: at line:= %d, iterator range bounds must be integer\n", astNode->tk->lineNo);
        }
        break;

    case CASE_STMT:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        break;

    case CASE:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        break;

    case DEFAULT:
        for (ASTNode *current = astNode->child; current != NULL; current = current->sibling)
            typechecker(current);
        break;

    // <moduleReuseStmt> <optional> ID <actual_para_list>
    case MODULE_REUSE:
        MainTableEntry *entry = searchModule(SymbolTable, astNode->tk->val.identifier);
        bool flag = true;
        if (entry == NULL)
        {
            printf("TYPE ERROR: at line:= %d, Module not found\n", astNode->tk->lineNo);
            break;
        }
        if (astNode->child->label == ASSIGN)
        {
            ASTNode *formal_out = astNode->child->child->child;
            ASTNode *formal_in = astNode->child->sibling->child;
            ParamList *real_in = entry->inputList;
            ParamList *real_out = entry->outputList;
            while (real_out != NULL)
            {
                if (formal_out == NULL || !compare_Datatype(formal_out->type, real_out->type))
                {
                    flag = false;
                    break;
                }
                formal_out = formal_out->sibling;
                real_out = real_out->next;
            }
            while (real_in != NULL)
            {
                if (formal_in == NULL || !compare_Datatype(formal_in->type, real_in->type))
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
            ParamList *formal_in = entry->inputList;
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
            printf("TYPE ERROR: at line:= %d, Module reuse parameters mismatch\n", astNode->tk->lineNo);
        }
        break;

    default:
        break;
    }
}
