#ifndef lexerDef_H
#define lexerDef_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*>>>>>>>>>>>> INIITIALIZING STRUCTS AND GLOBAL VARS <<<<<<<<<<<<<<<<<*/

char buffer1[32], buffer2[32];
FILE *ptr;

union TokenVal
{
    int integer;
    float decimal;
    char *identifier;
};

struct Token
{
    char *type;
    int lineNo;
    union TokenVal val;
};

#endif
