/********
 * Note: this is rushed code meant to show broadly what to do; I'm sure it has
 * bugs. If you find bugs feel free to report it, but we'll probably be slower
 * at fixing them than bugs in the LaTeX/non-challenge-problem solutions.
 ********/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct node {
    int values[4];
    struct node *children[4];
    unsigned n : 2, is_leaf : 1;
    int height;
};
void print_tree(struct node *root);

void fix_height(struct node *node) {
    if (node->is_leaf) { node->height = 1; return; }
    int h = 0;
    for (int i = 0; i <= node->n; i++)
        if (node->children[i]->height > h)
            h = node->children[i]->height;
    node->height = h + 1;
}

int find_idx(struct node *root, int value) {
    int i = 0;
    for (; i < root->n; i++) if (root->values[i] >= value) break;
    return i;
}
void absorb(struct node *root, int c) {
    // absorb root->children[c] into root. root->children[c] should be a
    // 2-node.
    struct node *child = root->children[c];
    int i = find_idx(root, root->children[c]->values[0]);
    for (int j = 2; j >= i; j--) root->values[j+1] = root->values[j];
    for (int j = 2; j >= i; j--) root->children[j+1] = root->children[j];
    root->values[i] = child->values[0];
    root->children[i] = child->children[0];
    root->children[i+1] = child->children[1];
    root->n++;
    fix_height(root);
    fix_height(child);
}
void split(struct node *root) {
    // splits root into a 2-node. root should be a 4-node.
    struct node *left = calloc(1, sizeof(*left)),
                *right = calloc(1, sizeof(*right));
    *left = (struct node){
        {root->values[0]}, {root->children[0], root->children[1]},
        1, root->is_leaf};
    *right = (struct node){
        {root->values[2]}, {root->children[2], root->children[3]},
        1, root->is_leaf};
    *root = (struct node){{root->values[1]}, {left, right}, 1, 0};
    fix_height(left);
    fix_height(right);
    fix_height(root);
}

struct node *insert(struct node *root, int value) {
    if (!root) {
        root = calloc(1, sizeof(*root));
        *root = (struct node){{value}, {}, 1, 1};
        fix_height(root);
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
        fix_height(root);
        int was_split = (old_n == 2 && root->children[c]->n == 1);
        if (was_split) absorb(root, c);
    }
    if (root->n > 2) split(root);
    return root;
}

struct node *join(struct node *root1, int x, struct node *root2) {
    if (!root1) return insert(root2, x);
    if (!root2) return insert(root1, x);

    if (root1->height == root2->height) {
        struct node *root = calloc(1, sizeof(*root));
        *root = (struct node){{x}, {root1, root2}, 1, 1};
        fix_height(root);
        return root;
    }
    struct node *which_root = 0;
    if (root1->height == root2->height + 1) {
        // smash it in here
        root1->values[root1->n++] = x;
        root1->children[root1->n] = root2;
        which_root = root1;
        // then split-absorb on the way up
    } else if (root1->height + 1 == root2->height) {
        // smash it in here
        root2->values[root2->n++] = x;
        root2->children[root2->n] = root1;
        which_root = root2;
        // then split-absorb on the way up
    } else if (root1->height > root2->height) {
        int c = find_idx(root1, x);
        int old_n = root1->children[c]->n;
        root1->children[c] = join(root1->children[c], x, root2);
        fix_height(root1);
        int was_split = (old_n == 2 && root1->children[c]->n == 1);
        if (was_split) absorb(root1, c);
        which_root = root1;
    } else if (root2->height > root1->height) {
        int c = find_idx(root2, x);
        int old_n = root2->children[c]->n;
        root2->children[c] = join(root1, x, root2->children[c]);
        fix_height(root2);
        int was_split = (old_n == 2 && root2->children[c]->n == 1);
        if (was_split) absorb(root2, c);
        which_root = root2;
    }
    if (which_root->n > 2) split(which_root);
    return which_root;
}

void tree_split(struct node *root, int value, struct node **outsmall, struct node **outbig) {
    struct node **smaller_trees = calloc(root->height, sizeof(*smaller_trees)),
                **bigger_trees = calloc(root->height, sizeof(*bigger_trees));
    int *smaller_values = calloc(root->height, sizeof(*smaller_values)),
        *bigger_values = calloc(root->height, sizeof(*bigger_values));
    int small_i = 0, big_i = 0;
    while (1) {
        struct node *old_root = root;
        int c = find_idx(root, value);
        if (c < root->n && root->values[c] == value) {
            // split root into two trees at this value
            if (c == 1) {
                smaller_trees[small_i++] = root;
                bigger_trees[big_i++] = root->children[2];
                root->n--;
            } else if (root->n == 2) {
                smaller_trees[small_i++] = root->children[0];
                bigger_trees[big_i++] = root;
                root->n--;
                root->values[0] = root->values[1];
                root->children[0] = root->children[1];
                root->children[1] = root->children[2];
            } else {
                smaller_trees[small_i++] = root->children[0];
                bigger_trees[big_i++] = root->children[1];
            }
            break;
        } else if (c == 1) {
            // have a smaller tree, and maybe a bigger one (if 3-node)
            smaller_trees[small_i] = old_root->children[0];
            smaller_values[small_i++] = old_root->values[0];
            if (old_root->n == 3) {
                bigger_trees[big_i] = old_root->children[2];
                bigger_values[big_i++] = old_root->values[1];
            }
            root = old_root->children[1];
        } else if (c == 2) {
            assert(old_root->n == 2);
            smaller_trees[small_i] = old_root;
            smaller_values[small_i++] = old_root->values[1];
            old_root->n--;
            root = old_root->children[2];
        } else if (c == 0) {
            root = old_root->children[0];
            if (old_root->n == 2) {
                bigger_trees[big_i] = old_root;
                bigger_values[big_i++] = old_root->values[0];
                old_root->n--;
                old_root->values[0] = old_root->values[1];
                old_root->children[0] = old_root->children[1];
                old_root->children[1] = old_root->children[2];
            } else {
                bigger_trees[big_i] = old_root->children[1];
                bigger_values[big_i++] = old_root->values[0];
            }
        }
    }
    printf("After split, we have %d smaller and %d bigger\n",
            small_i, big_i);
    // now link up the small and big trees from bottom-to-top.
    for (int i = small_i-2; i >= 0; i--) {
        smaller_trees[i]
            = join(smaller_trees[i], smaller_values[i], smaller_trees[i+1]);
    }
    for (int i = big_i-2; i >= 0; i--) {
        bigger_trees[i]
            = join(bigger_trees[i], bigger_values[i], bigger_trees[i+1]);
    }
    *outbig = bigger_trees[0];
    *outsmall = smaller_trees[0];
}

void swap(int *A, int i, int j) { int t = A[i]; A[i] = A[j]; A[j] = t; }

int main() {
    srand(24);

    struct node *root1 = 0, *root2 = 0;

    int N1 = 10, N2 = 2;
    int *values1 = calloc(N1, sizeof(*values1)),
        *values2 = calloc(N2, sizeof(*values2));
    for (int i = 0; i < N1; i++) values1[i] = i;
    for (int i = 0; i < N2; i++) values2[i] = N1 + 1 + i;
    for (int i = 0; i < N1; i++) swap(values1, i, i + rand() % (N1 - i));
    for (int i = 0; i < N2; i++) swap(values2, i, i + rand() % (N2 - i));

    for (int i = 0; i < N1; i++) root1 = insert(root1, values1[i]);
    for (int i = 0; i < N2; i++) root2 = insert(root2, values2[i]);

    printf("Tree 1:\n");
    print_tree(root1);

    printf("Tree 2:\n");
    print_tree(root2);

    struct node *joined = join(root1, N1, root2);
    printf("Joined tree at %lu:\n", N1);
    print_tree(joined);

    struct node *left = 0, *right = 0;
    tree_split(joined, N1, &left, &right);
    printf("Split < %d:\n", N1);
    print_tree(left);
    printf("Split > %d:\n", N1);
    print_tree(right);

    return 0;
}

void _print_tree(struct node *root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("| ");
    if (root->n == 0) printf("<NO VALUES! Probably a bug>");
    for (int i = 0; i < root->n; i++) printf("%d;", root->values[i]);
    printf("\n");
    for (int i = 0; i < root->n+1; i++)
        _print_tree(root->children[i], depth + 1);
}
void print_tree(struct node *root) { _print_tree(root, 0); }
