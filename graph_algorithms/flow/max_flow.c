#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

struct edge {
    int src, dst;
    float capacity, flow;
    // pprev_src, next_src link together all the nodes with the same SRC value
    struct edge **pprev_src, *next_src,
    // pprev_dst, next_dst link together all the nodes with the same DST value
                **pprev_dst, *next_dst,
                **pprev_all, *next_all;

    // only used for the residual graph edges
    struct edge *increase, *reduce;
};

struct graph {
    struct edge **out;
    struct edge **in;
    struct edge *all;
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
struct edge *new_edge(struct graph *graph, int src, int dst, float capacity) {
    struct edge *edge = calloc(1, sizeof(*edge));
    edge->src = src;
    edge->dst = dst;
    edge->capacity = capacity;
    link(edge, graph->out[src], src);
    link(edge, graph->in[dst], dst);
    link(edge, graph->all, all);
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
    unlink(edge, all);
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

void print_graph(struct graph *graph, int with_flow) {
    for (int i = 0; i < graph->n_nodes; i++) {
        printf("NODE: %d: ", i);
        for (struct edge *edge = graph->out[i]; edge; edge = edge->next_src)
            if (with_flow)
                printf("%d --(%f/%f)--> %d, ", edge->src, edge->flow,
                       edge->capacity, edge->dst);
            else
                printf("%d --(%f)--> %d, ", edge->src, edge->capacity, edge->dst);
        printf("\n");
    }
}

struct graph *residual_of(struct graph *graph) {
    struct graph *residual = new_graph(graph->n_nodes);
    for (struct edge *edge = graph->all; edge; edge = edge->next_all) {
        if (edge->flow > 0) {
            // can reduce this flow if edge->dst has too many supplies
            struct edge *redge = new_edge(residual, edge->dst, edge->src, edge->flow);
            redge->reduce = edge;
        }
        if (edge->flow < edge->capacity) {
            // can increase this flow if edge->src has too many supplies
            struct edge *redge = new_edge(residual, edge->src, edge->dst, edge->capacity - edge->flow);
            redge->increase = edge;
        }
    }
    return residual;
}

void edmonds_karp(struct graph *graph, int s, int t) {
    // note lots of memory leaks here
    // Initially, have zero flow
    for (struct edge *edge = graph->all; edge; edge = edge->next_all)
        edge->flow = 0;
    while (1) {
        struct graph *residual = residual_of(graph);

        // Now, do BFS to find the path in the residual graph having the fewest
        // number of edges.
        int *seen_set = calloc(graph->n_nodes, sizeof(int));
        struct edge **pred = calloc(graph->n_nodes, sizeof(struct edge *));
        int *frontier = calloc(1 + graph->n_edges, sizeof(int));
        int frontier_start = 0, frontier_end = 0;
        seen_set[s] = 1;
        frontier[frontier_end++] = s;

        while (frontier_start != frontier_end) {
            int node = frontier[frontier_start++];
            for (struct edge *edge = residual->out[node]; edge; edge = edge->next_src) {
                if (seen_set[edge->dst]) continue;
                seen_set[edge->dst] = 1;
                frontier[frontier_end++] = edge->dst;
                pred[edge->dst] = edge;
                if (edge->dst == t) goto found_t;
            }
        }
        return;
found_t:
        float min_capacity = INFINITY;
        for (int p = t; pred[p]; p = pred[p]->src)
            if (pred[p]->capacity < min_capacity)
                min_capacity = pred[p]->capacity;
        for (int p = t; pred[p]; p = pred[p]->src) {
            if (pred[p]->increase)  pred[p]->increase->flow += min_capacity;
            if (pred[p]->reduce)    pred[p]->reduce->flow -= min_capacity;
        }
    }
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
        float capacity;
        struct edge *edge;

        printf("Enter [e]dmonds-karp, [i]nsert edge, [r]emove edge, bul[k] insert edges: ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 'e':
            printf("Enter the source node: "); scanf("%d", &src);
            printf("Enter the destination node: "); scanf("%d", &dst);
            if (!(0 <= src && src < n && 0 <= dst && dst < n)) {
                printf("Invalid node ID.\n");
                break;
            }
            edmonds_karp(graph, src, dst);
            print_graph(graph, 1);
            break;
        case 'i':
            printf("Enter the source node: "); scanf("%d", &src);
            printf("Enter the destination node: "); scanf("%d", &dst);
            printf("Enter the capacity: "); scanf("%f", &capacity);
            if (!(0 <= src && src < n && 0 <= dst && dst < n)) {
                printf("Invalid input.\n");
                break;
            }
            new_edge(graph, src, dst, capacity);
            printf("Graph capacities afterwards:\n");
            print_graph(graph, 0);
            break;
        case 'k':
            while (1) {
                scanf("%d %d %f", &src, &dst, &capacity);
                if (src == -1) break;
                if (!(0 <= src && src < n && 0 <= dst && dst < n)) {
                    printf("Invalid input.\n");
                    break;
                }
                new_edge(graph, src, dst, capacity);
            }
            printf("Graph capacities afterwards:\n");
            print_graph(graph, 0);
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
            printf("Graph capacities afterwards:\n");
            print_graph(graph, 0);
            break;
        }
    }
}
