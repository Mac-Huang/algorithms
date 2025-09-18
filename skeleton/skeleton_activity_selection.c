#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <string.h>

struct activity {
    float start, end;
    char *name;
};

struct activity_list {
    struct activity **activities;
    int n_activities, capacity;
};

void append_activity(struct activity_list *list, struct activity *new) {
    if (list->capacity <= list->n_activities + 1) {
        list->capacity = 2 * (list->n_activities + 1);
        list->activities = realloc(list->activities,
                                   list->capacity * sizeof(new));
    }
    list->activities[list->n_activities++] = new;
}

void swap(struct activity **A, size_t i, size_t j) {
    struct activity *tmp = A[i]; A[i] = A[j]; A[j] = tmp;
}

//////// QUICKSORT CODE
// NOTE: this is biased!
size_t random_between(size_t lo, size_t hi) { return (random()%(hi-lo))+lo; }
size_t partition(struct activity **A, size_t lo, size_t hi) {
    swap(A, lo, random_between(lo, hi));
    float v = A[lo]->end;
    size_t i = lo, j = hi;
    while (1) {
        while (A[++i]->end < v) if ((i + 1) >= hi) break;
        while (A[--j]->end > v) ;
        if (i >= j) { swap(A, lo, j); return j; }
        swap(A, i, j);
    }
}
void quicksort(struct activity **A, size_t lo, size_t hi) {
    if ((hi - lo) <= 1) return;
    size_t p = partition(A, lo, hi);
    quicksort(A, lo, p);
    quicksort(A, p+1, hi);
}

struct activity_list *best_schedule(struct activity_list *all) {
    assert(!"unimplemented");
}

void print_list(struct activity_list *list) {
    quicksort(list->activities, 0, list->n_activities);
    for (int i = 0; i < list->n_activities; i++) {
        struct activity *a = list->activities[i];
        printf("%s: %.2f -- %.2f\n", a->name, a->start, a->end);
    }
}

int main() {
    struct activity_list *activities = calloc(1, sizeof(*activities));
    while (1) {
        float start, end;
        char name[1024];
        struct activity *activity;
        struct activity_list *schedule;

        printf("Enter [p]rint all activities, [n]ew activity, [s]chedule: ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 'p':
            print_list(activities);
            break;
        case 'n':
            activity = calloc(1, sizeof(*activity));
            printf("Short Name: "); scanf("%s", name);
            printf("Start time: "); scanf("%f", &activity->start);
            printf("End time: "); scanf("%f", &activity->end);
            activity->name = strdup(name);
            append_activity(activities, activity);
            break;
        case 's':
            schedule = best_schedule(activities);
            print_list(schedule);
            free(schedule);
            break;
        }
    }
    
}
