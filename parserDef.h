/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#ifndef parserDef_H
#define parserDef_H

#include "lexerDef.h"
#include <stdlib.h>

typedef enum {program, moduleDeclarations, otherModules, driverModule, moduleDeclaration, module, moduleDef, input_plist, ret, output_plist, dataType, _input_plist, _type, _output_plist, range_arrays, index_arr, statements, statement, ioStmt, simpleStmt, declareStmt, conditionalStmt, iterativeStmt, var_print, P1, boolConstt, new_index, assignmentStmt, moduleReuseStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, expression, element_index_with_expressions, sign, optional, actual_para_list, K, N_12, N_11, idList, N3, arithmeticOrBooleanExpr, U, unary_op, new_NT, arithmeticExpr, var_id_num, AnyTerm, N7, logicalOp, N8, relationalOp, term, N4, op1, factor, N5, op2, N_10, arrExpr, arrTerm, arr_N4, arr_N5, arrFactor, caseStmts, _default, value, N9, range_for_loop, index_for_loop, sign_for_loop, new_index_for_loop}non_terminal;
typedef enum {TERMINAL, NONTERMINAL}type;
typedef struct ASTNode ASTNode;

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
    struct ListNode* next;
}ListNode;

union treenodeval {
    non_terminal nt;
    struct Token* t;
};

typedef struct ParseNode {
    type t;
    int ruleno;
    struct ASTNode* addr;
    union treenodeval val;
    struct ParseNode* child;
    struct ParseNode* sibling;
}ParseNode;

typedef struct StackNode {
    struct ParseNode* val;
    struct StackNode* next;
}StackNode;

#endif