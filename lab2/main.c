#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INP stdin

void BadInputExit(void) {
    printf("bad input\n");
    exit(0);
}

void CheckInput(char const* p, int len) {
    if (len > 10) BadInputExit();
    for (int i = 0; i < len; ++i) {
        if (p[i] < '0' || p[i] > '9') BadInputExit();
        for (int j = 0; j < i; ++j) {
            if (p[j] == p[i]) BadInputExit();
        }
    }
}

void Swap(char* a, char* b) {
    char t = *a;
    *a = *b;
    *b = t;
}

void Reverse(char* arr, int start, int end) {
    while (start <= end) {
        Swap(&arr[start], &arr[end]);
        ++start;
        --end;
    }
}

int DecreasingSuffixLen(char const* p, int len) {
    int i;
    for (i = 1; i < len; ++i) {
        if (p[len-i-1] < p[len-i]) break;
    }
    return i;
}

int IndexOfMinAfterTurnPoint(char const* p, int len, int turnpoint) {
    int i;
    for (i = len-1; i > 0; --i) {
        if (p[i] > turnpoint) break;
    }
    return i;
}

int NextPermutation(char* p, int len) {
    int suflen = DecreasingSuffixLen(p, len);
    if (suflen == len) return 0;

    int tp_ind = len-suflen-1;
    Swap(&p[tp_ind], &p[IndexOfMinAfterTurnPoint(p, len, p[tp_ind])]);
    // p[len-suflen-1] is a Turn-Point
    // MinATP is the smallest element above the Turn-Point

    Reverse(p, tp_ind+1, len-1);
    return 1;
}

int main(void) {

    char p[10+2] = {0}; // 1 extra char for '\0' and 1 extra char for check of length
    if (fscanf(INP, "%11[^\n]", p) != 1) BadInputExit();
    int n;
    if (fscanf(INP, "%d", &n) != 1) BadInputExit();

    int len = (int)strlen(p);
    CheckInput(p, len);

    for (int cnt = 0; cnt < n; ++cnt) {
        if (NextPermutation(p, len)) printf("%s\n", p);
        else break;
    }

    return 0;
}
