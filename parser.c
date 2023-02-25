#include "lexer.h"
#include "parserdef.h"

#define NO_RULES 100
#define NO_TERMS 50
#define NO_NONTERMS 50

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_TERMS][NO_NONTERMS];

non_terminal maptoenum(char* s) {
    
}