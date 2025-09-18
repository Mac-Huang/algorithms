#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct node {
    int values[4];
    struct node *children[4];
    unsigned n : 2, is_leaf : 1;
};
void print_tree(struct node *root);

int find_idx(struct node *root, int value) {
    int i = 0;
    for (; i < root->n; i++) if (root->values[i] >= value) break;
    return i;
}
void absorb(struct node *root, int c) {
    // absorb root->children[c] into root. root->children[c] should be a
    // 2-node.
    assert(!"implement me");
}
void split(struct node *root) {
    // splits root into a 2-node. root should be a 4-node.
    struct node *left = calloc(1, sizeof(*left)),
                *right = calloc(1, sizeof(*right));
    assert(!"implement me: set up left and right properly");
    *root = (struct node){{root->values[1]}, {left, right}, 1, 0};
}

struct node *insert(struct node *root, int value) {
    if (!root) {
        root = calloc(1, sizeof(*root));
        *root = (struct node){{value}, {}, 1, 1};
        return root;
    }
    int c = find_idx(root, value);
    if (c < root->n && root->values[c] == value) return root;
    if (root->is_leaf) {
        for (int i = 2; i >= c; i--) root->values[i+1] = root->values[i];
        root->values[c] = value;
        root->n++;
    } else {
        int old_n = root->children[c]->n;
        root->children[c] = insert(root->children[c], value);
        int was_split = (old_n == 2 && root->children[c]->n == 1);
        if (was_split) absorb(root, c);
    }
    if (root->n > 2) split(root);
    return root;
}

int main() {
    struct node *root = 0;
    while (1) {
        int value = -1;
        printf("Enter the value you wish to insert, in range [0, 100): ");
        scanf("%d", &value);
        if (value < 0 || value >= 100)  break;
        root = insert(root, value);
        print_tree(root);
    }
    printf("Got an invalid input. Goodbye!\n");
    return 0;
}

void _print_tree(struct node *root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("| ");
    for (int i = 0; i < root->n; i++) printf("%d;", root->values[i]);
    printf("\n");
    for (int i = 0; i < root->n+1; i++)
        _print_tree(root->children[i], depth + 1);
}
void print_tree(struct node *root) { _print_tree(root, 0); }
