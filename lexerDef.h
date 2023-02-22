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

struct Token
{
    char *type;
    int lineNo;
    union TokenVal val;
};

void printToken(struct Token *tk){
    if(tk==NULL) {
        printf("ERROR: Empty Token\n");
        return;
    }
    printf("line := %d  type:= %s  ",tk->lineNo,tk->type);
    if(strcmp(tk->type,"TK_NUM")==0){
        printf("val := %d\n",tk->val.integer);
    }
    else if(strcmp(tk->type,"TK_RNUM")==0){
        printf("val := %f\n",tk->val.decimal);
    }
    else{
        printf("val := %s\n",tk->val.identifier);
    }
}

#endif
