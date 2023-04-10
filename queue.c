#include <stdlib.h>
#include "symbolTableDef.h"

typedef struct QueueNode{
    ModuleTableEntry* var;
    struct Quadruple* label;
    struct QueueNode* next;
}QueueNode;

QueueNode* enqueue(QueueNode* head, QueueNode* newNode){
    if(head==NULL){
        newNode->next = NULL;
        return newNode;
    }
    QueueNode* temp = head;
    while(temp->next!=NULL){
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->next = NULL;
    return head;
}

QueueNode* dequeue(QueueNode* head){
    if(head==NULL){
        return NULL;
    }
    QueueNode* temp = head->next;
    free(head);
    return temp;
}

bool is_empty(QueueNode* head){
    return head==NULL;
}
