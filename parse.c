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

TreeNode* parse(){
    StackNode* S;
    TreeNode* Root = (TreeNode*)malloc(sizeof(TreeNode));
    Root->t = NONTERMINAL;
    Root->ruleno = 0;
    Root->sibling = NULL;
    TreeNode* X;
    X = Root;
    push(S,X);
    struct Token* L = getNextToken();
    while(!L){
        if(isEmpty(S)){
            //reportError
            break;
        }
        X = top(S);
        if(X->t==TERMINAL){
            if(X->val.t.type == L->type){
                X->t = TERMINAL;
                X->val.t = *L;
                X->child = NULL;
                X->sibling = NULL;
                pop(S);
                L = getNextToken();
            }
            else{
                //reportError
                pop(S);
            }
        }
        else if(X->t==NONTERMINAL){
            if(parseTable[X->val.nt][L->type]>=0){
                X->ruleno = parseTable[X->val.nt][L->type];
                //pushRule(S)
            }

            // else if( ){

            // }

            else{
                L = getNextToken();
            }
        }
        else    
            continue;
    }
    if(!isEmpty(S)){
        //reportError
    }
    return Root;
}