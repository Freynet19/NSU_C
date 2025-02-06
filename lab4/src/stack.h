#pragma once

typedef struct StackItem StackItem;
typedef struct Stack Stack;

struct StackItem {
    struct StackItem *next;
    char op; // operation sign
    char pr; // priority of operation
};

struct Stack {
    StackItem *head;
};

char OpPriority(char op);
Stack *InitStack(void);
void StackPush(Stack *st, char op);
char StackPop(Stack *st);
void FreeStack(Stack *st);
