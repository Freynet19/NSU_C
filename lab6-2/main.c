#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    struct Node **c; // children
    int *keys;
    int n_keys;
    bool isLeaf;
} Node;

typedef struct BTree {
    Node *root;
    int degree;
    int height;
} BTree;

int MaxKeys(BTree *tree) {
    return 2 * tree->degree - 1;
}

Node *CreateNode(BTree *tree, int isLeaf) {
    Node *nd = malloc(sizeof(Node));
    nd->n_keys = 0;
    nd->keys = malloc(MaxKeys(tree) * sizeof(int));
    nd->isLeaf = isLeaf;
    if (isLeaf) nd->c = NULL;
    else nd->c = calloc(MaxKeys(tree) + 1, sizeof(Node*));
    return nd;
}

void Move(char type, Node *dest, int i_dest, Node *src, int i_src, int qnt) { // k -> move keys; c -> move children
    if (type == 'k') memmove(&(dest->keys[i_dest]), &(src->keys[i_src]), qnt * sizeof(int));
    else             memmove(&(dest->c[i_dest]), &(src->c[i_src]), qnt * sizeof(Node*));
}

void Split(Node *nd, int index, BTree *tree) {
    int deg = tree->degree;
    Node *old = nd->c[index];
    Node *new = CreateNode(tree, old->isLeaf);
    new->n_keys = old->n_keys = deg - 1;

    Move('k', new, 0, old, deg, new->n_keys);
    if (!(old->isLeaf)) Move('c', new, 0, old, deg, new->n_keys + 1);

    Move('k', nd, index + 1, nd, index, nd->n_keys - index);
    if (!(nd->isLeaf))  Move('c', nd, index + 1, nd, index, nd->n_keys - index + 1);

    nd->keys[index] = old->keys[deg - 1];
    nd->n_keys++;
    nd->c[index + 1] = new;
}

int GetIndex(Node *nd, int key) {
    int l = 0;
    int r = nd->n_keys - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (key < nd->keys[mid]) r = mid - 1;
        else l = mid + 1;
    }
    return r + 1;
}

void Insert2Node(int key, BTree *tree, Node *nd) {
    int index = GetIndex(nd, key);

    if (nd->isLeaf) {
        Move('k', nd, index + 1, nd, index, nd->n_keys - index);
        nd->keys[index] = key;
        nd->n_keys++;
    }
    else {
        if (nd->c[index]->n_keys == MaxKeys(tree)) {
            Split(nd, index, tree);
            if (key > nd->keys[index]) index++;
        }
        Insert2Node(key, tree, nd->c[index]);
    }
}

void Insert(int key, BTree *tree) {
    if (tree->root->n_keys == MaxKeys(tree)) {
        Node *nd = CreateNode(tree, 0);
        nd->c[0] = tree->root;
        Split(nd, 0, tree);
        tree->root = nd;
        tree->height++;
    }
    Insert2Node(key, tree, tree->root);
}

void DeleteNode(Node *nd) {
    if (nd == NULL) return;
    free(nd->keys);
    if (nd->c != NULL) {
        for (int i = 0; i <= nd->n_keys; i++) {
            DeleteNode(nd->c[i]);
        }
        free(nd->c);
    }
    free(nd);
}

void DeleteTree(BTree *tree) {
    DeleteNode(tree->root);
    free(tree);
}

int main(void) {
    int D;
    if (scanf("%d", &D) != 1) return 1;
    int N;
    if (scanf("%d", &N) != 1) return 1;

    if (N == 0) {
        printf("0\n");
        return 0;
    }

    BTree *tree = malloc(sizeof(BTree));
    tree->height = 1;
    tree->degree = D;
    tree->root = CreateNode(tree, 1);

    for (int i = 0; i < N; ++i) {
        int k;
        if (scanf("%d", &k) != 1) {
            DeleteTree(tree);
            return 1;
        }
        Insert(k, tree);
    }

    printf("%d\n", tree->height);
    DeleteTree(tree);
    return 0;
}

