#include "lexer.h"
#include <stdlib.h>

int main()
{
    ptr = initLexer(ptr, 32);
    struct Token *tk;
    do{
        tk = getNextToken();
        printToken(tk);
    }while(tk!=NULL);
    return 0;
}