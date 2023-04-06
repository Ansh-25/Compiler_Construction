/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#ifndef astDef_H
#define astDef_H

#include "parserDef.h"
#include <stdlib.h>
#include <stdbool.h>

typedef enum {PROGRAM,UNARY_PLUS, UNARY_MINUS, ID, NUM, RNUM, ARRAY,ARR_INDEX1, ARR_INDEX2, PLUS, MINUS, MUL, DIV, AND, OR, LT, LE, GT, GE, EQ, NE, MODULEDECLARATION, DRIVERMODULE,MODULE_REUSE, MODULE, RET, PARAMETER, INTEGER_, REAL_, BOOLEAN_, RANGE_WHILE,RANGE_FOR, STATEMENTS, INPUT, OUTPUT, ARR_OUTPUT, TRUE, FALSE, ASSIGN, ARR_ASSIGN, INDEX_ARR, DECLARE, ID_LIST, CASE,CASE_STMT,RANGE, INPUT_PLIST, OUTPUT_PLIST} Label;

typedef enum {INTEGER, REAL, BOOLEAN} Prim_type;

typedef struct Array_tuple{
    Prim_type pt; 
    int lower_bound;
    int upper_bound;
}Array_tuple;

typedef union Type{
    Prim_type pt;
    Array_tuple at;
}Type;

typedef struct DataType{
    bool is_primitive;
    Type t;
}DataType;

typedef struct ASTNode{
    Label label;
    DataType type;
    struct Token* tk;
    struct ASTNode* child;
    struct ASTNode* sibling;
}ASTNode;


#endif