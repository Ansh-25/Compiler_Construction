#include "ff.h"


struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS - 1];
struct ListNode* synchronizingSet[NO_NONTERMS];

void createSynchronizingSet(){

    ListNode* follow_set = NULL;
    for (int i = 0; i < NO_NONTERMS; i ++) 
        for (follow_set = follow[i]; follow_set != NULL; follow_set = follow_set -> next)
            synchronizingSet[i] = insertlast(synchronizingSet[i], follow_set -> val);
   
    ListNode* first_set = NULL;
    for (int i = 0; i < NO_NONTERMS; i ++) 
        for (first_set = first[i]; first_set != NULL; first_set = first_set -> next)
            synchronizingSet[i] = insertlast(synchronizingSet[i], first_set -> val);

    for (int i = 0; i < NO_NONTERMS; i ++){
		grammarchar semicol; semicol.t = TERMINAL; semicol.g.t = TK_SEMICOLON;
        synchronizingSet[i] = insertlast(synchronizingSet[i], semicol);
		grammarchar enddef; enddef.t = TERMINAL; enddef.g.t = TK_ENDDEF;
        synchronizingSet[i] = insertlast(synchronizingSet[i], enddef);
		grammarchar enddriverdef; enddriverdef.t = TERMINAL; enddriverdef.g.t = TK_DRIVERENDDEF;
        synchronizingSet[i] = insertlast(synchronizingSet[i], enddriverdef);
		grammarchar end; end.t = TERMINAL; end.g.t = TK_END;
        synchronizingSet[i] = insertlast(synchronizingSet[i], end);
		grammarchar eof; eof.t = TERMINAL; eof.g.t = TK_EOF;
        synchronizingSet[i] = insertlast(synchronizingSet[i], eof);
	}
}



// void pushRule(int rule, StackNode* S) {
// 	//find the LHS of the rule in the grammar
// 	struct ListNode* LHS = grammar[rule]->next;
// 	//create treenode of first grammarchar
// 	TreeNode* temp= (TreeNode*)malloc(sizeof(TreeNode));
// 	if(LHS->val.t==TERMINAL){
// 		temp->t=TERMINAL;
// 		temp->ruleno=rule;
// 		temp->val.t.type=LHS->val.g.t;
// 		temp->child = NULL;
// 		temp->sibling=NULL;
// 	}
// 	else {
// 		temp->t=NONTERMINAL;
// 		temp->ruleno=rule;
// 		temp->val.nt =LHS->val.g.nt;
// 		temp->child = NULL;
// 		temp->sibling=NULL;
// 	}
// 	StackNode* auxilliary;
// 	//push pointer to a seperate stack
// 	auxilliary = push(auxilliary, temp);
// 	//child pointer of top of stack points to first grammarchar
// 	top(S)->child=temp;
// 	//go through rest of LHS, creating treeNodes, pointing prev sibling pointer to it, pushing to stack and continue
// 	while(LHS->next!=NULL){
// 		TreeNode* nextNode = (TreeNode*)malloc(sizeof(TreeNode));
// 		if(LHS->next->val.t==TERMINAL){
// 			nextNode->t=TERMINAL;
// 			nextNode->ruleno=rule;
// 			nextNode->val.t.type = LHS->next->val.g.t;
// 			nextNode->child = NULL;
// 			nextNode->sibling=NULL;
// 		}
// 		else {
// 			nextNode->t=NONTERMINAL;
// 			nextNode->ruleno = rule;
// 			nextNode->val.nt = LHS->next->val.g.nt;
// 			nextNode->child = NULL;
// 			nextNode->sibling=NULL;
// 		}
// 		temp->sibling = nextNode;
// 		push(auxilliary, nextNode);
// 		temp=temp->sibling;
// 		LHS=LHS->next;
// 	}
// 	//pop from auxilliary stack and push to main stack
// 	S = pop(S);
// 	while(!isEmpty(auxilliary)){
// 		S = push(S,top(auxilliary));
//       auxilliary =  pop(auxilliary);
// 	}
// }

StackNode* pushrule(int rule, StackNode* S){
	StackNode* aux = NULL;
    //printf("%d",grammar[rule]==NULL);
	ListNode* x = grammar[rule]->next;
    TreeNode* prev = (TreeNode*)malloc(sizeof(TreeNode));

    while(x!=NULL){
        TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
        if(x==grammar[rule]->next){

            top(S)->child = temp;
            //printf("%d",top(S)->child->val.nt==1);
        }
        temp->t = x->val.t;
        if(x->val.t==TERMINAL){
            temp->val.t.type = x->val.g.t;
        }
        else
            temp->val.nt = x->val.g.nt;
        temp->ruleno = rule;
        temp->child = NULL;
        temp->sibling = NULL;
        prev->sibling = temp;
        prev = temp;
        aux = push(aux,temp);
        x = x->next;
    }
    S = pop(S);
    while(isEmpty(aux) == 0){
        S = push(S,top(aux));
        aux = pop(aux);
    }
    return S;
}

void printStack(StackNode* S) {
    StackNode* curr = S;
    while (curr != NULL) {
        if (curr -> val -> t == TERMINAL)
            printf("TERMINAL: %d ",curr->val->val.t.type);
        else
            printf("NON-TERMINAL: %d ",curr->val->val.nt);
        curr = curr -> next;
    }
    printf("\n");
}

TreeNode* parse(){
    createSynchronizingSet();
    StackNode* S = NULL;
    TreeNode* Eof = (TreeNode*)malloc(sizeof(TreeNode));
    Eof->val.t.type = TK_EOF;
    Eof->t = TERMINAL;
    Eof->sibling = NULL;
    Eof->child = NULL;
    TreeNode* Root = (TreeNode*)malloc(sizeof(TreeNode));
    Root->val.nt = program;
    Root->t = NONTERMINAL;
    Root->ruleno = 0;
    Root->sibling = NULL;
    TreeNode* X = Root;
    S = push(S,Eof);
    S = push(S,X);
    //printf("%d",top(S)->val.nt);
    struct Token* L = getNextToken();
    while(L!=NULL){
         //printStack(S);
        //printToken(L);
        //printf("hi");
        if(isEmpty(S)){
            printf("\n Syntax Error at line no %d ... empty stack\n",L->lineNo);
            break;
        }
        //printf("hi");
        X = top(S);
        if(X->t==TERMINAL){
            if(X->val.t.type == L->type){
                X->t = TERMINAL;
                X->val.t = *L;
                X->ruleno = -1;
                X->child = NULL;
                X->sibling = NULL;
                S = pop(S);
                if (L -> type == TK_EOF) {
                    break;
                }
                L = getNextToken();
                if (L == NULL) {
                    L = malloc(sizeof(struct Token));
                    L -> type = TK_EOF;
                }
            }
            else{
               
                printf("\n Syntax Error at line no %d ... terminal mismatch\n",L->lineNo);
                printf("actual : %d\n",L->type);
                printf("exp : %d\n",X->val.t.type);
                S = pop(S);
            }
        }
        else if(X->t==NONTERMINAL){
            //printf("hi");
            if(parseTable[X->val.nt][L->type]>=0){
                X->ruleno = parseTable[X->val.nt][L->type];
                //X->child = grammar[parseTable[X->val.nt][L->type]]->next;
                if(grammar[parseTable[X->val.nt][L->type]]->next->val.t==TERMINAL && grammar[parseTable[X->val.nt][L->type]]->next->val.g.t==EPS)
                    S = pop(S);
                else 
                    S = pushrule(X->ruleno, S);
                // printStack(S);
            }

            else if(contains(synchronizingSet[X->val.t.type],L->type)){
                printf("\n Syntax Error at line no %d ... non-terminal mismatch, popping stack\n",L->lineNo);
                S = pop(S);
            }

            else {
                printf("\n Syntax Error at line no %d ... non-terminal mismatch, getting new token\n",L->lineNo);
                if (L -> type == TK_EOF) {
                    break;
                }
                L = getNextToken();
                if (L == NULL) {
                    L = malloc(sizeof(struct Token));
                    L -> type = TK_EOF;
                }
            }
        }
        else    
            continue;
    }
    if(!isEmpty(S)){
        printStack(S);
        printf("Error ... stack not empty yet");
    }
    return Root;
}

int main(){
    ptr = initLexer(ptr,32);
    loadgrammar("GrammarForParser");
    computefirstandfollow();
    createParseTable();
    //printf("%d",parseTable[0][10]);
    parse();
    return 0;
}
