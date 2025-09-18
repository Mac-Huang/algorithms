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
    struct balance_node *first_one, *first_zero;
};
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
