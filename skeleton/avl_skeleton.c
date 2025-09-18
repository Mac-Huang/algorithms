/////// TODO: You need to replace every instance of the token "direction" in
/////// this file with either "left" or "right."

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

struct node {
    int value;
    int height; // NOTE: we're not doing the 2-bit trick
    struct node *direction;
    struct node *direction;
};

int max(int a, int b) { return a < b ? b : a; }

void print_tree(struct node *root);

int height(struct node *node) { return node ? node->height : 0; }
void fix_height(struct node *node) {
    node->height = 1 + max(height(node->direction), height(node->direction));
}

struct node *rotate_direction(struct node *root) {
    struct node *new_root = root->direction;
    root->direction = root->direction->direction;
    new_root->direction = root;
    fix_height(root);
    fix_height(new_root);
    return new_root;
}

struct node *rotate_direction(struct node *root) {
    struct node *new_root = root->direction;
    root->direction = root->direction->direction;
    new_root->direction = root;
    fix_height(root);
    fix_height(new_root);
    return new_root;
}

struct node *avl_insert(struct node *root, int value) {
    if (!root) {
        struct node *new = calloc(1, sizeof(*new));
        new->value = value;
        new->height = 1;
        return new;
    }

    if (value == root->value) return root;

    struct node **pchild
        = (value < root->value) ? &(root->direction) : &(root->direction);
    *pchild = avl_insert(*pchild, value);
    fix_height(root);

    // maybe rebalance @root
    if (height(root->direction) > 1 + height(root->direction)) {         // direction-HEAVY
        // If ZIG-ZAG, need to do one more rotation
        if (height(root->direction->direction) > height(root->direction->direction))
            root->direction = rotate_direction(root->direction);
        root = rotate_direction(root);
    } else if (height(root->direction) > 1 + height(root->direction)) {  // direction-HEAVY
        // If ZIG-ZAG, need to do one more rotation
        if (height(root->direction->direction) > height(root->direction->direction))
            root->direction = rotate_direction(root->direction);
        root = rotate_direction(root);
    }
    return root;
}

int main() {
    struct node *root = 0;
    while (1) {
        int value = -1;
        printf("Enter the value you wish to insert, in range [0, 100): ");
        scanf("%d", &value);
        if (value < 0 || value >= 100)  break;
        root = avl_insert(root, value);
        print_tree(root);
    }
    printf("Got an invalid input. Goodbye!\n");
    return 0;
}

///////////// This code is just for printing the tree!
void flatten(struct node *root, struct node **flat, int i) {
    if (!root) return;
    flat[i] = root;
    flatten(root->direction, flat, (2*i) + 1);
    flatten(root->direction, flat, (2*i) + 2);
}
// We should think of the spacing bottom-up.
// On the very bottom row, we start at 0 and print each leaf spaced by n_digits
//      So the ith bottom node is at position 2*n_digits*i
// On every other row, we place it halfway between its children
int offset(int i, int height, int n_digits) {
    int last_layer = (1 << (height-1)) - 1;
    if (i >= last_layer) return 2 * (n_digits) * (i - last_layer);
    return (offset(2*i+1, height, n_digits) + offset(2*i+2, height, n_digits)) / 2;
}
void print_tree(struct node *root) {
    assert(root);

    size_t max_n = (1 << root->height);
    struct node **flat = calloc(max_n, sizeof(*flat));
    flatten(root, flat, 0);

    int n_digits = 2;

    for (int l = 0; l < root->height; l++) {
        int lo = (1 << l) - 1;
        int hi = (1 << (l+1)) - 1;
        int curr_offset = 0;
        for (int i = lo; i < hi; i++) {
            int target = offset(i, root->height, n_digits);
            while (curr_offset < target) curr_offset++, printf(" ");
            if (!flat[i]) printf("  ");
            else          printf("% 2d", flat[i]->value);
            curr_offset += 2;
        }
        printf("\n");
    }
    free(flat);
}
