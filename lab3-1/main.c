#include <stdio.h>
#include <stdlib.h>

void Swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int Partition(int* a, int n) {
    int pivot = a[0];
    int l = -1, r = n;

    while (1) {
        ++l;
        while (a[l] < pivot) ++l;
        --r;
        while (a[r] > pivot) --r;

        if (l >= r) return r;
        else Swap(&a[l], &a[r]);
    }
}

void QuickSort(int* a, int n) {
    if (n > 1) {
        int pivot = Partition(a, n) + 1;
        QuickSort(a, pivot);
        QuickSort(a + pivot, n - pivot);
    }
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 1;
    int *a = malloc(n*sizeof(int));
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &a[i]) != 1) return 1;
    }

    QuickSort(a, n);

    for (int i = 0; i < n; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}
