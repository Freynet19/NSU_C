#include <stdio.h>
#include <string.h>
#include <limits.h>

#define HALFSIZE (20) // size of half of the text; any number above patlen
#define INP stdin

typedef unsigned char uchar;

void CreateBadCharTable(uchar* pat, int* bct) {
    int len = (int)strlen((char*)pat);
    for (int c = 0; c <= UCHAR_MAX; ++c) {
        bct[c] = len;
    }
    for (int i = 0; i < len - 1; ++i) {
        bct[pat[i]] = len - i - 1;
    }
}

void SearchAndPrintBM(uchar const* pat, uchar const* text, int* start,  int shift, int patlen, int textlen, int const* bct) {
    while (*start <= HALFSIZE + patlen && *start < textlen) {
        int i = 0;
        while (1) {
            uchar c = text[*start - i];
            printf("%d ", (*start - i + 1) + shift);
            if (c != pat[patlen - 1 - i]) {
                *start += bct[text[*start]];
                break;
            }
            ++i;
            if (i >= patlen) {
                *start += patlen;
                break;
            }
        }
    }
}

int main(void) {

    uchar pat[16+1] = {0};
    uchar text1[HALFSIZE*2] = {0}; // pointer to 1st half of the text
    uchar *text2 = text1 + HALFSIZE; // pointer to 2nd half of the text

    if (fscanf(INP, "%16[^\n]", pat) != 1) {
        printf("bad input\n");
        return 0;
    }
    getc(INP);

    int bct[UCHAR_MAX+1] = {0};
    CreateBadCharTable(pat, bct);

    int patlen = (int)strlen((char*)pat);
    int textlen = (int)fread(text1, sizeof(char), sizeof(text1), INP);

    int shift = 0;
    int start = patlen - 1;

    while(1) {
        SearchAndPrintBM(pat, text1, &start, shift, patlen, textlen, bct);
        start -= HALFSIZE;
        shift += HALFSIZE;
        memmove(text1, text2, HALFSIZE);
        if (textlen < HALFSIZE*2) break;
        else textlen = HALFSIZE + fread(text2, sizeof(char), HALFSIZE*sizeof(char), INP);
    }

    printf("\n");
    return 0;
}
