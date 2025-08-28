struct node {
    int value;
    struct node *left, *right;
};

struct node *search_or_insert(struct node **root, int v, int do_insert) {
    if (!(*root) && !do_insert) return 0;
    if (!(*root)) {
        *root = calloc(1, sizeof(**root));
        (*root)->value = v;
        return *root;
    }
    if (v == (*root)->value) return *root;
    if (v < (*root)->value)
        return search_or_insert(&((*root)->left), v, do_insert);
    return search_or_insert(&((*root)->right), v, do_insert);
}

void main() {
    struct node *tree = 0;
    search_or_insert(&tree, 4, 1);
    search_or_insert(&tree, 5, 1);
    search_or_insert(&tree, 3, 1);
    assert(!search_or_insert(&tree, 2, 0));
    assert(search_or_insert(&tree, 5, 0));
    assert(search_or_insert(&tree, 5, 0)->value == 5);
}
