#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

struct node {
    int id;
    struct node *parent;
    int weight;
};

struct node *find_root(struct node *node) {
    assert(!"unimplemented");
}
struct node *union_nodes(struct node *node1, struct node *node2) {
    node1 = find_root(node1);
    node2 = find_root(node2);
    assert(!"unimplemented");
}
int same_set(struct node *node1, struct node *node2) {
    assert(!"unimplemented");
}

#define N 1024
struct node ID2NODE[N] = {0};

int main() {
    for (int i = 0; i < N; i++) ID2NODE[i].id = i;
    for (int i = 0; i < N; i++) ID2NODE[i].weight = 1;

    while (1) {
        int value1 = -1, value2 = -1;
        printf("Enter [s]ame-set, [f]ind-root, [u]nion: ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 's':
            printf("Enter the first id to use: ");
            scanf("%d", &value1);
            printf("Enter the second id to use: ");
            scanf("%d", &value2);
            if (value1 < 0 || value2 < 0 || value1 >= N || value2 >= N) {
                printf("Invalid input.\n");
                break;
            }
            printf("same_set(%d, %d) = %d\n", value1, value2,
                   same_set(&ID2NODE[value1], &ID2NODE[value2]));
            break;
        case 'f':
            printf("Enter the id to use: ");
            scanf("%d", &value1);
            if (value1 < 0 || value1 >= N) {
                printf("Invalid input.\n");
                break;
            }
            printf("find_root(%d) = %d\n", value1, find_root(&ID2NODE[value1])->id);
            break;
        case 'u':
            printf("Enter the first id to use: ");
            scanf("%d", &value1);
            printf("Enter the second id to use: ");
            scanf("%d", &value2);
            if (value1 < 0 || value2 < 0 || value1 >= N || value2 >= N) {
                printf("Invalid input.\n");
                break;
            }
            printf("union(%d, %d) = %d\n", value1, value2,
                   union_nodes(&ID2NODE[value1], &ID2NODE[value2])->id);
            break;
        }
    }
    return 0;
}
