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
    char *identifier;
};

struct Token
{
    char *type;
    int lineNo;
    union TokenVal val;
};

#endif
