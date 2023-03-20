/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#include "parserDef.h"

#define NO_RULES 141 //No. of grammar rules: 141
#define NO_TERMS 58 //No. of terminals: 58
#define NO_NONTERMS 72 //No. of Non Terminals: 72
#define MAXTERMLEN 50 // length of maximum terminal/nonterminal

extern struct ListNode* grammar[NO_RULES];
extern struct ListNode* first[NO_NONTERMS];
extern struct ListNode* follow[NO_NONTERMS];
extern int parseTable [NO_NONTERMS][NO_TERMS - 1];
extern struct ListNode* synchronizingSet[NO_NONTERMS];

void computefirstandfollow();

void loadgrammar(char* filename);

void createParseTable();

void printTree(TreeNode* root, FILE* fp);

TreeNode* parse();
