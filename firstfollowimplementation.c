#include "lexer.h"
#include "parserdef.h"

#define NO_RULES 141
#define NO_TERMS 57
#define NO_NONTERMS 72
#define MAXTERMLEN 31

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS];

/*computefirstandfollow (grammar){
    for all non terminals t:
        computefirst(t);
    for all non terminals t:
        computefollow(t);
}*/

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

void computeFirst (non_terminal A)
{
    if (first[A]!=NULL) return;
    for(int i=0; i<NO_RULES; i++){
        if (grammar[i] -> val.g.nt == A){
            ListNode* rhs_current = grammar[i] -> next;
            if (rhs_current -> val.t == TERMINAL){
                first[i] = insertlast(first[i], rhs_current -> val);
            }

            else if (rhs_current -> val.t == NONTERMINAL){
                computeFirst(rhs_current -> val.g.nt);
                ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                while (first_rhs_current != NULL) {
                    if (first_rhs_current -> val.g.t != EPS)
                        first[i] = insertlast(first[i], first_rhs_current -> val);
                    first_rhs_current = first_rhs_current -> next;
                }
                while (derivesepsilon(rhs_current -> val)){
                    rhs_current = rhs_current -> next;
                    if (rhs_current == NULL){
                        grammarchar add_eps;
                        add_eps.t = TERMINAL;
                        add_eps.g.t = EPS;
                        first[i] = insertlast(first[i], add_eps);
                    }
                    else if (rhs_current -> val.t == TERMINAL){
                        first[i] = insertLast(first[i], rhs_current -> val);
                    }
                    else{
                        computeFirst(rhs_current -> val.g.nt);
                        ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                        while (first_rhs_current != NULL) {
                            if (first_rhs_current -> val.g.t != EPS)
                                first[i] = insertlast(first[i], first_rhs_current -> val);
                            first_rhs_current = first_rhs_current -> next;
                        }
                    }
                }
            }
        }
    }
}


void computeFollow(non_terminal A) {
    if (follow[A]!=NULL) return;
    for(int i=0; i<NO_RULES; i++){
        ListNode* lhs_current = grammar[i];
        ListNode* rhs_current = grammar[i] -> next;
        if (rhs_current -> val.t == TERMINAL){
            follow[i] = insertLast(follow[i], rhs_current -> val);
        }
        else if(rhs_current -> val.t == NONTERMINAL){
            ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
            while (first_rhs_current != NULL) {
                if (first_rhs_current -> val.g.t != EPS){
                    follow[i] = insertlast(follow[i], first_rhs_current -> val);
                    first_rhs_current = first_rhs_current -> next;
                }
            }

            while(derivesepsilon(rhs_current -> val)){
                rhs_current = rhs_current -> next;
                if (rhs_current == NULL){
                    computeFollow(lhs_current -> val.g.nt);
                    follow[i] = insertLast(follow[i], lhs_current -> val);
                }
                else if (rhs_current -> val.t == TERMINAL){
                    follow[i] = insertLast(follow[i], rhs_current -> val);
                }
                else{
                    computeFirst(rhs_current -> val.g.nt);
                    ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                    while (first_rhs_current != NULL) {
                        follow[i] = insertlast(follow[i], first_rhs_current -> val);
                        first_rhs_current = first_rhs_current -> next;
                    }
                }
            }
        }
    }
}