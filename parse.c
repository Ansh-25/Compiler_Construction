#include "lexer.h"
#include "parserdef.h"

#define NO_RULES 141
#define NO_TERMS 58
#define NO_NONTERMS 72
#define MAXTERMLEN 31

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS - 1];
struct ListNode* synchronizingSet[NO_NONTERMS];

void createSynchronizingSet(){
    for(int i=0; i<NO_NONTERMS; i++) {
        struct ListNode* C = follow[i];
	struct ListNode* temp synchronizingSet[i];
	while(C!=NULL){
            ListNode* newnode = (ListNode *)malloc(sizeof(ListNode));
    	    newnode -> val = C->val;
    	    newnode -> next = NULL;
	    temp=newnode;
	    C=C->next;
	    temp=temp->next;
        }
    }
    //rest is hardcoding
}

int contains(ListNode* ln, tokentype term) {
    if (ln == NULL)
        return 0;
    for (ListNode* curr = ln; curr != NULL; curr = curr -> next) {
        if (curr -> val.t == TERMINAL && curr -> val.g.t == term)
            return 1;
    }
    return 0;
}

void pushRule(int rule, StackNode* S) {
	//find the LHS of the rule in the grammar
	struct ListNode* LHS = grammar[rule]->next;
	//create treenode of first grammarchar
	TreeNode* temp= (TreeNode*)malloc(sizeof(TreeNode));
	if(LHS->val.t==TERMINAL){
		temp->t=TERMINAL;
		temp->ruleno=rule;
		//temp->val.t = TK_EPS;
		temp->child = NULL;
		temp->sibling=NULL;
	}
	else {
		temp->t=NONTERMINAL;
		temp->ruleno=rule;
		temp->val.t = LHS->val.g.nt;
		temp->child = NULL;
		temp->sibling=NULL;
	}
	StackNode* auxilliary;
	//push pointer to a seperate stack
	push(auxilliary, temp);
	//child pointer of top of stack points to first grammarchar
	X->child=temp;
	//go through rest of LHS, creating treeNodes, pointing prev sibling pointer to it, pushing to stack and continue
	while(LHS->next!=NULL){
		TreeNode* nextNode = (TreeNode*)malloc(sizeof(TreeNode));
		if(LHS->next->val.t==TERMINAL){
			nextNode->t=TERMINAL;
			nextNode->ruleno=rule;
			//nextNode->val.t = TK_EPS;
			nextNode->child = NULL;
			nextNode->sibling=NULL;
		}
		else {
			nextNode->t=NONTERMINAL;
			nextNode->ruleno = rule;
			nextNode->val.t = LHS->next->val.g.nt;
			nextNode->child = NULL;
			nextNode->sibling=NULL;
		}
		temp->sibling = nextNode;
		push(auxilliary, nextNode);
		temp=temp->sibling;
		LHS=LHS->next;
	}
	//pop from auxilliary stack and push to main stack
	pop(S);
	while(isEmpty(auxilliary)==0){
		push(S,pop(auxilliary));
	}
}

TreeNode* parse(){
    createSynchronizingSet();
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
            if(parseTable[X->val.g.nt][L->type]>=0){
                X->ruleno = parseTable[X->val.g.nt][L->type];
                pushRule(X->ruleno, S);
            }

            else if(contains(synchronizingSet[X->val.g.t],L.type)){
                pop(S);
            }

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
