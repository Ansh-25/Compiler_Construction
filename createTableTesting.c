#include "lexer.h"
#include "parserdef.h"


//typedef enum {E,E',T,T',F} non_t;


#define NO_RULES 141
#define NO_TERMS 57
#define NO_NONTERMS 72
#define MAXTERMLEN 31

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS];

// int derivesEpsilon(int b){
//     if(b==1 || b==3)
//         return 1;
//     else    
//         return 0;
// }
int derivesepsilon(grammarchar gc){
    if (gc.t == TERMINAL)
        return 0;
    if (first[gc.g.nt] == NULL)
        return 0;
    ListNode* curr = first[gc.g.nt];
    while (curr -> next != NULL)
        curr = curr -> next;
    if (curr -> val.g.t == EPS)
        return 1;
    return 0;
}

void printList(ListNode* head){
    ListNode* t2 = head;
    while(t2!=NULL){
        printf("%d",t2->val.t==TERMINAL?t2->val.g.t1:t2->val.g.nt1);
        t2=t2->next;
    }
}
void createParseTable(){
    int s = 57*72;
    memset(parseTable,-1,s*sizeof(int));

    for(int i=0;i<=7;i++){
        struct ListNode* A = grammar[i];
        struct ListNode* B = grammar[i];
        int a = A->val.g.nt1;
        int b = -1;
        do{
            B = B->next;
            if(B==NULL || (B->val.t==TERMINAL && B->val.g.t1==eps)){
                struct ListNode* C = follow[a];
                while(C!=NULL){
                    int c = C->val.g.t1; 
                    parseTable[a][c] = i;
                    C = C->next;
                }
                break;
            }

            if(B->val.t==TERMINAL){
                b = B->val.g.t1; 
                parseTable[a][b] = i;
                break;
            }
            else{
                b = B->val.g.nt1;
                struct ListNode* C = first[b];
                while(C!=NULL){
                    int c = C->val.g.t1; 
                    if(C->val.g.t1!=eps)
                        parseTable[a][c] = i;
                    C = C->next;
                }
            }
        } while(derivesepsilon(B->val)); //derivesEpsilon returns 0 on input -1
    }
}


int main(){
    grammarchar gc1;
    grammarchar gc2;
    gc1.t = TERMINAL;
    gc2.t = NONTERMINAL;
    gc2.g.nt1 = E;
    grammar[0] = insertlast(grammar[0],gc2);
    gc2.g.nt1 = T;
    grammar[0] = insertlast(grammar[0],gc2);
    gc2.g.nt1 = E1;
    grammar[0] = insertlast(grammar[0],gc2);
    gc2.g.nt1 = E1;
    grammar[1] = insertlast(grammar[1],gc2);
    gc1.g.t1 = plus;
    grammar[1] = insertlast(grammar[1],gc1);
    gc2.g.nt1 = T;
    grammar[1] = insertlast(grammar[1],gc2);
    gc2.g.nt1 = E1;
    grammar[1] = insertlast(grammar[1],gc2);
    gc2.g.nt1 = E1;
    grammar[2] = insertlast(grammar[2],gc2);
    gc1.g.t1 = eps;
    grammar[2] = insertlast(grammar[2],gc1);
    gc2.g.nt1 = T;
    grammar[3] = insertlast(grammar[3],gc2);
    gc2.g.nt1 = F;
    grammar[3] = insertlast(grammar[3],gc2);
    gc2.g.nt1 = T1;
    grammar[3] = insertlast(grammar[3],gc2);
    gc2.g.nt1 = T1;
    grammar[4] = insertlast(grammar[4],gc2);
    gc1.g.t1 = mul;
    grammar[4] = insertlast(grammar[4],gc1);
    gc2.g.nt1 = F;
    grammar[4] = insertlast(grammar[4],gc2);
    gc2.g.nt1 = T1;
    grammar[4] = insertlast(grammar[4],gc2);
    gc2.g.nt1 = T1;
    grammar[5] = insertlast(grammar[5],gc2);
    gc1.g.t1 = eps;
    grammar[5] = insertlast(grammar[5],gc1);
    gc2.g.nt1 = F;
    grammar[6] = insertlast(grammar[6],gc2);
    gc1.g.t1 = id;
    grammar[6] = insertlast(grammar[6],gc1);
    gc2.g.nt1 = F;
    grammar[7] = insertlast(grammar[7],gc2);
    gc1.g.t1 = op;
    grammar[7] = insertlast(grammar[7],gc1);
    gc1.g.t1 = id;
    grammar[7] = insertlast(grammar[7],gc1);
    gc1.g.t1 = cl;
    grammar[7] = insertlast(grammar[7],gc1);

    gc2.g.nt1 = E;
    gc1.g.t1 = op;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc1.g.t1 = id;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc2.g.nt1 = E1;
    gc1.g.t1 = plus;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc1.g.t1 = eps;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc2.g.nt1 = T;
    gc1.g.t1 = op;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc1.g.t1 = id;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc2.g.nt1 = F;
    gc1.g.t1 = op;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc1.g.t1 = id;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc2.g.nt1 = T1;
    gc1.g.t1 = mul;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);
    gc1.g.t1 = eps;
    first[gc2.g.nt1] = insertlast(first[gc2.g.nt1],gc1);

    gc2.g.nt1 = E;
    gc1.g.t1 = cl;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = $;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc2.g.nt1 = E1;
    gc1.g.t1 = cl;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = $;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc2.g.nt1 = T;
    gc1.g.t1 = plus;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = cl;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = $;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc2.g.nt1 = T1;
    gc1.g.t1 = plus;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = cl;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = $;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc2.g.nt1 = F;
    gc1.g.t1 = mul;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = plus;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = cl;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);
    gc1.g.t1 = $;
    follow[gc2.g.nt1] = insertlast(follow[gc2.g.nt1],gc1);

    //printList(grammar[2]);
    
    createParseTable();
    for(int i=0;i<5;i++){
        for(int j=0;j<7;j++){
            printf(" %d",parseTable[i][j]);
        }
        printf("\n");
    }
    return 0;
}
