// This is a version of the AVL code that's space-optimized: instead of storing
// an explicit height field on each node, it uses the lower two bits of the
// right child pointer (this assumes pointers are all at least 4-byte aligned!)
//
// Also, we do an explicit loop rather than recursion to better control stack
// usage/avoid function call overhead.
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

struct node {
    int value;
    // low bits of _right_and_balance give the balance factor; zero them out to
    // get the actual right child pointer
    struct node *left, *_right_and_balance;
};
#define ptr(x) ((struct node *)((uintptr_t)(x) & ~3ull))
#define bal(x) ((uintptr_t)(x) & 3ull)

static struct node *left(struct node *node) { return ptr(node)->left; }
static struct node *right(struct node *node) { return ptr(ptr(node)->_right_and_balance); }
static void set_left(struct node *node, struct node *left) { ptr(node)->left = left; }
static void set_right(struct node *node, struct node *right) {
    node = ptr(node);
    node->_right_and_balance = (void*)((uintptr_t)ptr(right) | bal(node->_right_and_balance));
}
static int balance(struct node *node) {
    uintptr_t val = bal(ptr(node)->_right_and_balance);
    if (!val) return 0;
    if (val == 1ull) return +1;
    return -1;
}
static void set_balance(struct node *node, int bal) {
    node = ptr(node);
    node->_right_and_balance
        = (void*)((uintptr_t)ptr(node->_right_and_balance) | ((uintptr_t)bal & 3ull));
}
void set_ptr(struct node **p, struct node *new) {
    *p = (void*)((uintptr_t)ptr(new) | bal(*p));
}

static struct node *rotate_right(struct node *root) {
    struct node *new_root = left(root);
    set_left(root, right(left(root)));
    set_right(new_root, root);
    return new_root;
}

static struct node *rotate_left(struct node *root) {
    struct node *new_root = right(root);
    set_right(root, left(right(root)));
    set_left(new_root, ptr(root));
    return new_root;
}

// Roughly based on Knuth Vol. 3 Algorithm A. The value steps are:
//
// A1--A5: Traverse down the tree and insert the new node @Q. Along the way,
// keep track of @S, the deepest node we see that is unbalanced (or the root if
// all nodes on path are balanced).
//
// A6: Every node strictly between @S and @Q must have had balance 0, so now
// their balance factors need to be updated properly (basically, all of them
// have grown unbalanced in the direction of @Q).
//
// A7.i: If @S has balance factor 0, it must be the root. Update its balance
// factor like we did the nodes on the path and exit.
//
// A7.ii: If @S was unbalanced in the opposite direction of @Q, then the tree
// just became more balanced. Set @S's balance factor to 0 and exit.
//
// A7.iii: If @S was unbalanced in the same direction as @Q, things have gotten
// really bad. Let @R be the child of @S on the path to @Q. Either:
//      a) @R is unbalanced in the same direction as @S, in which case we do a
//      single rotation at @S.
//      b) @R is unbalanced in the opposite direction as @S, in which case we
//      do a double rotation, first at @R then at @S.
static void *avl_insert(struct node **node, int value) {
    // A1--A5: traverse down the tree and track the path
    #define max_path_len 64
    static struct node **path[max_path_len];
    static int directions[max_path_len];
    static int balances[max_path_len];
    int i = 0;
    path[i] = node;
    while (ptr(*path[i])) {
        struct node **n = path[i];
        if (value == ptr(*n)->value)
            directions[i] = 0;
        else directions[i] = (value < ptr(*n)->value) ? -1 : +1;
        balances[i] = balance(*n);
        if (!directions[i]) return ptr(*n);
        if (directions[i] < 0)  { directions[i] = -1; path[++i] = &(ptr(*n)->left); }
        else                    { directions[i] = +1; path[++i] = &(ptr(*n)->_right_and_balance); }
    }
    set_ptr(path[i], calloc(1, sizeof(struct node)));
    ptr(*path[i])->value = value;
    balances[i] = 0;
    struct node *new_node = ptr(*path[i]);
    if (i != 0 && directions[i - 1] == +1) {
        set_balance(*path[i-1], balances[i-1]);
    }

    // A6: work back up while balance = 0 and update
    int j = i;
    while (j --> 0) {
        if (balances[j] != 0) break;
        set_balance(*path[j], directions[j]);
        balances[j] = directions[j];
    }
    if (j == -1) return new_node; // A7.i: if @S is the root ...

    // Otherwise, @S = *path[j]
    // A7.ii
    if (balances[j] != directions[j]) {
        set_balance(*path[j], 0);
        return new_node;
    }

    // A7.iii(a): if @R is unbalanced in the same direction as @S ...
    struct node *A = ptr(*path[j]);
    if (balances[j] == balances[j+1]) {
        // single rotation at balances[j]
        if (balances[j] < 0) set_ptr(path[j], rotate_right(*path[j]));
        else                 set_ptr(path[j], rotate_left(*path[j]));
        set_balance(*path[j], 0);
        set_balance(A, 0);
    } else {
        struct node *B = ptr(*path[j+1]),
                    *X = ptr(*path[j+2]);
        int old_balance_A = balance(A),
            old_balance_B = balance(B),
            old_balance_X = balance(X);
        // double rotation at balances[j+1] then at balances[j+1]
        if (balances[j+1] < 0)  set_ptr(path[j+1], rotate_right(*path[j+1]));
        else                    set_ptr(path[j+1], rotate_left(*path[j+1]));
        if (balances[j] < 0)    set_ptr(path[j], rotate_right(*path[j]));
        else                    set_ptr(path[j], rotate_left(*path[j]));
        set_balance(X, 0);
        // The balances for A and B are more complicated.
        if (old_balance_X == 0) { // X is the newly inserted node
            set_balance(A, 0);
            set_balance(B, 0);
        } else if (old_balance_X == old_balance_A) {
            set_balance(B, 0);
            set_balance(A, -old_balance_A);
        } else { // old_balance_X = -old_balance_A
            set_balance(B, old_balance_A);
            set_balance(A, 0);
        }
    }
    return new_node;
}

void print_tree(struct node *root);
int main() {
    struct node *root = 0;
    while (1) {
        int value = -1;
        printf("Enter the value you wish to insert, in range [0, 100): ");
        scanf("%d", &value);
        if (value < 0 || value >= 100)  break;
        avl_insert(&root, value);
        print_tree(root);
    }
    printf("Got an invalid input. Goodbye!\n");
    return 0;
}

///////////// This code is just for printing the tree!
size_t compute_height(struct node *node) {
    if (!node) return 0;
    size_t lh = compute_height(left(node)),
           rh = compute_height(right(node));
    return (lh < rh) ? (rh + 1) : (lh + 1);
}
void flatten(struct node *root, struct node **flat, int i) {
    if (!root) return;
    flat[i] = root;
    flatten(left(root), flat, (2*i) + 1);
    flatten(right(root), flat, (2*i) + 2);
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

    size_t root_height = compute_height(root);
    size_t max_n = (1 << root_height);
    struct node **flat = calloc(max_n, sizeof(*flat));
    flatten(root, flat, 0);

    int n_digits = 2;

    for (int l = 0; l < root_height; l++) {
        int lo = (1 << l) - 1;
        int hi = (1 << (l+1)) - 1;
        int curr_offset = 0;
        for (int i = lo; i < hi; i++) {
            int target = offset(i, root_height, n_digits);
            while (curr_offset < target) curr_offset++, printf(" ");
            if (!flat[i]) printf("  ");
            else          printf("% 2d", flat[i]->value);
            curr_offset += 2;
        }
        printf("\n");
    }
    free(flat);
}
