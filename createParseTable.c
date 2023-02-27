#include "lexer.h"
#include "parserdef.h"

#define NO_RULES 141
#define NO_TERMS 55
#define NO_NONTERMS 71
#define MAXTERMLEN 31

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS];

void createParseTable(){
    int s = (NO_TERMS+1)*(NO_NONTERMS+1);
    memset(parseTable,-2,s*sizeof(int));

    for(int i=0;i<NO_RULES;i++){
        ListNode* A = grammar[i];
        ListNode* B = grammar[i];
        int a = A->val.g.nt;
        int b = -1;
        do{
            B = B->next;
            if(B==NULL || (B->val.t==TERMINAL && B->val.g.t==EPS)){
                ListNode* C = follow[a];
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
                ListNode* C = first[b];
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