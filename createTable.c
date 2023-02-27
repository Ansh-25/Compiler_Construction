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

void createParseTable(){
    int s = (NO_TERMS)*(NO_NONTERMS);
    memset(parseTable,-2,s*sizeof(int));

    for(int i=0;i<NO_RULES;i++){
        struct ListNode* A = grammar[i];
        struct ListNode* B = grammar[i];
        int a = A->val.g.nt;
        int b = -1;
        do{
            B = B->next;
            if(B==NULL || (B->val.t==TERMINAL && B->val.g.t==EPS)){
                struct ListNode* C = follow[a];
                while(!C){
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
                while(!C){
                    int c = C->val.g.t; 
                    if(c!=51)
                        parseTable[a][c] = i;
                    C = C->next;
                }
            }
        } while(derivesEpsilon(b)); //derivesEpsilon returns 0 on input -1
    }
}