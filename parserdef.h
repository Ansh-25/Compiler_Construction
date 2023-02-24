#include "lexerDef.h"
#include <stdlib.h>

typedef enum {program,moduleDeclarations}non_terminal;

union grammaritem {
    non_terminal nt;
    tokentype t;
};

struct grammarchar {
    union grammaritem g;
    int type;
};

struct ListNode {
    struct grammarchar val;
    struct ListNode* next;
};

struct TreeNode {
    int test;
};

struct StackNode {
    struct TreeNode* val;
    struct StackNode* next;
};

void insert (struct ListNode* head, struct grammarchar gc) {
    struct ListNode* newnode = (struct ListNode *)malloc(sizeof(struct ListNode));
    newnode -> val = gc;
    newnode -> next = NULL;
    if (head == NULL)
        head = newnode;
    else {
        struct ListNode* curr = head;
        while (curr -> next != NULL)
            curr = curr -> next;
        curr -> next = newnode;
    }
}

void deletelast (struct ListNode* head) {
    if (head != NULL) {
        if (head -> next == NULL)
            head = NULL;
        struct ListNode* curr = head;
        while (curr -> next -> next != NULL)
            curr = curr -> next;
        curr -> next = NULL;
    }
}

int isEmpty (struct StackNode* top) {
    return top == NULL;
}

void push (struct StackNode* top, struct TreeNode* newnode) {
    struct StackNode* newstacknode = (struct StackNode*)malloc(sizeof(struct StackNode));
    newstacknode -> val = newnode;
    newstacknode -> next = top;
    top = newstacknode;
}

struct TreeNode* peek (struct StackNode* top) {
    if (isEmpty(top))
        return NULL;
    return top -> val;
}

struct TreeNode* pop (struct StackNode* top) {
    if (isEmpty(top))
        return NULL;
    struct StackNode* curr = top;
    top = top -> next;
    return curr -> val;
}