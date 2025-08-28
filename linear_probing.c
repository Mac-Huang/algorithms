#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

struct rhs {
    char **buckets;
    int n_buckets, n_items;

    size_t salt;
};

size_t hash(struct rhs *rhs, char *input) {
    size_t hash = rhs->salt;
    for (; *input; input++) hash = (hash * 33) ^ *input;
    return hash % rhs->n_buckets;
}

int search(struct rhs *rhs, char *input) {
    if (!rhs->n_buckets) return 0;
    size_t bucket = hash(rhs, input);
    while (rhs->buckets[bucket] && strcmp(rhs->buckets[bucket], input))
        if (++bucket == rhs->n_buckets) bucket = 0;
    return rhs->buckets[bucket] != 0;
}

void insert(struct rhs *rhs, char *input) {
    static int rehashing = 0;
    if (!rehashing && search(rhs, input)) return;

    if (rhs->n_buckets <= 2*(rhs->n_items+1)) {
        // rehash
        printf("INFO: Rehashing; n_buckets=%d, n_items+1=%ld\n",
               rhs->n_buckets, rhs->n_items + 1);
        rehashing++;
        char **old_buckets = rhs->buckets;
        size_t n_old_buckets = rhs->n_buckets;
        rhs->n_buckets = 4 * (rhs->n_items + 1);
        rhs->buckets = calloc(rhs->n_buckets, sizeof(char *));
        rhs->salt = rand();
        for (size_t i = 0; i < n_old_buckets; i++)
            if (old_buckets[i])
                insert(rhs, old_buckets[i]);
        free(old_buckets);
        rehashing--;
    }

    if (!rehashing) rhs->n_items++;

    size_t bucket = hash(rhs, input);
    while (rhs->buckets[bucket]) if (++bucket == rhs->n_buckets) bucket = 0;
    rhs->buckets[bucket] = strdup(input);
}

int main() {
    struct rhs rhs = {0};
    while (1) {
        char buf[1024] = {0};
        printf("Enter [s]earch, [i]nsert, [v]iew buckets: ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 's':
            printf("Enter the string to search for (no spaces, <1024 chars): ");
            scanf("%s", buf);
            if (search(&rhs, buf))  printf("Found!\n");
            else                    printf("Not found!\n");
            break;
        case 'i':
            printf("Enter the string to insert (no spaces, <1024 chars): ");
            scanf("%s", buf);
            insert(&rhs, buf);
            printf("Done inserting %s!\n", buf);
            break;
        case 'v':
            for (int i = 0; i < rhs.n_buckets; i++) {
                if (!rhs.buckets[i]) printf("Bucket %d: <empty>\n", i);
                else                 printf("Bucket %d: %s\n", i, rhs.buckets[i]);
            }
            break;
        }
    }
    return 0;
}
