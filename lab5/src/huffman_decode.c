#include <stdlib.h>
#include "huffman_decode.h"

struct DecNode {
    struct DecNode *left, *right;
    uchar ch;
};

DecNode *NewDecNode(uchar ch) {
    DecNode *nd = malloc(sizeof(DecNode));
    nd->left = NULL, nd->right = NULL;
    nd->ch = ch;
    return nd;
}

int IsDecLeaf(DecNode *nd) {
    if (nd == NULL) return 0;
    return (nd->left == NULL && nd->right == NULL);
}

void CheckBitPos(BitBuffer *BB, FILE *in) {
    if (BB->pos == 8) {
        if (fread(&BB->byte, 1, 1, in) == 0) return;
        BB->pos = 0;
    }
}

int BitRead(BitBuffer *BB, FILE *in) {
    int bit = BB->byte >> (7 - BB->pos) & 1;
    BB->pos++;
    CheckBitPos(BB, in);
    return bit;
}

void FreeDecTree(DecNode *nd) {
    if (nd == NULL) return;
    if (nd->left != NULL) FreeDecTree(nd->left);
    if (nd->right != NULL) FreeDecTree(nd->right);
    free(nd);
}

uchar RestoreTreeChar(BitBuffer *BB, FILE *in) {
    uchar ch = 0;
    for (int i = 128; i > 0; i /= 2) {
        if (BitRead(BB, in) == 1) ch += i;
    }
    return ch;
}

DecNode *RebuildTree(BitBuffer *BB, FILE *in) {
    if (BitRead(BB, in) == 1) return NewDecNode(RestoreTreeChar(BB, in));
    else {
        DecNode *nd = NewDecNode(0);
        nd->left = RebuildTree(BB, in);
        nd->right = RebuildTree(BB, in);
        return nd;
    }
}

void RestoreText(DecNode *nd, BitBuffer *BB, FILE *in, FILE *out) {
    while (!IsDecLeaf(nd)) {
        if (BitRead(BB, in) == 1) nd = nd->right;
        else nd = nd->left;
    }
    fwrite(&(nd->ch), 1, 1, out);
}

void Decode(FILE *in, FILE *out) {
    fseek(in, 1, SEEK_SET);
    int textlen;
    if (fread(&textlen, sizeof(int), 1, in) == 0) return;

    BitBuffer BB;
    BB.byte = BB.pos = 0;
    if (fread(&BB.byte, 1, 1, in) == 0) return;

    DecNode *root = RebuildTree(&BB, in);
    if (root == NULL) return;

    for (int i = 0; i < textlen; ++i) {
        RestoreText(root, &BB, in, out);
    }

    FreeDecTree(root);
}
