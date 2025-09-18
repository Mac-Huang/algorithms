#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

struct node {
    int value;
    int height; // NOTE: we're not doing the 2-bit trick
    struct node *left;
    struct node *right;
};

int max(int a, int b) { return a < b ? b : a; }

void print_tree(struct node *root);

int height(struct node *node) { return node ? node->height : 0; }
void fix_height(struct node *node) {
    node->height = 1 + max(height(node->left), height(node->right));
}

struct node *rotate_right(struct node *root) {
    struct node *new_root = root->left;
    root->left = root->left->right;
    new_root->right = root;
    fix_height(root);
    fix_height(new_root);
    return new_root;
}

struct node *rotate_left(struct node *root) {
    struct node *new_root = root->right;
    root->right = root->right->left;
    new_root->left = root;
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
        = (value < root->value) ? &(root->left) : &(root->right);
    *pchild = avl_insert(*pchild, value);
    fix_height(root);

    // maybe rebalance @root
    if (height(root->left) > 1 + height(root->right)) {         // LEFT-HEAVY
        // If ZIG-ZAG, need to do one more rotation
        if (height(root->left->right) > height(root->left->left))
            root->left = rotate_left(root->left);
        root = rotate_right(root);
    } else if (height(root->right) > 1 + height(root->left)) {  // RIGHT-HEAVY
        // If ZIG-ZAG, need to do one more rotation
        if (height(root->right->left) > height(root->right->right))
            root->right = rotate_right(root->right);
        root = rotate_left(root);
    }
    return root;
}

int main() {
    struct node *root = 0;
    while (1) {
        int value = -1;
        printf("Enter the value you wish to insert, in range [0, 100): ");
        fflush(stdout);
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
    flatten(root->left, flat, (2*i) + 1);
    flatten(root->right, flat, (2*i) + 2);
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
