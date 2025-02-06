#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HALFSIZE (20)
#define INP stdin

typedef unsigned char uchar;

void CreatePrefixTable(uchar const* pat, int patlen, int* preftab) {
    for (int i = 1; i < patlen; ++i) {
        for (int k = 1; k <= i; ++k) {
            int match = 1;
            for (int j = 0; j < k; ++j) {
                if (pat[j] != pat[i+j-k+1]) {
                    match = 0;
                    break;
                }
            }
            if (match) preftab[i] = k;
        }
    }
}

void SearchAndPrintKMP(uchar const* pat, uchar const* text, int* i, int* j, int shift, int patlen, int textlen, int const* preftab) {
    while (textlen - *i >= patlen - *j && *i < HALFSIZE*2) {
        if (pat[*j] == text[*i] && *j < patlen) {
            ++*i;
            ++*j;
        }
        else if (*j > 0) {
            printf("%d %d ", *i+shift-*j+1, *j);
            *j = preftab[*j-1];
        }
        else ++*i;
    }
}

int main(void) {

    uchar pat[16+1] = {0};
    uchar text1[HALFSIZE*2] = {0};
    uchar *text2 = text1 + HALFSIZE;

    if (fscanf(INP, "%16[^\n]", pat) != 1) {
        printf("\n");
        return 0;
    }
    getc(INP);

    int patlen = (int)strlen((char*)pat);
    int textlen = (int)fread(text1, sizeof(char), sizeof(text1), INP);
    int *preftab = calloc(patlen, sizeof(int));
    CreatePrefixTable(pat, patlen, preftab);
    for (int i = 0; i < patlen; ++i) {
        printf("%d ", preftab[i]);
    }
    printf("\n");

    if (textlen < patlen) {
        free(preftab);
        return 0;
    }

    int shift = 0, i = 0, j = 0;

    while (1) {
        SearchAndPrintKMP(pat, text1, &i, &j, shift, patlen, textlen, preftab);
        i -= HALFSIZE;
        shift += HALFSIZE;
        memmove(text1, text2, HALFSIZE*sizeof(char));
        if (textlen < HALFSIZE*2) break;
        else textlen = HALFSIZE + fread(text2, sizeof(char), HALFSIZE*sizeof(char), INP);
    }

    free(preftab);
    printf("\n");
    return 0;
}
