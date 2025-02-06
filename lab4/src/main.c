#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "rpn.h"

enum { SYNTAX_ERROR = 1, DIVISION_BY_ZERO = 2 };

void ErrorExit(char code) {
    switch(code) {
        case 1: printf("syntax error\n");     break;
        case 2: printf("division by zero\n"); break;
        default: break;
    }
    exit(0);
}

void RPN_SynErr(Stack *st, RPN *rpn) {
    FreeStack(st);
    FreeRPN(rpn);
    ErrorExit(SYNTAX_ERROR);
}

enum { NUMBER, OPERATION, BRACE, ERROR };

char TypeOf(char c) {
    if ('0' <= c && c <= '9') return NUMBER;
    if (c == '+' || c == '-' ||
        c == '*' || c == '/') return OPERATION;
    if (c == '(' || c == ')') return BRACE;
    return ERROR;
}

void RPNPushFromStack(Stack *st, RPN *rpn) {
    char op = StackPop(st);
    RPNPush(rpn, op, OPERATION);
}

RPN *PostfixNotation(const char *input) {
    Stack *st = InitStack();
    RPN *rpn = InitPRN();

    if (input[0] == '-') RPNPush(rpn, 0, NUMBER);

    int len = (int)strlen(input);
    char prev_type = -1;
    for (int i = 0; i < len; ++i) {
        char c = input[i];
        char type = TypeOf(c);
        switch (type) {
            case NUMBER:
                c -= '0';
                if (prev_type == NUMBER) {
                    rpn->tail->value *= 10;
                    rpn->tail->value += c;
                }
                else RPNPush(rpn, c, NUMBER);
                break;

            case OPERATION:
                if (prev_type == OPERATION) RPN_SynErr(st, rpn);
                while (st->head != NULL && st->head->pr >= OpPriority(c)) {
                    RPNPushFromStack(st, rpn);
                }
                StackPush(st, c);
                break;

            case BRACE:
                if (c == '(') {
                    if (prev_type == NUMBER) RPN_SynErr(st, rpn);
                    StackPush(st, c);
                }
                else {
                    if (rpn->head == NULL) {
                        if (prev_type == OPERATION || prev_type == -1) RPN_SynErr(st, rpn);
                        if (st->head == NULL || st->head->op == '(') RPN_SynErr(st, rpn);
                    }
                    while (1) {
                        if (st->head == NULL) RPN_SynErr(st, rpn);
                        if (st->head->op == '(') break;
                        RPNPushFromStack(st, rpn);
                    }
                    StackPop(st);
                }
                break;

            default:
                RPN_SynErr(st, rpn);
                break;
        }
        prev_type = type;
    }
    while (st->head != NULL) {
        RPNPushFromStack(st, rpn);
    }
    if (rpn->head->type != NUMBER) RPN_SynErr(st, rpn);
    free(st);
    return rpn;
}

int Calculate(const char *input) {
    RPN *rpn = PostfixNotation(input);

    while (rpn->head != rpn->tail) {
        if (rpn->head->next == rpn->tail) {
            FreeRPN(rpn);
            ErrorExit(SYNTAX_ERROR);
        }

        RPNItem *cur = rpn->head;
        while (cur->type != OPERATION) {
            cur = cur->next;
        }

        int x = RPNPop(rpn, cur->prev);
        if (RPNOperation(rpn, cur->prev, x) != 0) {
            FreeRPN(rpn);
            ErrorExit(DIVISION_BY_ZERO);
        }
    }

    int ans = rpn->head->value;
    FreeRPN(rpn);
    return ans;
}

int main(void) {
    char input[1001] = {0};
    if (scanf("%1000[^\n]", input) != 1) ErrorExit(SYNTAX_ERROR);
    printf("%d\n", Calculate(input));
    return 0;
}
