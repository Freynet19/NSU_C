#include "rpn.h"
#include <stdlib.h>

RPN *InitPRN(void) {
    RPN *rpn = malloc(sizeof(RPN));
    rpn->head = NULL;
    rpn->tail = NULL;
    return rpn;
}

void FreeRPNItem(RPNItem *item) {
    if (item == NULL) return;
    FreeRPNItem(item->next);
    free(item);
}

void FreeRPN(RPN *rpn) {
    FreeRPNItem(rpn->head);
    free(rpn);
}

RPNItem *NewRPNItem(int value, char type) {
    RPNItem *new = malloc(sizeof(RPNItem));
    new->next = NULL;
    new->value = value;
    new->type = type;
    return new;
}

void RPNPush(RPN *rpn, int value, char type) {
    RPNItem *new = NewRPNItem(value, type);
    if (rpn->tail != NULL) {
        rpn->tail->next = new;
        new->prev = rpn->tail;
        rpn->tail = new;
    }
    else {
        rpn->head = new;
        rpn->tail = new;
    }
}

int RPNPop(RPN *rpn, RPNItem *item) {
    RPNItem *prev = item->prev;
    RPNItem *next = item->next;
    if (item == rpn->head) rpn->head = next;
    if (item == rpn->tail) rpn->tail = prev;
    if (prev != NULL) prev->next = next;
    if (next != NULL) next->prev = prev;
    int num = item->value;
    free(item);
    return num;
}

int RPNOperation(RPN *rpn, RPNItem *item, int x) {
    char op = (char)RPNPop(rpn, item->next);
    switch (op) {
        case '+':
            item->value += x;
            break;
        case '-':
            item->value -= x;
            break;
        case '*':
            item->value *= x;
            break;
        case '/':
            if (x == 0) return -1;
            item->value /= x;
            break;
        default: break;
    }
    return 0;
}
