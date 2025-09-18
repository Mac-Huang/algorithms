#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

/**** Risky Hash Set storing strings.
 * This example uses the following choices:
 *      1. It assumes the values to be stored are strings
 *      2. For hashing, it uses djb2
 *         (https://doc.riot-os.org/group__sys__hashes__djb2.html) with a
 *         random salt.
 *      3. It implements the approach we talked about at the end of lecture
 *         where you resize after too many insertions to keep the number of
 *         collisions low; just like in lecture, we only implement insertion in
 *         this version. Implementing deletion is an extra challenge we suggest
 *         you try yourself! It's particularly hard because of the list of
 *         insertions.
 *      4. To keep track of the list of all insertions, it uses a dynamically
 *         growing array!
 *****/

struct array {
    char **data;
    size_t capacity, count;
};

void array_insert(struct array *array, char *item) {
    if ((array->count + 1) >= array->capacity) {
        array->capacity = 2 * (array->count + 1);
        char **old_data = array->data;
        array->data = calloc(array->capacity, sizeof(char *));
        for (size_t i = 0; i < array->count; i++) array->data[i] = old_data[i];
        free(old_data);
    }
    array->data[array->count++] = item;
}

struct rhs {
    char **buckets;
    int n_buckets;

    struct array insertions;

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
    return rhs->buckets[bucket] && !strcmp(rhs->buckets[bucket], input);
}

size_t square(size_t a) { return a * a; }

void _insert(struct rhs *rhs, char *input, int rehashing) {
    if (!rehashing && search(rhs, input)) return; // already in the array

    if (rhs->n_buckets <= square(rhs->insertions.count+1)) {
        // rehash
        printf("INFO: Rehashing; n_buckets=%d, n_insertions+1=%ld\n",
               rhs->n_buckets, rhs->insertions.count + 1);
        char **old_buckets = rhs->buckets;
        size_t n_old_buckets = rhs->n_buckets;
        rhs->n_buckets = 4 * square(rhs->insertions.count + 1);
        rhs->buckets = calloc(rhs->n_buckets, sizeof(char *));
        rhs->salt = rand();
        for (size_t i = 0; i < n_old_buckets; i++)
            if (old_buckets[i])
                _insert(rhs, old_buckets[i], 1);
        free(old_buckets);
    }

    size_t bucket = hash(rhs, input);
    if (rhs->buckets[bucket])
        printf("INFO: Collision between '%s' and '%s'; both hash to %lu\n",
               input, rhs->buckets[bucket], bucket);

    if (!rehashing) array_insert(&(rhs->insertions), input);
    rhs->buckets[bucket] = strdup(input);
}

void insert(struct rhs *rhs, char *input) { _insert(rhs, input, 0); }

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
