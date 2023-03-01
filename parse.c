#include "ff.h"


struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS - 1];
struct ListNode* synchronizingSet[NO_NONTERMS];

void createSynchronizingSet(){

    ListNode* follow_set = NULL;
    for (int i = 0; i < NO_NONTERMS; i ++) //pushes all elements of the follow set into the respective synchronizing set
        for (follow_set = follow[i]; follow_set != NULL; follow_set = follow_set -> next) 
            synchronizingSet[i] = insertlast(synchronizingSet[i], follow_set -> val);
   
    ListNode* first_set = NULL;
    for (int i = 0; i < NO_NONTERMS; i ++) //pushes all elements of the first set into the respective synchronizing set
        for (first_set = first[i]; first_set != NULL; first_set = first_set -> next)
            synchronizingSet[i] = insertlast(synchronizingSet[i], first_set -> val);

    for (int i = 0; i < NO_NONTERMS; i ++){ //pushes ;, >>, >>>, END, EOF into each synchronizing set
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


StackNode* pushrule(int rule, StackNode* S){
	StackNode* aux = NULL; //creates auxilliary stack
	ListNode* x = grammar[rule]->next; //takes LHS of the grammar rule
    TreeNode* prev = (TreeNode*)malloc(sizeof(TreeNode));

    while(x!=NULL){
        TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
        if(x==grammar[rule]->next){//if this is the first node of the LHS of the rule, then we point the child pointer of the parent(i.e. top of the main stack) to it

            top(S)->child = temp;
        }
        temp->t = x->val.t; //initialise the TreeNode with information of the grammar character depending on whether it is a terminal or non-terminal
        if(x->val.t==TERMINAL){
            temp->val.t.type = x->val.g.t;
        }
        else
            temp->val.nt = x->val.g.nt;
        temp->ruleno = rule;
        temp->child = NULL;
        temp->sibling = NULL;
        prev->sibling = temp; //point the previous node's sibling pointer to the current TreeNode
        prev = temp;
        aux = push(aux,temp); //push to auxilliary stack
        x = x->next;
    }
    S = pop(S); //remove the top of the main stack
    while(isEmpty(aux) == 0){ //add the LHS of the rule to the main stack. We add it from the auxilliary stack since we need it to be pushed from right to left
        S = push(S,top(aux));
        aux = pop(aux);
    }
    return S;
}

void printStack(StackNode* S) { //prints the stack
    StackNode* curr = S;
    while (curr != NULL) { //loop until end of stack
        if (curr -> val -> t == TERMINAL) //if stack contains the terminal, print that
            printf("TERMINAL: %d ",curr->val->val.t.type);
        else //else print it as a non-terminal
            printf("NON-TERMINAL: %d ",curr->val->val.nt);
        curr = curr -> next;
    }
    printf("\n");
}

TreeNode* parse(){
    createSynchronizingSet(); //first create the synchronizing set for all non-terminals
    StackNode* S = NULL; //create stack
    TreeNode* Eof = (TreeNode*)malloc(sizeof(TreeNode)); //create EOF TreeNode
    Eof->val.t.type = TK_EOF;
    Eof->t = TERMINAL;
    Eof->sibling = NULL;
    Eof->child = NULL;
    TreeNode* Root = (TreeNode*)malloc(sizeof(TreeNode)); //create Root TreeNode
    Root->val.nt = program; //our start symbol is program
    Root->t = NONTERMINAL;
    Root->ruleno = 0;
    Root->sibling = NULL;
    TreeNode* X = Root;
    S = push(S,Eof); //push into stack, EOF at bottom then program
    S = push(S,X);
    struct Token* L = getNextToken(); //start at the first token
    while(L!=NULL){
         //printStack(S);
        //printToken(L);
        //printf("hi");
        if(isEmpty(S)){ //if stack is empty and we still have a token, it means there is an error
            printf("\n Syntax Error at line no %d ... empty stack\n",L->lineNo); 
            break;
        }
        //printf("hi");
        X = top(S); //get top of stack
        if(X->t==TERMINAL){ //if top of stack is terminal,
            if(X->val.t.type == L->type){
                X->t = TERMINAL; //we convert the treenode to leafnode,
                X->val.t = *L;
                X->ruleno = -1;
                X->child = NULL;
                X->sibling = NULL;
                S = pop(S); //pop the terminal at the top of the stack
                if (L -> type == TK_EOF) { //if we reach the end of the file, then break
                    break;
                }
                L = getNextToken(); //get next token
                if (L == NULL) { //if we are unable to get next token, then we convert it to EOF token
                    L = malloc(sizeof(struct Token));
                    L -> type = TK_EOF;
                }
            }
            else{ //if we the top of the stack is a different terminal to the token, then we have an error
               
                printf("\n Syntax Error at line no %d ... terminal mismatch\n",L->lineNo);
                printf("actual : %d\n",L->type);
                printf("exp : %d\n",X->val.t.type);
                S = pop(S);
            }
        }
        else if(X->t==NONTERMINAL){ //else if top of stack is a non-terminal
            //printf("hi");
            if(parseTable[X->val.nt][L->type]>=0){ //check the parse table for the right rule
                X->ruleno = parseTable[X->val.nt][L->type];
                //X->child = grammar[parseTable[X->val.nt][L->type]]->next;
                if(grammar[parseTable[X->val.nt][L->type]]->next->val.t==TERMINAL && grammar[parseTable[X->val.nt][L->type]]->next->val.g.t==EPS)
                    S = pop(S); //if the rule derives epsilon, just pop S
                else 
                    S = pushrule(X->ruleno, S); //otherwise push the LHS of the rule (the pushrule function pops S on its own at the end)
                // printStack(S);
            }

            else if(contains(synchronizingSet[X->val.t.type],L->type)){ //otherwise if we can't find the right rule, check if the non-terminal contains the top of the stack in the synchronizing set. if yes, we can pop the stack and continue
                printf("\n Syntax Error at line no %d ... non-terminal mismatch, popping stack\n",L->lineNo);
                S = pop(S);
            }

            else { //otherwise get next token
                printf("\n Syntax Error at line no %d ... non-terminal mismatch, getting new token\n",L->lineNo);
                if (L -> type == TK_EOF) { //if L is TK_EOF, we can't get next token, so we just break
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
    if(!isEmpty(S)){ //if stack is not empty after consuming all tokens, we have an error
        printStack(S);
        printf("Error ... stack not empty yet");
    }
    return Root;
}

void printTreeInOrder(TreeNode* root) {
    if (root == NULL)
        return;
    if (root -> t == TERMINAL) {
        printf("Terminal %s with token value ", mapttokentostring(root -> val.t.type));
        if (root -> val.t.type == TK_NUM)
            printf("%d\n", root -> val.t.val.integer);
        else if (root -> val.t.type == TK_RNUM)
            printf("%lf\n", root -> val.t.val.decimal);
        else
            printf("%s\n", root -> val.t.val.identifier);
    }
    else {
        TreeNode* curr = root -> child;
        while (curr != NULL) {
            printTreeInOrder(curr);
            curr = curr -> sibling;
        }
    }
}

void printTree(TreeNode* root) {
    if(root==NULL) return;
    TreeNode* curr = root->child;
    printTree(root->child);
    if(root->t==TERMINAL && root->val.t.type==TK_NUM) printf("Terminal %s with token value %d\n", mapttokentostring(root->val.t.type),root->val.t.val.integer);
    else if(root->t==TERMINAL && root->val.t.type==TK_RNUM) {printf("Terminal %s with token value %lf\n", mapttokentostring(root->val.t.type),root->val.t.val.decimal);}
    else if(root->t==TERMINAL) {printf("Terminal %s with token value %s\n", mapttokentostring(root->val.t.type),root->val.t.val.identifier);}
    else printf("Non-Terminal %s\n",mapnttostring(root->val.nt));
    if (curr != NULL)
        curr=curr->sibling;
    for(;curr!=NULL; curr=curr->sibling){
        printTree(curr);
    }
}

int main(){
    ptr = initLexer(ptr,32);
    loadgrammar("GrammarForParser");
    computefirstandfollow();
    createParseTable();
    //printf("%d",parseTable[0][10]);
    TreeNode* root = parse();
    printTreeInOrder(root);
    return 0;
}
