#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HALFSIZE (20) // size of half of the text; any number above patlen
#define INP stdin

typedef unsigned char uchar;

int ComputeHash(uchar const* s, int len) {
    int h = 0, t = 1;
    for (int i = 0; i < len; ++i) {
        h += (s[i] % 3) * t;
        t *= 3;
    }
    return h;
}

int UpdateHash(uchar c0, uchar cn, int hash, int t3) {
    return (hash - (c0 % 3)) / 3 + (cn % 3) * t3;
}

void PowT3(int* t3, int n) {
    for (int i = 1; i < n; ++i) {
        *t3 *= 3;
    }
}

void SearchAndPrintRK(int hash, int* strhash, uchar const* pat, uchar* text, uchar* str, int* start, int shift, int patlen, int textlen, int t3) {
    while (*start <= HALFSIZE + patlen && *start < textlen) {
        if (*start >= patlen) *strhash = UpdateHash(text[*start-patlen], text[*start], *strhash, t3);
        else *strhash = ComputeHash(str, patlen);
        if (hash == *strhash) {
            int pos = *start-patlen+1;
            memcpy(str, text+pos, patlen*sizeof(char));
            for (int i = 0; i < patlen; ++i) {
                printf("%d ", pos+i+shift+1);
                if (str[i] != pat[i]) break;
            }
        }
        *start += 1;
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

    int patlen = (int)strlen((char*)pat);
    int textlen = (int)fread(text1, sizeof(char), sizeof(text1), INP);

    int hash = ComputeHash(pat, patlen);
    printf("%d ", hash);

    int t3 = 1;
    PowT3(&t3, patlen);

    int shift = 0;
    int start = patlen - 1;

    uchar* str = calloc(patlen+1, sizeof(char));
    memcpy(str, text1, patlen*sizeof(char));
    int strhash = ComputeHash(str, patlen);

    while (1) {
        SearchAndPrintRK(hash, &strhash, pat, text1, str, &start, shift, patlen, textlen, t3);
        start -= HALFSIZE;
        shift += HALFSIZE;
        memmove(text1, text2, HALFSIZE);
        if (textlen < HALFSIZE*2) {
            free(str);
            break;
        }
        else textlen = HALFSIZE + fread(text2, sizeof(char), HALFSIZE*sizeof(char), INP);
    }

    printf("\n");
    return 0;
}
