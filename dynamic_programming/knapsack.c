#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

// To see the recursive/non-memoized/non-DP version take a long time, switch
// this to #if 0.
#if 1
int n_materials = 10;
int weights[] = {15, 63, 202, 10, 5, 15, 63, 202, 10, 5};
float price_per_lb[] = {20, 25.2, 15.125, 6.34, 5, 17.6, 63, 21, 3, 7};
#else
#define n_materials 100
int weights[n_materials] = {1};
float price_per_lb[n_materials] = {1.};
#endif

void swap(int *A, size_t i, size_t j) {
    int tmp = A[i]; A[i] = A[j]; A[j] = tmp;
}
size_t random_between(size_t lo, size_t hi) {
    return (random() % (hi - lo)) + lo;
}
size_t partition(int *A, size_t lo, size_t hi) {
    swap(A, lo, random_between(lo, hi));
    size_t v = price_per_lb[A[lo]], i = lo, j = hi;
    while (1) {
        while (price_per_lb[A[++i]] < v) if ((i + 1) >= hi) break;
        while (price_per_lb[A[--j]] > v) ;
        if (i >= j) {
            swap(A, lo, j);
            return j;
        }
        swap(A, i, j);
    }
}
void quicksort(int *A, size_t lo, size_t hi) {
    if ((hi - lo) <= 1) return;
    size_t p = partition(A, lo, hi);
    quicksort(A, lo, p);
    quicksort(A, p+1, hi);
}

float greedy_fractional_knapsack(int capacity) {
    int *item_order = calloc(n_materials, sizeof(int));
    for (int i = 0; i < n_materials; i++) item_order[i] = i;
    quicksort(item_order, 0, n_materials);
    float total = 0.;
    for (int i = n_materials; i --> 0;) {
        int m = item_order[i];
        int how_much = (capacity > weights[m]) ? weights[m] : capacity;
        total += (price_per_lb[m] * how_much);
        capacity -= how_much;
        if (!capacity) break;
    }
    free(item_order);
    return total;
}

float __01_knapsack_recursive(int capacity, int i) {
    if (i <= 0) return 0.;
    float without = __01_knapsack_recursive(capacity, i-1),
          with = without;
    if (weights[i-1] <= capacity)
        with = (weights[i-1] * price_per_lb[i-1])
            + __01_knapsack_recursive(capacity - weights[i-1], i-1);
    return (with < without) ? without : with;
}

float _01_knapsack_recursive(int capacity) {
    return __01_knapsack_recursive(capacity, n_materials);
}

float _01_knapsack_memo_bottomup(int capacity) {
    float **memo = calloc(capacity+1, sizeof(float*));
    for (int i = 0; i <= capacity; i++)
        memo[i] = calloc(n_materials+1, sizeof(float));

    for (int c = 1; c <= capacity; c++) {
        for (int i = 1; i <= n_materials; i++) {
            float without = memo[c][i-1], with = without;
            if (weights[i-1] <= c)
                with = (weights[i-1] * price_per_lb[i-1]) + memo[c - weights[i-1]][i-1];
            memo[c][i] = (with < without) ? without : with;
        }
    }
    return memo[capacity][n_materials];
}

int main() {
    int capacity = 20;
    printf("greedy_fractional_knapsack(%d) = %f\n", capacity,
           greedy_fractional_knapsack(capacity));
    printf("01_knapsack_memo_bottomup(%d) = %f\n", capacity,
           _01_knapsack_memo_bottomup(capacity));
    printf("01_knapsack_recursive(%d) = %f\n", capacity,
           _01_knapsack_recursive(capacity));
    return 0;
}
