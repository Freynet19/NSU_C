#include <stdio.h>
#include <stdlib.h>

void Swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void Heapify(int* a, int n, int i) {
    int root = i; // parent
    int left = 2*i + 1; // left child
    int right = 2*i + 2; // right child

    if (left < n && a[left] > a[root]) root = left;
    if (right < n && a[right] > a[root]) root = right;

    if (i != root) {
        Swap(&a[i], &a[root]);
        Heapify(a, n, root);
    }
}

void HeapSort(int* a, int n) {
    for (int i = n / 2 - 1; i >= 0; --i) { // creating initial heap
        Heapify(a, n, i);
    }
    for (int i = n - 1; i >= 0; --i) { // pushing max elements to end, reducing heap length
        Swap(&a[0], &a[i]);
        Heapify(a, i, 0);
    }
}

int main(void) {

    int n;
    if (scanf("%d", &n) != 1) return 1;
    int *a = malloc(n*sizeof(int));
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 1;
    }

    HeapSort(a, n);

    for (int i = 0; i < n; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}
