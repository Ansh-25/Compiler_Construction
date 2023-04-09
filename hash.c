#include "hash.h"

int is_init=0;
struct hashentry* hash[47];

int sumchars(char* s){
    int res=0;
    for(int i=0;s[i]!='\0';++i){
        res = (res+s[i])%47;
    }
    return res;
}

void insert_hash(char* str, tokentype tk){
    int ind = sumchars(str);
    while(hash[ind] != NULL) {ind++; ind %=47;}
    hash[ind] = (struct hashentry*)malloc(sizeof(struct hashentry));
    strcpy(hash[ind] -> str,str);
    hash[ind] -> tk = tk;
}

int search_hash(char* str){
    int ind = sumchars(str);
    // printf("hello := %d\n",hash[ind]!=NULL);
    while(hash[ind]!=NULL){
        // printf("%s == %s\n",str,hash[ind]->str);
        // printf("%s",hash[ind]);
        if(strcmp(str,hash[ind] -> str)==0) return hash[ind] -> tk;
        ind++;
        ind %= 47;
    }
    //printf("hello\n");
    return -1;
}

void init_hash()
{ 
    if(is_init==0){
        char *keywords[] = {"integer", "real", "boolean", "of", "array", "start", "end", "declare", "module", "driver", "program", "get_value", "print", "use", "with", "parameters", "takes", "input", "returns", "for", "in", "switch", "case", "break", "default", "while", "AND", "OR", "true", "false"};
        tokentype keywordenum[] = {TK_INTEGER, TK_REAL, TK_BOOLEAN, TK_OF, TK_ARRAY, TK_START, TK_END, TK_DECLARE, TK_MODULE, TK_DRIVER, TK_PROGRAM, TK_GET_VALUE, TK_PRINT, TK_USE, TK_WITH, TK_PARAMETERS, TK_TAKES, TK_INPUT, TK_RETURNS, TK_FOR, TK_IN, TK_SWITCH, TK_CASE, TK_BREAK, TK_DEFAULT, TK_WHILE, TK_AND, TK_OR, TK_TRUE, TK_FALSE};
        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) insert_hash(keywords[i],keywordenum[i]);
        is_init ++;
    }
}