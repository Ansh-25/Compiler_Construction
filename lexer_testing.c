#include "lexervarbuff.h"

int main()
{
    ptr = initLexer(ptr, 128);
    struct Token *tk;
    do{
        tk = getNextToken();
        printToken(tk);
        // printf("hi - %d\n",search_hash("driver"));
        // printf("%d\n",search_hash("program"));
        // printf("%s\n",buffer1);
        // printf("%s\n",buffer2);
    }while(tk!=NULL);
    return 0;
}