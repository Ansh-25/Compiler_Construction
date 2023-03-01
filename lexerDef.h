#ifndef lexerDef_H
#define lexerDef_H

typedef enum {TK_ID, TK_PLUS, TK_MINUS, TK_NUM, TK_RNUM, TK_EQ, TK_MUL, TK_LT, TK_LE, TK_DEF, TK_DRIVERDEF, TK_COMMA, TK_SEMICOLON, TK_COLON, TK_ASSIGNOP, TK_BO, TK_BC, TK_GT, TK_GE, TK_ENDDEF, TK_DRIVERENDDEF, TK_SQBO, TK_SQBC, TK_NE, TK_DIV, TK_RANGEOP, TK_INTEGER, TK_REAL, TK_BOOLEAN, TK_OF, TK_ARRAY, TK_START, TK_END, TK_DECLARE, TK_MODULE, TK_DRIVER, TK_PROGRAM, TK_GET_VALUE, TK_PRINT, TK_USE, TK_WITH, TK_PARAMETERS, TK_TAKES, TK_INPUT, TK_RETURNS, TK_FOR, TK_IN, TK_SWITCH, TK_CASE, TK_BREAK, TK_DEFAULT, TK_WHILE, TK_AND, TK_OR, TK_TRUE, TK_FALSE, TK_EOF, EPS}tokentype;

union TokenVal
{
    int integer;
    double decimal;
    char identifier[32];
};

struct Token    
{
    tokentype type;
    int lineNo;
    union TokenVal val;
};

#endif
