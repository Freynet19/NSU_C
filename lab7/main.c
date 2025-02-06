#include <stdio.h>
#include <stdlib.h>

#define N_MAX (2000)
#define M_MAX(N) (N * (N-1) / 2)

typedef unsigned char byte;
typedef struct AdjMat AdjMat;

struct AdjMat {
    int verCount;
    byte data[];
};

AdjMat *InitMatrix(int len) {
    AdjMat *matrix = NULL;
    int bytesSize = len * len / 8 + (len % 8 != 0);
    matrix = calloc(1, sizeof(*matrix) + bytesSize);
    matrix->verCount = len;
    return matrix;
}

int GetEdge(AdjMat *matrix, int u, int v) {
    int edgeIndex = (u * matrix->verCount) + v;
    return (matrix->data[edgeIndex / 8] >> (edgeIndex % 8)) & 1;
}

void AddEdge(AdjMat *matrix, int u, int v) {
    int edgeIndex = (u * matrix->verCount) + v;
    matrix->data[edgeIndex / 8] |= 1 << (edgeIndex % 8);
}

int GTS_DFS(AdjMat *matrix, int *sortedGraph, int *sortedCount, byte *visited, int u) {
    visited[u] = 1;
    for (int i = 0; i < matrix->verCount; ++i) {
        if (GetEdge(matrix, u, i) == 1) {
            if (visited[i] == 1) return 1;
            if (!visited[i]) {
                if (GTS_DFS(matrix, sortedGraph, sortedCount, visited, i) != 0) return 1;
            }
        }
    }
    visited[u] = 2; // all children are visited
    sortedGraph[*sortedCount] = u + 1;
    ++*sortedCount;
    return 0;
}

int GetTopologicalSort(AdjMat *matrix, int *sortedGraph) {
    byte *visited = calloc(matrix->verCount, 1);
    int sortedCount = 0;
    for (int i = 0; i < matrix->verCount; ++i) {
        if (!visited[i]) {
            if (GTS_DFS(matrix, sortedGraph, &sortedCount, visited, i) != 0) {
                free(visited);
                return 1;
            }
        }
    }
    free(visited);
    return 0;
}

enum { BAD_NUM_OF_VERTICES = 1, BAD_NUM_OF_EDGES = 2, BAD_VERTEX = 3, BAD_NUM_OF_LINES = 4, IMPOS_TO_SORT = 5 };

void ErrorExit(char code) {
    switch(code) {
        case 1: printf("bad number of vertices\n"); break;
        case 2: printf("bad number of edges\n");    break;
        case 3: printf("bad vertex\n");             break;
        case 4: printf("bad number of lines\n");    break;
        case 5: printf("impossible to sort\n");     break;
        default: break;
    }
    exit(0);
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) ErrorExit(BAD_NUM_OF_LINES);
    if (N < 0 || N > N_MAX) ErrorExit(BAD_NUM_OF_VERTICES);

    int M;
    if (scanf("%d", &M) != 1) ErrorExit(BAD_NUM_OF_LINES);
    if (M < 0 || M > M_MAX(N)) ErrorExit(BAD_NUM_OF_EDGES);

    AdjMat *matrix = InitMatrix(N);

    for (int i = 1; i <= M; ++i) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            free(matrix);
            ErrorExit(BAD_NUM_OF_LINES);
        }
        if (u < 1 || u > N || v < 1 || v > N) {
            free(matrix);
            ErrorExit(BAD_VERTEX);
        }
        AddEdge(matrix, u-1, v-1);
    }

    int *sortedGraph = malloc(N * sizeof(int));
    if (GetTopologicalSort(matrix, sortedGraph) != 0) {
        free(matrix);
        free(sortedGraph);
        ErrorExit(IMPOS_TO_SORT);
    }

    for (int i = N - 1; i >= 0; --i) {
        printf("%d ", sortedGraph[i]);
    }

    printf("\n");
    free(matrix);
    free(sortedGraph);
    return 0;
}
