#include <stdlib.h>
#include "huffman_encode.h"
#define BUFFERSIZE 512

typedef unsigned long long ull;

struct HTNode {
    struct HTNode *left, *right;
    int freq;
    uchar ch;
};

struct QNode {
    HTNode *nd;
    struct QNode *next;
};

struct PriorQueue {
    QNode *head;
    int curSize;
};

struct HCode {
    ull num;
    int len;
};

HTNode *NewNode(uchar ch, int freq) {
    HTNode *nd = malloc(sizeof(HTNode));
    nd->left = NULL, nd->right = NULL;
    nd->ch = ch;
    nd->freq = freq;
    return nd;
}

QNode *NewQueueNode(HTNode *nd, QNode *next) {
    QNode *qn = malloc(sizeof(QNode));
    qn->nd = nd;
    qn->next = next;
    return qn;
}

PriorQueue *CreateQueue(void) {
    PriorQueue *queue = malloc(sizeof(PriorQueue));
    queue->curSize = 0;
    queue->head = NULL;
    return queue;
}

void FreeHTree(HTNode *root) {
    if (root == NULL) return;
    FreeHTree(root->left);
    FreeHTree(root->right);
    free(root);
}

void FreeQNodes(QNode *head) {
    if (head == NULL) return;
    FreeQNodes(head->next);
    free(head);
}

void FreeQueue(PriorQueue *q) {
    FreeQNodes(q->head);
    free(q);
}

void FreeHuffman(HTNode *root, HCode *hc_arr) {
    FreeHTree(root);
    free(hc_arr);
}

HCode *InitHCodeArr(void) {
    HCode *hc_arr = malloc(256 * sizeof(HCode));
    for (int i = 0; i < 256; ++i) {
        hc_arr[i].num = 0;
        hc_arr[i].len = 0;
    }
    return hc_arr;
}

void QueueInsert(PriorQueue *q, HTNode *nd) {
    if (q->head != NULL) {
        QNode *cur = q->head;
        while (cur->next != NULL && cur->next->nd->freq < nd->freq) {
            cur = cur->next;
        }
        cur->next = NewQueueNode(nd, cur->next);
    }
    else q->head = NewQueueNode(nd, NULL);
    q->curSize++;
}

void FillPriorityQueue(PriorQueue *q, int CharFreq[256]) {
    for (int i = 0; i < 256; ++i) {
        if (CharFreq[i] > 0) {
            QueueInsert(q, NewNode((uchar)i, CharFreq[i]));
        }
    }
}

void MakeFreqTable(FILE *in, PriorQueue *queue) {
    int CharFreq[256] = {0};
    int cur_len;
    uchar *buffer = malloc(BUFFERSIZE);
    while ((cur_len = (int)fread(buffer, 1, BUFFERSIZE, in)) > 0) {
        for (int i = 0; i < cur_len; ++i) {
            CharFreq[buffer[i]]++;
        }
    }
    free(buffer);
    FillPriorityQueue(queue, CharFreq);
}

HTNode *QueuePop(PriorQueue *q) {
    HTNode *nd = q->head->nd;
    QNode *next = q->head->next;
    free(q->head);
    q->head = next;
    q->curSize--;
    return nd;
}

HTNode *MakeHuffmanTree(PriorQueue *q) {
    while (q->curSize >= 2) {
        HTNode *left = QueuePop(q);
        HTNode *right = QueuePop(q);
        HTNode *parent = NewNode(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        QueueInsert(q, parent);
    }
    HTNode *root = q->head->nd;
    return root;
}

int IsLeaf(HTNode *nd) {
    if (nd == NULL) return 0;
    return (nd->left == NULL && nd->right == NULL);
}

void GetHuffmanCodes(HTNode *nd, HCode *hc_arr, ull num, int len) {
    if (IsLeaf(nd)) {
        hc_arr[nd->ch].num = num;
        hc_arr[nd->ch].len = len;
        return;
    }
    if (nd->left != NULL) GetHuffmanCodes(nd->left, hc_arr, 2*num, len+1);
    if (nd->right != NULL) GetHuffmanCodes(nd->right, hc_arr, 2*num+1, len+1);
}

void BitWrite(BitBuffer *BB, int bit, FILE *out) {
    if (bit == 1) BB->byte |= (1 << (7 - BB->pos));
    BB->pos++;
    if (BB->pos == 8) {
        fwrite(&(BB->byte), 1, 1, out);
        BB->pos = 0;
        BB->byte = 0;
    }
}

void WriteTree(HTNode *nd, BitBuffer *BB, FILE *out) {
    if (IsLeaf(nd)) {
        BitWrite(BB, 1, out);
        for (int i = 0; i < 8; ++i) {
            BitWrite(BB, (nd->ch >> (7 - i)) & 1, out);
        }
    } else {
        BitWrite(BB, 0, out);
        if (nd->left != NULL) WriteTree(nd->left, BB, out);
        if (nd->right != NULL) WriteTree(nd->right, BB, out);
    }
}

void WriteCodes(HCode *hc_arr, BitBuffer *BB, FILE *in, FILE *out) {
    fseek(in, 1, SEEK_SET);
    uchar* buffer = malloc(BUFFERSIZE);
    int read_len;
    while ((read_len = (int)fread(buffer, 1, BUFFERSIZE, in)) > 0) {
        for (int i = 0; i < read_len; ++i) {
            HCode hc = hc_arr[buffer[i]];
            for (int j = 1; j <= hc.len; ++j) {
                BitWrite(BB, (int)((hc.num >> (hc.len - j)) & 1), out);
            }
        }
    }
    free(buffer);
    if (BB->pos != 0) fwrite(&(BB->byte), 1, 1, out);
}

void Encode(FILE *in, FILE *out) {
    fseek(in, 1, SEEK_SET);
    PriorQueue *queue = CreateQueue();
    MakeFreqTable(in, queue);
    if (queue->curSize == 0) {
        FreeQueue(queue);
        return;
    }

    HTNode *root = MakeHuffmanTree(queue);
    FreeQueue(queue);

    HCode *hc_arr = InitHCodeArr();
    GetHuffmanCodes(root, hc_arr, 0, 0);
    fwrite(&(root->freq), sizeof(int), 1, out); // textlen

    BitBuffer BB;
    BB.byte = BB.pos = 0;
    WriteTree(root, &BB, out);
    WriteCodes(hc_arr, &BB, in, out);
    FreeHuffman(root, hc_arr);
}
