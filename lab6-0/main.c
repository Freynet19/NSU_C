#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
    int key, height;
    struct Node *left, *right;
};

int height(Node *nd) {
    if (nd == NULL) return 0;
    return nd->height;
}

int balanceFactor(Node *nd) {
    if (nd == NULL) return 0;
    return height(nd->right) - height(nd->left);
}

int max(int m, int n) {
    return (m > n) ? m : n;
}

void UpdateHeight(Node *nd) {
    nd->height = 1 + max(height(nd->left), height(nd->right));
}

Node *NewNode(int key, Node *node) {
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node *Rotate_R(Node *b) {
    Node *a = b->left;
    Node *c = a->right;

    a->right = b;
    b->left = c;

    UpdateHeight(b);
    UpdateHeight(a);

    return a;
}

Node *Rotate_L(Node *a) {
    Node *b = a->right;
    Node *c = b->left;

    b->left = a;
    a->right = c;

    UpdateHeight(a);
    UpdateHeight(b);

    return b;
}

Node *AVLBalance(Node *nd) {
    int bal = balanceFactor(nd);
    if (bal > 1) {
        if (balanceFactor(nd->right) < 0)
            nd->right = Rotate_R(nd->right);
        return Rotate_L(nd);
    }
    else if (bal < -1) {
        if (balanceFactor(nd->left) > 0)
            nd->left = Rotate_L(nd->left);
        return Rotate_R(nd);
    }
    return nd;
}

Node *AVLInsert(Node *nd, int k, Node *new) {
    if (nd == NULL) return NewNode(k, new);
    if (k < nd->key) nd->left = AVLInsert(nd->left, k, new);
    else nd->right = AVLInsert(nd->right, k, new);

    UpdateHeight(nd);
    nd = AVLBalance(nd);

    return nd;
}

int main(void) {
    int N, k;
    if (scanf("%d", &N) != 1) return 1;
    Node *arrNodes = (Node*)malloc(N * sizeof(Node));
    Node *root = NULL;

    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &k) == 1)
            root = AVLInsert(root, k, &arrNodes[i]);
        else {
            free(arrNodes);
            return 1;
        }
    }

    printf("%d\n", height(root));
    free(arrNodes);
    return 0;
}
