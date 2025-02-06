#include "stack.h"
#include <stdlib.h>

Stack *InitStack(void) {
    Stack *st = malloc(sizeof(Stack));
    st->head = NULL;
    return st;
}

void FreeStackItem(StackItem *item) {
    if (item == NULL) return;
    FreeStackItem(item->next);
    free(item);
}

void FreeStack(Stack *st) {
    FreeStackItem(st->head);
    free(st);
}

char OpPriority(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

StackItem *NewStackItem(StackItem *next, char op) {
    StackItem *new = malloc(sizeof(StackItem));
    new->next = next;
    new->op = op;
    new->pr = OpPriority(op);
    return new;
}

void StackPush(Stack *st, char op) {
    st->head = NewStackItem(st->head, op);
}

char StackPop(Stack *st) {
    StackItem *next = st->head->next;
    char op = st->head->op;
    free(st->head);
    st->head = next;
    return op;
}
