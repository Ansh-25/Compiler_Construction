#include "lexer.h"
#include "parserdef.h"

#define NO_RULES 100
#define NO_TERMS 50
#define NO_NONTERMS 50
#define MAXTERMLEN 20

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_TERMS][NO_NONTERMS];

non_terminal maptoenum(char* s) {}

void loadgrammar(char* filename) {
    FILE* fp = fopen(filename,"r");
    if (!fp) {
        printf("Grammar file cannot be opnened\n");
        exit(1);
    }
    char ch;
    int index = 0;
    char currinput[MAXTERMLEN];
    for (int i = 0; i < MAXTERMLEN; i ++)
        currinput[i] = '\0';
    while (!feof(fp)) {
        ch = fgetc(fp);
        if (ch == ' ' || ch == '\n') {
            struct ListNode* newnode = (struct ListNode*)malloc(sizeof(struct ListNode));
            if (currinput[0] == '<') {
                newchar -> t = NONTERMINAL;

            }
        }
        else {
            currinput[index] = ch;
            index ++;
        }
    }
}