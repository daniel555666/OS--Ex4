#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef test
#define test
#include "Ex4.h"
#endif

typedef struct _NODE_ {
    char* txt;
    struct _NODE_ *next;
} stack, *pstack;
pstack newNode(char *newText)
{
    pstack st = (pstack)malloc(sizeof(stack));
    st->txt = (char*)malloc(sizeof(char)*MAXDATASIZE);
    strcpy(st->txt, newText);
    st->next = NULL;
    return st;
}
void PUSH(pstack* st, char* txt)
{
    pthread_mutex_lock(&lock);
    pstack newHead = newNode(txt);
    newHead->next = *st;
    *st = newHead;
    pthread_mutex_unlock(&lock);
}
int IsEmpty(pstack st)
{
    return !st;
}
char* TOP(pstack st)
{
    if (IsEmpty(st))
    {
        perror("the stack is empty");
    }
    char *temp = (char*)malloc(sizeof(char)*MAXDATASIZE);
    strcpy(temp, st->txt);
    return temp;
}
char* POP(pstack *st)
{
    pthread_mutex_lock(&lock);
    if (IsEmpty(*st))
    {
        perror("the stack is empty");
    }
    pstack temp = *st;
    *st = (*st)->next;
    char* popped = (char*)malloc(sizeof(char)*MAXDATASIZE);
    strcpy(popped, temp->txt);
    free(temp->txt);
    free(temp);
    pthread_mutex_unlock(&lock);
    return popped;
}