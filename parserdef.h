#include "lexerDef.h"
#include <stdlib.h>

typedef enum {program, moduleDeclarations, otherModules, driverModule, moduleDeclaration, module, moduleDef, input_plist, ret, output_plist, dataType, _input_plist, type, _output_plist, range_arrays, index_arr, statements, statement, ioStmt, simpleStmt, declareStmt, conditionalStmt, iterativeStmt, var_print, P1, boolConstt, array_element_for_print, new_index, assignmentStmt, moduleReuseStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, element_index_with_expressions, sign, optional, actual_para_list, N_11, idList, N3, arithmeticOrBooleanExpr, U, unary_op, new_NT, arithmeticExpr, var_id_num, unary_op, AnyTerm, N7, logicalOp, N8, relationalOp, term, N4, op1, factor, N5, op2, N_10, arrExpr, arrTerm, arr_N4, arr_N5, arrFactor, }non_terminal;
typedef enum {TERMINAL, NONTERMINAL}type;

union grammaritem {
    non_terminal nt;
    tokentype t;
};

typedef struct grammarchar {
    union grammaritem g;
    type t;
}grammarchar;

typedef struct ListNode {
    grammarchar val;
    ListNode* next;
}ListNode;

union treenodeval {
    int ruleno;
    struct Token t;
};

typedef struct TreeNode {
    type t;
    union treenodeval val;
    struct TreeNode* child;
    struct TreeNode* sibling;
}TreeNode;

typedef struct StackNode {
    struct TreeNode* val;
    struct StackNode* next;
}StackNode;

void insertlast (ListNode* head, grammarchar gc) {
    ListNode* newnode = (ListNode *)malloc(sizeof(ListNode));
    newnode -> val = gc;
    newnode -> next = NULL;
    if (head == NULL)
        head = newnode;
    else {
        ListNode* curr = head;
        while (curr -> next != NULL)
            curr = curr -> next;
        curr -> next = newnode;
    }
}

void deletelast (ListNode* head) {
    if (head != NULL) {
        if (head -> next == NULL)
            head = NULL;
        ListNode* curr = head;
        while (curr -> next -> next != NULL)
            curr = curr -> next;
        curr -> next = NULL;
    }
}

int isEmpty (StackNode* top) {
    return top == NULL;
}

void push (StackNode* top, TreeNode* newnode) {
    StackNode* newstacknode = (StackNode*)malloc(sizeof(StackNode));
    newstacknode -> val = newnode;
    newstacknode -> next = top;
    top = newstacknode;
}

TreeNode* top (StackNode* top) {
    if (isEmpty(top))
        return NULL;
    return top -> val;
}

TreeNode* pop (StackNode* top) {
    if (isEmpty(top))
        return NULL;
    StackNode* curr = top;
    top = top -> next;
    return curr -> val;
}