#ifndef lexerDef_H
#define lexerDef_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*>>>>>>>>>>>> INIITIALIZING STRUCTS AND GLOBAL VARS <<<<<<<<<<<<<<<<<*/

union TokenVal
{
    int integer;
    float decimal;
    char identifier[32];
};

typedef enum {TK_ID, TK_PLUS, TK_MINUS, TK_NUM, TK_RNUM, TK_EQ, TK_MUL, TK_LT, TK_LE, TK_DEF, TK_DRIVERDEF, TK_COMMA, TK_SEMICOLON, TK_COLON, TK_ASSIGNOP, TK_BO, TK_BC, TK_GT, TK_GE, TK_ENDDEF, TK_DRIVERENDDEF, TK_SQBO, TK_SQBC, TK_NE, TK_DIV, TK_RANGEOP}tokentype;

struct Token
{
    tokentype type;
    int lineNo;
    union TokenVal val;
};

void printToken(struct Token *tk){
    if(tk==NULL) {
        printf("ERROR: Empty Token\n");
        return;
    }
    printf("line := %d  type:= %d  ",tk->lineNo,tk->type);
    if(tk -> type == TK_NUM){
        printf("val := %d\n",tk->val.integer);
    }
    else if(tk -> type == TK_RNUM){
        printf("val := %f\n",tk->val.decimal);
    }
    else{
        printf("val := %s\n",tk->val.identifier);
    }
}

#endif
