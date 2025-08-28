#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct edge {
    int src, dst;
    // pprev_src, next_src link together all the nodes with the same SRC value
    struct edge **pprev_src, *next_src,
    // pprev_dst, next_dst link together all the nodes with the same DST value
                **pprev_dst, *next_dst;
};

struct graph {
    struct edge **out;
    struct edge **in;
    int n_nodes, n_edges;
};

struct graph *new_graph(int n_nodes) {
    struct graph *graph = calloc(1, sizeof(*graph));
    graph->out = calloc(n_nodes, sizeof(graph->out));
    graph->in = calloc(n_nodes, sizeof(graph->in));
    graph->n_nodes = n_nodes;
    return graph;
}

#define link(node, list, which) do { \
        node->next_##which = list; \
        if (list) list->pprev_##which = &(node->next_##which); \
        list = node; \
        node->pprev_##which = &(list); \
    } while (0)
struct edge *new_edge(struct graph *graph, int src, int dst) {
    struct edge *edge = calloc(1, sizeof(*edge));
    edge->src = src;
    edge->dst = dst;
    link(edge, graph->out[src], src);
    link(edge, graph->in[dst], dst);
    graph->n_edges++;
    return edge;
}

#define unlink(node, which) do { \
        *(node->pprev_##which) = node->next_##which; \
        if (node->next_##which) node->next_##which->pprev_##which = node->pprev_##which; \
    } while (0)
void delete_edge(struct graph *graph, struct edge *edge) {
    unlink(edge, src);
    unlink(edge, dst);
    free(edge);
    graph->n_edges--;
}

struct edge *find_edge(struct graph *graph, int src, int dst) {
    // A hash table could be used to make this O(1) expected time.
    for (struct edge *edge = graph->out[src]; edge; edge = edge->next_src)
        if (edge->dst == dst)
            return edge;
    return 0;
}

void print_graph(struct graph *graph) {
    for (int i = 0; i < graph->n_nodes; i++) {
        printf("NODE: %d: ", i);
        for (struct edge *edge = graph->out[i]; edge; edge = edge->next_src)
            printf("%d -> %d, ", edge->src, edge->dst);
        printf("\n");
    }
}

// BFS and DFS return an array of reachable nodes
int *BFS(struct graph *graph, int start, int *n_reachable) {
    int *reachable = calloc(graph->n_nodes, sizeof(int));
    *n_reachable = 0;

    // this is essentially a direct-address table keeping track of all the
    // nodes we've put in either the frontier or in reachable.
    int *seen_set = calloc(graph->n_nodes, sizeof(int));

    int *frontier_ = calloc(graph->n_nodes, sizeof(int)),
        *frontier = frontier_;
    int n_in_frontier = 0;

    seen_set[start] = 1;
    frontier[n_in_frontier++] = start;

    while (n_in_frontier) {
        int node = *(frontier++);
        n_in_frontier--;

        reachable[(*n_reachable)++] = node;

        for (struct edge *edge = graph->out[node]; edge; edge = edge->next_src) {
            if (seen_set[edge->dst]) continue;
            seen_set[edge->dst] = 1;
            frontier[n_in_frontier++] = edge->dst;
        }
    }
    free(seen_set);
    free(frontier_);
    return reachable;
}

int *DFS(struct graph *graph, int start, int *n_reachable) {
    int *reachable = calloc(graph->n_nodes, sizeof(int));
    *n_reachable = 0;
    int *in_reachable = calloc(graph->n_nodes, sizeof(int));
    int *frontier = calloc(graph->n_nodes + graph->n_edges, sizeof(int));
    int n_in_frontier = 0;

    frontier[n_in_frontier++] = start;

    while (n_in_frontier) {
        int node = frontier[--n_in_frontier];
        // this occurs if the node was added to the frontier set multiple
        // times, and we already popped it from the frontier set earlier.
        // essentially, it's needed because this implementation doesn't
        // actually use a set, just a list, so deduplication has to be handled
        // somewhere.
        if (in_reachable[node]) continue;

        reachable[(*n_reachable)++] = node;
        in_reachable[node] = 1;

        for (struct edge *edge = graph->out[node]; edge; edge = edge->next_src) {
            if (in_reachable[edge->dst]) continue;
            frontier[n_in_frontier++] = edge->dst;
        }
    }
    free(in_reachable);
    free(frontier);
    return reachable;
}

int *toposort(struct graph *graph, int *n_order) {
    // Rather than copy the graph and delete nodes from it, we're just going to
    // count the number of incoming edges in the hypothetical
    // copied+edge-deleted graph.
    int *n_in_edges = calloc(graph->n_nodes, sizeof(int));
    for (int i = 0; i < graph->n_nodes; i++)
        for (struct edge *edge = graph->in[i]; edge; edge = edge->next_dst)
            n_in_edges[i]++;

    int *order = calloc(graph->n_nodes, sizeof(int));
    *n_order = 0;
    int *prereqs_met = calloc(graph->n_nodes, sizeof(int));
    int n_prereqs_met = 0;

    for (int i = 0; i < graph->n_nodes; i++)
        if (!n_in_edges[i])
            prereqs_met[n_prereqs_met++] = i;

    while (n_prereqs_met) {
        int node = prereqs_met[--n_prereqs_met];
        order[(*n_order)++] = node;
        for (struct edge *edge = graph->out[node]; edge; edge = edge->next_src)
            if (!(--n_in_edges[edge->dst]))
                prereqs_met[n_prereqs_met++] = edge->dst;
    }
    free(n_in_edges);
    free(prereqs_met);
    return order;
}

int main() {
    int n = 0;
    printf("Enter the number of nodes your graph should have: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("Must be a positive number. Goodbye!\n");
        return 1;
    }

    struct graph *graph = new_graph(n);
    while (1) {
        int src = -1, dst = -1, n_reach, *reach, n_order, *order;
        struct edge *edge;

        printf("Enter [b]fs, [d]fs, [t]oposort, [i]nsert edge, [r]emove edge: ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 'b':
        case 'd':
            printf("Enter the node to explore from: ");
            scanf("%d", &src);
            if (src < 0 || src >= n) {
                printf("Invalid node ID.\n");
                break;
            }
            if (choice == 'b') reach = BFS(graph, src, &n_reach);
            else               reach = DFS(graph, src, &n_reach);
            printf("Can reach: ");
            for (int i = 0; i < n_reach; i++)
                printf("%d, ", reach[i]);
            printf("\n");
            free(reach);
            break;
        case 't':
            order = toposort(graph, &n_order);
            printf("Toposort: ");
            for (int i = 0; i < n_order; i++)
                printf("%d, ", order[i]);
            printf("\n");
            break;
        case 'i':
            printf("Enter the source node: "); scanf("%d", &src);
            printf("Enter the destination node: "); scanf("%d", &dst);
            if (!(0 <= src && src < n && 0 <= dst && dst < n)) {
                printf("Invalid input.\n");
                break;
            }
            new_edge(graph, src, dst);
            break;
        case 'r':
            printf("Enter the source node: "); scanf("%d", &src);
            printf("Enter the destination node: "); scanf("%d", &dst);
            if (!(0 <= src && src < n && 0 <= dst && dst < n)) {
                printf("Invalid input.\n");
                break;
            }
            edge = find_edge(graph, src, dst);
            if (!edge) {
                printf("No such edge.\n");
                break;
            }
            delete_edge(graph, edge);
            break;
        }
        printf("Graph edges afterwards:\n");
        print_graph(graph);
    }
    
}
