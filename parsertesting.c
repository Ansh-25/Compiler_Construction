#include "ff.h"

int main() {
    loadgrammar("GrammarForParser");
    computefirstandfollow();
    createParseTable();
    printf("FIRST :- \n");
    for (int i = 0; i < NO_NONTERMS; i ++) {
        printf("\nFirst set of %d: \n", i);
        for (ListNode* curr = first[i]; curr != NULL; curr = curr -> next) {
            printf("%s ",mapttokentostring(curr -> val.g.t));
        }
    }
    printf("\n\n\nFOLLOW :- \n");
    for (int i = 0; i < NO_NONTERMS; i ++) {
        printf("\nFollow set of %d: \n", i);
        for (ListNode* curr = follow[i]; curr != NULL; curr = curr -> next) {
            printf("%s ",mapttokentostring(curr -> val.g.t));
        }
    }

    printf("\n");
    
    for (int i = 0; i < NO_NONTERMS; i ++) {
        for (int j = 0; j < NO_TERMS - 1; j ++) {
            printf("%d ",parseTable[i][j]);
        }
        printf("\n");
    }
}