#include "lexer.h"
#include "parserdef.h"


#define NO_RULES 141
#define NO_TERMS 58
#define NO_NONTERMS 72
#define MAXTERMLEN 50

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS - 1];

void createParseTable(){
    int s = (NO_TERMS-1)*(NO_NONTERMS);
    memset(parseTable,-1,s*sizeof(int));

    for(int i=0;i<NO_RULES;i++){
        struct ListNode* A = grammar[i];
        struct ListNode* B = grammar[i];
        int a = A->val.g.nt;
        int b = -1;
        do{
            B = B->next;
            if(B==NULL || (B->val.t==TERMINAL && B->val.g.t==EPS)){
                struct ListNode* C = follow[a];
                while(C!=NULL){
                    int c = C->val.g.t; 
                    parseTable[a][c] = i;
                    C = C->next;
                }
                break;
            }

            if(B->val.t==TERMINAL){
                b = B->val.g.t; 
                parseTable[a][b] = i;
            }
            else{
                b = B->val.g.nt;
                struct ListNode* C = first[b];
                while(C!=NULL){
                    int c = C->val.g.t; 
                    if(C->val.g.t!=EPS)
                        parseTable[a][c] = i;
                    C = C->next;
                }
            }
        } while(derivesepsilon(B->val)); //derivesEpsilon returns 0 on input -1
    }
}