#ifndef IntCodeGenDef_H
#define IntCodeGenDef_H

#include <stdlib.h>
#include <stdbool.h>
#include "symbolTableDef.h"

typedef enum {PLUS, MINUS, MUL, DIV, AND, OR, LT, LE, GT, GE, EQ, NE, UNARY_PLUS, UNARY_MINUS, EQUALS, GOTO, IF_TRUE, IF_FALSE, IF_LT, IF_LE, IF_GT, IF_GE, IF_EQ, IF_NE, PARAM, MCALL, FCALL, ARRAY1, ARRAY2, INPUT, OUTPUT} Operation;

union ColEntry{
    bool is_blank;
    struct Quadruple* label;
    ModuleTableEntry* var;
    int val1;
    float val2;
    bool val3;
};

typedef struct Quadruple{
    Operation op;
    union ColEntry res;
    union ColEntry first;
    union ColEntry second;
}Quadruple;

typedef struct QuadNode{
    Quadruple quad;
    struct QuadNode* next;
}QuadNode;


#endif