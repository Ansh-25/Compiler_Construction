#include "lexer.h"
#include "parserdef.h"

#define NO_RULES 141
#define NO_TERMS 57
#define NO_NONTERMS 71
#define MAXTERMLEN 31

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS];