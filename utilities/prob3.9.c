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

/******** ONLINE AVL ********/
struct balance_node {
    struct node **pnode;
    // skip to the next node with the same balance factor
    struct balance_node *next_bal;
    // for the entire list
    struct balance_node *next_all;
    struct balance_node *prev_all;
};
struct online_state {
    struct node *root;
    // pointer to the pointer on which to insert the next AVL node
    struct node **pnext;
    // pointer to the first '1' node and the first '0' node
    struct balance_node *first_one;
    struct balance_node *first_zero;
};
void print_state(struct online_state *state) {
    printf("All nodes on right edge: ");
    for (struct balance_node *n = state->first_zero; n; n = n->next_all)
        printf("%d; ", (*(n->pnode))->value);
    printf("\n");
    printf("Balanced nodes on right edge: ");
    for (struct balance_node *n = state->first_zero; n; n = n->next_bal)
        printf("%d; ", (*(n->pnode))->value);
    printf("\n");
    printf("Unbalanced nodes on right edge: ");
    for (struct balance_node *n = state->first_one; n; n = n->next_bal)
        printf("%d; ", (*(n->pnode))->value);
    printf("\n");
}

// Every Bi lives on one of two linked lists.
void insert_max(struct online_state *state, int value) {
    if (!state->pnext) state->pnext = &(state->root);

    // (0) Add the AVL node
    struct node *new = calloc(1, sizeof(*new));
    new->value = value;

    // (1) Prepend a zero to the balance list
    struct balance_node *node = calloc(1, sizeof(*node));
    node->next_bal = state->first_zero;
    node->next_all = state->first_zero;
    node->pnode = state->pnext;
    *(state->pnext) = new;
    state->pnext = &(new->right);
    if (state->first_zero) state->first_zero->prev_all = node;
    state->first_zero = node;

    // (2a) If no ones, then just increase everything above it.
    if (!state->first_one) {
        state->first_one = node->next_all;
        node->next_bal = 0;
        return;
    }

    // (2) Otherwise identify Bi, the rightmost 1
    struct balance_node *rotation_point = state->first_one;
    printf("Rotating about %d\n", (*(rotation_point->pnode))->value);

    // (3) Rotate that node
    *(rotation_point->pnode) = rotate_left(*(rotation_point->pnode));

    // (4) Drop Bi, and make everything between the new zero and Bi's prev
    // (exclusive) a 1
    rotation_point->prev_all->next_all = rotation_point->next_all;
    if (rotation_point->next_all)
        rotation_point->next_all->prev_all = rotation_point->prev_all;
    rotation_point->prev_all->pnode = rotation_point->pnode;

    // prev stays on the zero list but everything else before it goes on the
    // ones list.
    if (rotation_point->prev_all == state->first_zero) {
        // We rotated the parent of the new root so nothing else really changes
        state->first_one = state->first_one->next_bal;
    } else if (rotation_point->prev_all == state->first_zero->next_all) {
        // Here again, everything else stays zero
        state->first_one = state->first_one->next_bal;
    } else {
        // need to move everything AFTER state->first_zero and BEFORE
        // rotation_point->prev_all onto the ones list.
        rotation_point->prev_all->prev_all->next_bal = rotation_point->next_bal;
        state->first_one = state->first_zero->next_all;
        state->first_zero->next_bal = rotation_point->prev_all;
    }
}

int fix_heights(struct node *node) {
    if (!node) return 0;
    int l = fix_heights(node->left), r = fix_heights(node->right);
    return node->height = (l < r) ? r + 1 : l + 1;
}

int main() {
    struct node *truth_root = 0;
    struct online_state state = {0};
    for (int i = 0; i < 16; i++) {
        printf("~~~~~~~~~~~~~~~~ Inserting %d ~~~~~~~~~~~~~~\n", i);
        print_state(&state);
        insert_max(&state, i);
        truth_root = avl_insert(truth_root, i);
        fix_heights(state.root);
        printf("Online version:\n");
        print_tree(state.root);
        printf("Ground truth:\n");
        print_tree(truth_root);
    }
    return 0;
}

void _print_tree(struct node *root, int depth) {
    for (int i = 0; i < depth; i++) printf("| ");
    if (!root) { printf("<empty>\n"); return; }
    printf("%d (%d)\n", root->value, root->height);
    if (root->left || root->right) {
        _print_tree(root->left, depth + 1);
        _print_tree(root->right, depth + 1);
    }
}

void print_tree(struct node *root) {
    _print_tree(root, 0);
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
void old_print_tree(struct node *root) {
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
