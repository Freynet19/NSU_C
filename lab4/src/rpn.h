#pragma once

typedef struct RPNItem RPNItem;
typedef struct RPN RPN;

struct RPNItem {
    struct RPNItem *prev, *next;
    int value;
    char type;
};

struct RPN {
    RPNItem *head, *tail;
};

RPN *InitPRN(void);
void RPNPush(RPN *rpn, int item, char type);
int RPNPop(RPN *rpn, RPNItem *item);
int RPNOperation(RPN *rpn, RPNItem *item, int x);
void FreeRPN(RPN *rpn);
