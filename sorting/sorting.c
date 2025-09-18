#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/////// SHARED HELPER METHOD
void swap(int *A, size_t i, size_t j) {
    int tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}
// NOTE: this is biased!
size_t random_between(size_t lo, size_t hi) {
    return (random() % (hi - lo)) + lo;
}
void validate(int *A, size_t N) {
    for (size_t i = 0; i+1 < N; i++)
        assert(A[i] <= A[i+1]);
}

/////// SELECTION SORT
void selection_sort(int *A, size_t N) {
    for (size_t i = 0; i < N; i++) {
        size_t m = i, j = i;
        while (++j < N) if (A[m] > A[j]) m = j;
        swap(A, i, m);
    }
}

/////// INSERTION SORT
void insertion_sort(int *A, size_t N) {
    for (size_t i = 0; i < N; i++)
        for (size_t j = i; j >= 1 && A[j] < A[j-1]; j--)
            swap(A, j, j-1);
}

/////// MERGE SORT
static int *MERGE_TMP;
void _merge(int *A, size_t lo, size_t mid, size_t hi) {
    size_t i = lo, j = mid, k = 0;
    while (i < mid && j < hi)
        if (A[i] <= A[j])   MERGE_TMP[k++] = A[i++];
        else                MERGE_TMP[k++] = A[j++];
    while (i < mid) MERGE_TMP[k++] = A[i++];
    while (j < hi)  MERGE_TMP[k++] = A[j++];
    while (k --> 0) A[lo + k] = MERGE_TMP[k];
}
void _merge_sort(int *A, size_t lo, size_t hi) {
    if ((hi - lo) <= 1) return;
    // This cutoff should be tuned per-machine
    if ((hi - lo) <= 16) return insertion_sort(A+lo, hi-lo);
    size_t mid = lo + ((hi - lo) / 2);
    _merge_sort(A, lo, mid);
    _merge_sort(A, mid, hi);
    _merge(A, lo, mid, hi);
}
void merge_sort(int *A, size_t N) {
    MERGE_TMP = calloc(N, sizeof(*A));
    _merge_sort(A, 0, N);
    free(MERGE_TMP);
}

/////// QUICK SORT
size_t partition(int *A, size_t lo, size_t hi) {
    swap(A, lo, random_between(lo, hi));
    size_t v = A[lo], i = lo, j = hi;
    while (1) {
        while (A[++i] < v) if ((i + 1) >= hi) break;
        while (A[--j] > v) ;
        if (i >= j) {
            swap(A, lo, j);
            return j;
        }
        swap(A, i, j);
    }
}
void _quicksort(int *A, size_t lo, size_t hi) {
    if ((hi - lo) <= 1) return;
    // This cutoff should be tuned per-machine
    if ((hi - lo) <= 16) return insertion_sort(A+lo, hi-lo);
    size_t p = partition(A, lo, hi);
    _quicksort(A, lo, p);
    _quicksort(A, p+1, hi);
}
void quick_sort(int *A, size_t N) { _quicksort(A, 0, N); }

/////// HEAP SORT
size_t edge_cases(size_t i, size_t d, size_t N) { return i < N ? i : d; }
size_t   left(size_t i, size_t N) { return edge_cases((2*i) + 1, i, N); }
size_t  right(size_t i, size_t N) { return edge_cases((2*i) + 2, i, N); }
void sink(int *A, size_t N, size_t i) {
    size_t l = left(i, N), r = right(i, N);
    while (A[i] < A[l] || A[i] < A[r]) {
        size_t new_i = A[l] > A[r] ? l : r;
        swap(A, i, new_i);
        i = new_i;
        l = left(i, N);
        r = right(i, N);
    }
}
void heap_sort(int *A, size_t N) {
    // Heapify
    for (size_t i = N; i --> 0;) sink(A, N, i);
    // Repeatedly delete the root
    while (N) {
        swap(A, 0, --N);
        sink(A, N, 0);
    }
}

/////// C STDLIB QSORT
int compar(const void *_a, const void *_b) {
    int a = *(const int *)_a, b = *(const int *)_b;
    if (a < b) return -1;
    if (b < a) return +1;
    return 0;
}
void stdlib_sort(int *A, size_t N) { qsort(A, N, sizeof(*A), compar); }

/////// TEST HARNESS
int *generate_random(size_t N) {
    int *A = calloc(N, sizeof(*A));
    for (size_t i = 0; i < N; i++) A[i] = random();
    return A;
}

int *generate_sorted(size_t N) {
    int *A = calloc(N, sizeof(*A));
    for (size_t i = 0; i < N; i++) A[i] = i;
    return A;
}

int *generate_linear_inversions(size_t N) {
    int *A = calloc(N, sizeof(*A));
    for (size_t i = 0; i < N; i++) A[i] = i;
    for (size_t i = 0; i < N; i++) {
        size_t r = random_between(1, N);
        swap(A, r, r - 1);
    }
    return A;
}

clock_t TIME;
void start_timer() { TIME = clock(); }
float end_timer() { return (float)(clock() - TIME) / (float)CLOCKS_PER_SEC; }

void test_algo(int *(generator)(size_t), void sort(int*, size_t), const char *name, size_t N) {
    size_t n_runs = 4096;
    int *(As[n_runs]);
    for (size_t i = 0; i < n_runs; i++) As[i] = generator(N);

    start_timer();
    for (size_t i = 0; i < n_runs; i++) sort(As[i], N);
    float time = end_timer();
    printf("\t%s: %f\n", name, time);

    for (size_t i = 0; i < n_runs; i++) validate(As[i], N);
    for (size_t i = 0; i < n_runs; i++) free(As[i]);
}

void test_distribution(int *(generator)(size_t), const char *name, size_t N) {
    printf("~~~~~~ Testing with %s Inputs of Size %lu ~~~~~~\n", name, N);
    test_algo(generator, selection_sort, "Selection", N);
    test_algo(generator, insertion_sort, "Insertion", N);
    test_algo(generator, merge_sort, "Merge", N);
    test_algo(generator, quick_sort, "Quick", N);
    test_algo(generator, heap_sort, "Heap", N);
    test_algo(generator, stdlib_sort, "Stdlib", N);
}

int main() {
    size_t N = 2048;
    test_distribution(generate_random, "Random", N);
    test_distribution(generate_linear_inversions, "Linear Inversions", N);
    test_distribution(generate_sorted, "Sorted", N);
    return 0;
}
