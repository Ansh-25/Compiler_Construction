#include "ff.h"

int main() {
    loadgrammar("GrammarForParser");
    computefirstandfollow();
    createParseTable();
    // printf("FIRST :- \n");
    // for (int i = 0; i < NO_NONTERMS; i ++) {
    //     printf("\nFirst set of %d: \n", i);
    //     for (ListNode* curr = first[i]; curr != NULL; curr = curr -> next) {
    //         printf("%s ",mapttokentostring(curr -> val.g.t));
    //     }
    // }
    // printf("\n\n\nFOLLOW :- \n");
    // for (int i = 0; i < NO_NONTERMS; i ++) {
    //     printf("\nFollow set of %d: \n", i);
    //     for (ListNode* curr = follow[i]; curr != NULL; curr = curr -> next) {
    //         printf("%s ",mapttokentostring(curr -> val.g.t));
    //     }
    // }

    // printf("\n");
    
    // for (int i = 0; i < NO_NONTERMS; i ++) {
    //     for (int j = 0; j < NO_TERMS - 1; j ++) {
    //         printf("%d ",parseTable[i][j]);
    //     }
    //     printf("\n");
    // }

    ListNode* curr;
    for (int i = 0; i < NO_RULES; i ++) {
        printf("RULE: %d\n",i);
        curr = grammar[i];
        while (curr != NULL) {
            if (curr -> val.t == TERMINAL)
                printf("%d ", curr -> val.g.t);
            else
                printf("%d ", curr -> val.g.nt);
            curr = curr -> next;
        }
        printf("\n");
    }
}