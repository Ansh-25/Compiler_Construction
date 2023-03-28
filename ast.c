#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "astDef.h"

func makenode()

ASTNode* astroot;

// makenode will be implemented separately within each if else case

// void printAST(ASTNode* root){
//     if(root==NULL) return;
//     printf(root->ruleno);

// }

ASTList* insertAtEnd(ASTList* head1,ASTList* head2){
    if(head1==NULL) return head1 = head2;
    if(head2 == NULL) return head1;
    while(head1->next != NULL) head1 = head1 -> next;
    head1 -> next = head2;
    return head1;
}

makeAST(parserNode){
    switch(parserNode->ruleno) {
        case 0:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            astroot = (ASTNode*)malloc(sizseof(ASTNode));
            astroot->label = PROGRAM_NODE;
            astroot->moduleDeclarationsList = parserNode->child->addr;
            astroot->moduleList = parserNode->child->sibling->addr;
            insertAtEnd(astroot->moduleList, parserNode->child->sibling->sibling->addr);
            insertAtEnd(astroot->moduleList, parserNode->child->sibling->sibling->sibling->addr);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            free(parserNode);
            break;

        default:
            printf("Bye\n");
    }
    // if (root->ruleno == 1) {
    //     inherited semantic rules
    //     visit required children e.g. makeAST(root -> idList)
    //     synthesized symantic rules
    // }
    // else if (root->ruleno == 2) {
    //     inherited semantic rules
    //     visit required children e.g. makeAST(root -> idList)
    //     synthesized symantic rules
    // }
    // and so on
}

AST(){
	TreeNode* parserNode = parse();
	makeAST(parserNode);
	return astRoot;
}

int main(){
    AST();
    printf("hi\n");
    return 0;
}