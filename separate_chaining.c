#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

struct node {
    char *data;
    struct node *next;
};

struct rhs {
    struct node **buckets;
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
    struct node *node = rhs->buckets[hash(rhs, input)];
    for (; node; node = node->next)
        if (!strcmp(node->data, input)) return 1;
    return 0;
}

void insert(struct rhs *rhs, char *input) {
    static int rehashing = 0;
    if (!rehashing && search(rhs, input)) return; // already in the array

    if (rhs->n_buckets <= 2*(rhs->n_items+1)) {
        // rehash
        printf("INFO: Rehashing; n_buckets=%d, n_items+1=%ld\n",
               rhs->n_buckets, rhs->n_items + 1);
        rehashing++;
        struct node **old_buckets = rhs->buckets;
        size_t n_old_buckets = rhs->n_buckets;
        rhs->n_buckets = 4 * (rhs->n_items + 1);
        rhs->buckets = calloc(rhs->n_buckets, sizeof(char *));
        rhs->salt = rand();
        for (size_t i = 0; i < n_old_buckets; i++) {
            for (struct node *node = old_buckets[i]; node;) {
                struct node *next = node->next;
                insert(rhs, node->data);
                free(node);
                node = next;
            }
        }

        free(old_buckets);
        rehashing--;
    }

    if (!rehashing) rhs->n_items++;

    size_t h = hash(rhs, input);
    struct node *node = calloc(1, sizeof(*node));
    node->data = strdup(input);
    node->next = rhs->buckets[h];
    rhs->buckets[h] = node;
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
                printf("Bucket %d: ", i);
                for (struct node *node = rhs.buckets[i]; node; node = node->next)
                    printf("'%s', ", node->data);
                printf("\n");
            }
            break;
        }
    }
    return 0;
}
