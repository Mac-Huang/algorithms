#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct dynarray {
    int *data;
    int n;
    int s;
};

// returns the amount of work done, measured in number of copies.
size_t push(struct dynarray *dynarray, int value) {
    int work = 0;
    if (dynarray->n == dynarray->s) {
        int *new_data = malloc(2 * (dynarray->n + 1) * sizeof(int));
        for (int i = 0; i < dynarray->n; i++) {
            work++;
            new_data[i] = dynarray->data[i];
        }
        free(dynarray->data);
        dynarray->data = new_data;
        dynarray->s = 2 * (dynarray->n + 1);
    }
    work++;
    dynarray->data[dynarray->n++] = value;
    return work;
}

size_t push_bad(struct dynarray *dynarray, int value) {
    int work = 0;
    if (dynarray->n == dynarray->s) {
        int *new_data = malloc((dynarray->n + 1) * sizeof(int));
        for (int i = 0; i < dynarray->n; i++) {
            work++;
            new_data[i] = dynarray->data[i];
        }
        free(dynarray->data);
        dynarray->data = new_data;
        dynarray->s = (dynarray->n + 1);
    }
    work++;
    dynarray->data[dynarray->n++] = value;
    return work;
}

void main() {
    int N = 1024 * 128;
    size_t *work = calloc(N, sizeof(*work));

    printf("~~~~~~ Evaluating the doubling approach from lecture... ~~~~~~\n");
    struct dynarray dynarray = {0};
    for (int i = 0; i < N; i++) work[i] = push(&dynarray, i);
    size_t slowest = work[0];
    for (int i = 0; i < N; i++) if (work[i] > slowest) slowest = work[i];
    size_t total = 0;
    for (int i = 0; i < N; i++) total += work[i];

    printf("Total work: %lu copies\n", total);
    printf("Worst-case work: %d copies per op\n", slowest);
    printf("Amortized work: %lf copies per op\n", (double)total / (double)N);

    printf("~~~~~~ Evaluating the bad incrementing approach from EC quiz... ~~~~~~\n");
    total = 0;
    dynarray = (struct dynarray){0};
    for (int i = 0; i < N; i++) work[i] = push_bad(&dynarray, i);
    slowest = work[0];
    for (int i = 0; i < N; i++) if (work[i] > slowest) slowest = work[i];
    total = 0;
    for (int i = 0; i < N; i++) total += work[i];

    printf("Total work: %lu copies\n", total);
    printf("Worst-case work: %lu copies per op\n", slowest);
    printf("Amortized work: %lf copies per op\n", (double)total / (double)N);
}
