#include <stdio.h>
#include "huffman_encode.h"
#include "huffman_decode.h"

int main(void) {
    FILE *in = fopen("in.txt", "rb");
    FILE *out = fopen("out.txt", "wb");
    int c = getc(in);
    if (c == 'c') Encode(in, out);
    else if (c == 'd') Decode(in, out);
    fclose(in);
    fclose(out);
    return 0;
}
