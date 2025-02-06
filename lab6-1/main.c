#include <stdio.h>
#include <stdlib.h>

#define BLACK ('B')
#define RED ('R')

typedef struct Node {
    struct Node *left, *right;
    int key;
    char color;
} Node;

typedef struct ancestors {
    Node *arr[64];
    int len;
} ancs;

char GetColor(Node *nd) {
    if (nd == NULL) return BLACK;
    return nd->color;
}

void Recolor(Node *nd, char color) {
    nd->color = color;
}

void SwapColors(Node *a, Node *b) {
    char c1 = GetColor(a), c2 = GetColor(b);
    Recolor(a, c2);
    Recolor(b, c1);
}

Node *GetUncle(Node *g, Node *p) {
    if (p == g->left) return g->right;
    return g->left;
}

Node *ReassignParents(Node *root, Node *x, Node *p, Node *g) {
    if (p == root) return x;
    else if (g != NULL) {
        if (p == g->left) g->left = x;
        else g->right = x;
    }
    return root;
}

Node *Rotate_R(Node *root, Node *p, Node *g) {
    Node *x = p->left;
    Node *q = x->right;

    root = ReassignParents(root, x, p, g);

    x->right = p;
    p->left = q;

    return root;
}

Node *Rotate_L(Node *root, Node *p, Node *g) {
    Node *x = p->right;
    Node *q = x->left;

    root = ReassignParents(root, x, p, g);

    x->left = p;
    p->right = q;

    return root;
}

Node *NewNode(int key, Node *node) {
    node->color = RED;
    node->key = key;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node *ANC(int N, ancs *AL) {
    if (AL->len < N) return NULL;
    return AL->arr[AL->len - N];
} // represents N'th ancestor of node, 0->node_itself, 1->parent, 2->grandparent, 3->great_gp

Node *RBFix(Node *root, Node *x, ancs *AL) {
    if (x == root) {
        Recolor(x, BLACK);
        return root;
    }

    // x is not root
    Node *p = ANC(1, AL);
    if (GetColor(p) == BLACK) return root;

    // x's parent is RED and is not root
    Node *g = ANC(2, AL);
    Node *u = GetUncle(g, p);

    if (GetColor(u) == RED) {
        Recolor(p, BLACK);
        Recolor(u, BLACK);
        Recolor(g, RED);
        AL->len -= 2;
        return RBFix(root, g, AL);
    }

    else { // uncle is BLACK, g is not NULL
        if (g->left == p && p != NULL) {
            if (p->right == x) {
                root = Rotate_L(root, p, g);
                root = Rotate_R(root, g, ANC(3, AL));
                SwapColors(g, x);
            }
            else { // p->left == x
                root = Rotate_R(root, g, ANC(3, AL));
                SwapColors(g, p);
            }
        }

        else if (p != NULL) { // g->right == p
            if (p->left == x) {
                root = Rotate_R(root, p, g);
                root = Rotate_L(root, g, ANC(3, AL));
                SwapColors(g, x);
            }
            else { // p->right == x
                root = Rotate_L(root, g, ANC(3, AL));
                SwapColors(g, p);
            }
        }
        return root;
    }
}

Node *RBInsert(Node *root, int k, Node *new) {
    if (root == NULL) {
        root = NewNode(k, new);
        Recolor(root, BLACK);
        return root;
    }
    
    struct ancestors AL; // Ancestor List
    AL.len = 0;

    Node *nd = root;
    Node *parent;
    while (nd != NULL) {
        parent = nd;
        AL.arr[AL.len] = nd;
        AL.len++;
        if (k < nd->key) nd = nd->left;
        else nd = nd->right;
    }
    if (k < parent->key) parent->left = NewNode(k, new);
    else parent->right = NewNode(k, new);
    
    return RBFix(root, new, &AL);
}

int BlackHeight(Node *root) {
    if (root == NULL) return 0;
    int h = 1;
    for (Node *nd = root; nd != NULL; nd = nd->left) {
        if (GetColor(nd) == BLACK) ++h;
    }
    return h;
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) return 1;
    Node *arrNodes = malloc(N * sizeof(Node));
    Node *root = NULL;

    for (int i = 0; i < N; ++i) {
        int k;
        if (scanf("%d", &k) != 1) {
            free(arrNodes);
            return 1;
        }
        root = RBInsert(root, k, &arrNodes[i]);
    }

    printf("%d\n", BlackHeight(root));
    free(arrNodes);
    return 0;
}
