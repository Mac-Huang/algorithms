#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

struct node {
    int value;
    // note: possible to implement splay trees top-down, without a parent
    // pointer. that optimization is out-of-scope for this class.
    struct node *parent;
    struct node *left;
    struct node *right;
};

void print_tree(struct node *root);
void print_tree_basic(struct node *root);

// For bottom-up splay, we need to implement rotations that also modify and use
// the parent pointers. So it's a bit more complicated, but the same idea.
struct node *SCRATCH; // dummy node that acts as the parent of the root
struct node **splay_pp(struct node *node) {
    // if x->left = node, returns &(x->left). ditto for right.
    if (!(node->parent)) return &SCRATCH;
    assert(node->parent->left == node || node->parent->right == node);
    return (node->parent->left == node) ? &(node->parent->left)
                                        : &(node->parent->right);
}

#define SET_CHILD(PAR, WHICH, CHI) \
  do { PAR->WHICH = CHI; if (CHI) CHI->parent = PAR; } while(0)

// These are just the normal rotate right/rotate left, except also updating
// parent pointers.
struct node *splay_rotate_r(struct node *a) {
    struct node **ap = splay_pp(a),
          *b = a->left,
          *d = b ? b->right : 0;
    b->parent = a->parent;  *ap = b;
    SET_CHILD(b, right, a);
    SET_CHILD(a, left, d);
    return b;
}

struct node *splay_rotate_l(struct node *a) {
    struct node **ap = splay_pp(a),
          *c = a->right,
          *d = c ? c->left : 0;
    c->parent = a->parent;  *ap = c;
    SET_CHILD(c, left, a);
    SET_CHILD(a, right, d);
    return c;
}

// The splay operation.
struct node *splay_splay(struct node *node) {
  while (node->parent) {
      if (!(node->parent->parent)) { // 'zig'
          // rotate around the parent
          if (node->parent->left == node) splay_rotate_r(node->parent);
          else                            splay_rotate_l(node->parent);
      } else {
          // 0 -> left, 1 -> right
          int side = (node->parent->right == node);
          int parent_side = (node->parent->parent->right == node->parent);
          if (side == 0 && parent_side == 0) { // 'zig-zig'
              assert(!"unimplemented");
          } else if (side == 1 && parent_side == 1) { // 'zig-zig'
              assert(!"unimplemented");
          } else if (side == 0 && parent_side == 1) { // 'zig-zag'
              assert(!"unimplemented");
          } else if (side == 1 && parent_side == 0) { // 'zig-zag'
              assert(!"unimplemented");
          }
      }
  }
  return node;
}

struct node *splay_search(struct node *root, int value) {
    if (!root) return 0;
    if (value == root->value) return splay_splay(root);
    if (value < root->value)
        if (root->left)     return splay_search(root->left, value);
        else                return splay_splay(root);
    else
        if (root->right)    return splay_search(root->right, value);
        else                return splay_splay(root);
}
struct node *bst_insert(struct node *root, int value) {
    if (!root) {
        struct node *new = calloc(1, sizeof(*new));
        new->value = value;
        return new;
    }
    if (value == root->value) return root;
    if (value < root->value)
        SET_CHILD(root, left, bst_insert(root->left, value));
    else
        SET_CHILD(root, right, bst_insert(root->right, value));
    return root;
}
struct node *splay_insert(struct node *root, int value) {
    return splay_search(bst_insert(root, value), value);
}
void splay_split(struct node *root, int value,
                 struct node **smaller, struct node **bigger) {
    struct node *node = splay_search(root, value);
    assert(node);
    node->left->parent = node->right->parent = 0;
    *smaller = node->left;
    *bigger = node->right;
    node->left = node->right = 0;
}
struct node *bst_find_max(struct node *root1) {
    if (!root1) return root1;
    return root1->right ? bst_find_max(root1->right) : root1;
}
struct node *splay_join(struct node *root1, struct node *root2) {
    if (!root1) return root2;
    root1 = splay_splay(bst_find_max(root1));
    SET_CHILD(root1, right, root2);
    return root1;
}
struct node *splay_delete(struct node *root, int value) {
    struct node *left = 0, *right = 0;
    splay_split(root, value, &left, &right);
    return splay_join(left, right);
}

int main() {
    struct node *root = 0;
    while (1) {
        int value = -1;
        printf("Enter [s]earch, [i]nsert, [b]stinsert, [d]elete (split/join not supported interactively): ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 's':
            printf("Enter the value to search for: ");
            scanf("%d", &value);
            root = splay_search(root, value);
            break;
        case 'i':
            printf("Enter the value to insert in range [0, 100): ");
            scanf("%d", &value);
            if (!(0 <= value && value < 100)) {
                printf("Invalid input, ignoring.\n");
                break;
            }
            root = splay_insert(root, value);
            break;
        case 'b':
            printf("Enter the value to bst-insert in range [0, 100): ");
            scanf("%d", &value);
            if (!(0 <= value && value < 100)) {
                printf("Invalid input, ignoring.\n");
                break;
            }
            root = bst_insert(root, value);
            break;
        case 'd':
            printf("Enter the value to delete: ");
            scanf("%d", &value);
            root = splay_delete(root, value);
            break;
        }
        printf("Splay tree afterwards:\n");
        print_tree(root);
    }
    return 0;
}

///////////// This code is just for printing the tree!
int height(struct node *root) {
    if (!root) return 0;
    int l = height(root->left), r = height(root->right);
    return 1 + (l < r ? r : l);
}
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
    if (!root) { printf("<empty tree>\n"); return; }

    int h = height(root);
    size_t max_n = (1 << h);
    struct node **flat = calloc(max_n, sizeof(*flat));
    flatten(root, flat, 0);

    int n_digits = 2;

    for (int l = 0; l < h; l++) {
        int lo = (1 << l) - 1;
        int hi = (1 << (l+1)) - 1;
        int curr_offset = 0;
        for (int i = lo; i < hi; i++) {
            int target = offset(i, h, n_digits);
            while (curr_offset < target) curr_offset++, printf(" ");
            if (!flat[i]) printf("  ");
            else          printf("% 2d", flat[i]->value);
            curr_offset += 2;
        }
        printf("\n");
    }
    free(flat);
}

//// in case you don't like the fancy printing
void _print_tree_basic(struct node *root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("| ");
    printf("%d\n", root->value);
    _print_tree_basic(root->left, depth + 1);
    _print_tree_basic(root->right, depth + 1);
}
void print_tree_basic(struct node *root) { _print_tree_basic(root, 0); }
