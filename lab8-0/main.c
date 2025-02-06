#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N_MAX 5000
#define M_MAX(N) ((N) * ((N)-1) / 2)

typedef struct Edge {
    int len;
    int src, dest;
} Edge;

typedef struct DSU {
    int *parent;
    int *rank;
} DSU;

void Swap(int *u, int *v) {
    int t = *u;
    *u = *v;
    *v = t;
}

int Find(DSU *dsu, int v) {
    if (dsu->parent[v] == v) return v;
    return Find(dsu, dsu->parent[v]);
}

void Union(DSU *dsu, int u, int v) {
    int u_rep = Find(dsu, u);
    int v_rep = Find(dsu, v);
    if (u_rep == v_rep) return;

    if (dsu->rank[u_rep] < dsu->rank[v_rep]) {
        dsu->parent[u_rep] = v_rep;
    } else if (dsu->rank[u_rep] > dsu->rank[v_rep]) {
        dsu->parent[v_rep] = u_rep;
    } else {
        dsu->parent[u_rep] = v_rep;
        dsu->rank[v_rep]++;
    }
}

DSU *CreateDSU(int num_vertices) {
    DSU *dsu = malloc(sizeof(DSU));
    dsu->parent = malloc(num_vertices * sizeof(int));
    dsu->rank = calloc(num_vertices, sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        dsu->parent[i] = i;
    }
    return dsu;
}

void DeleteDSU(DSU *dsu) {
    free(dsu->parent);
    free(dsu->rank);
    free(dsu);
}

int CompareEdges(const void *x, const void *y) {
    const Edge *a = (Edge *)x;
    const Edge *b = (Edge *)y;
    return a->len > b->len;
}

enum { BAD_NUM_OF_VERTICES = 1, BAD_NUM_OF_EDGES = 2, BAD_VERTEX = 3,
        BAD_LENGTH = 4, BAD_NUM_OF_LINES = 5, NO_SPANNING_TREE = 6 };

void ErrorExit(char code) {
    switch(code) {
        case 1: printf("bad number of vertices\n"); break;
        case 2: printf("bad number of edges\n");    break;
        case 3: printf("bad vertex\n");             break;
        case 4: printf("bad length\n");             break;
        case 5: printf("bad number of lines\n");    break;
        case 6: printf("no spanning tree\n");       break;
        default: break;
    }
    exit(0);
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) ErrorExit(BAD_NUM_OF_LINES);
    if (N < 0 || N > N_MAX) ErrorExit(BAD_NUM_OF_VERTICES);
    if (N == 0) ErrorExit(NO_SPANNING_TREE);

    int M;
    if (scanf("%d", &M) != 1) ErrorExit(BAD_NUM_OF_LINES);
    if (M < 0 || M > M_MAX(N)) ErrorExit(BAD_NUM_OF_EDGES);

    Edge *edge_list = calloc(M, sizeof(Edge));
    for (int i = 0; i < M; i++) {
        int u, v;
        long len;
        if (scanf("%d %d %ld", &u, &v, &len) != 3) {
            free(edge_list);
            ErrorExit(BAD_NUM_OF_LINES);
        }

        if (u < 1 || u > N || v < 1 || v > N) {
            free(edge_list);
            ErrorExit(BAD_VERTEX);
        }
        if (len < 0 || len > INT_MAX) {
            free(edge_list);
            ErrorExit(BAD_LENGTH);
        }

        if (u > v) Swap(&u, &v);
        edge_list[i].src = u - 1;
        edge_list[i].dest = v - 1;
        edge_list[i].len = (int)len;
    }

    qsort(edge_list, M, sizeof(Edge), CompareEdges);

    Edge *graph = calloc(N - 1, sizeof(Edge));
    DSU *dsu = CreateDSU(N);

    int num_edge_list = 0;
    int num_vertices = 0;
    while (num_vertices < N && num_edge_list < M) {
        int set_u = Find(dsu, edge_list[num_edge_list].src);
        int set_v = Find(dsu, edge_list[num_edge_list].dest);
        if (set_u != set_v) {
            Union(dsu, set_u, set_v);
            graph[num_vertices] = edge_list[num_edge_list];
            num_vertices++;
        }
        num_edge_list++;
    }

    DeleteDSU(dsu);
    free(edge_list);

    if (num_vertices != N - 1) {
        free(graph);
        ErrorExit(NO_SPANNING_TREE);
    } else {
        for (int i = 0; i < N - 1; i++) {
            printf("%d %d\n", graph[i].src + 1, graph[i].dest + 1);
        }
    }

    free(graph);
    return 0;
}
