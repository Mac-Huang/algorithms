#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

struct edge {
    int src, dst;
    float weight;
    // pprev_src, next_src link together all the nodes with the same SRC value
    struct edge **pprev_src, *next_src,
    // pprev_dst, next_dst link together all the nodes with the same DST value
                **pprev_dst, *next_dst,
                **pprev_all, *next_all;
};

struct graph {
    struct edge **out;
    struct edge **in;
    struct edge *all;
    int n_nodes, n_edges;
};

void swap(struct edge **A, size_t i, size_t j) {
    struct edge *tmp = A[i]; A[i] = A[j]; A[j] = tmp;
}

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
struct edge *new_edge(struct graph *graph, int src, int dst, float weight) {
    struct edge *edge = calloc(1, sizeof(*edge));
    edge->src = src;
    edge->dst = dst;
    edge->weight = weight;
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

void print_graph(struct graph *graph) {
    for (int i = 0; i < graph->n_nodes; i++) {
        printf("NODE: %d: ", i);
        for (struct edge *edge = graph->out[i]; edge; edge = edge->next_src)
            printf("%d -> %d, ", edge->src, edge->dst);
        printf("\n");
    }
}

size_t edge_cases(size_t i, size_t d, size_t N) { return i < N ? i : d; }
size_t   left(size_t i, size_t N) { return edge_cases((2*i) + 1, i, N); }
size_t  right(size_t i, size_t N) { return edge_cases((2*i) + 2, i, N); }
size_t parent(size_t i, size_t N) { return i == 0 ? 0 : ((i-1) / 2); }
void sink(struct edge **heap, size_t N, size_t i) {
    size_t l = left(i, N), r = right(i, N);
    while (heap[i]->weight > heap[l]->weight || heap[i]->weight > heap[r]->weight) {
        size_t new_i = heap[l]->weight < heap[r]->weight ? l : r;
        swap(heap, i, new_i);
        i = new_i;
        l = left(i, N);
        r = right(i, N);
    }
}
void swim(struct edge **heap, size_t N, size_t i) {
    size_t p = parent(i, N);
    while (heap[p]->weight > heap[i]->weight) {
        swap(heap, i, p);
        i = p;
        p = parent(i, N);
    }
}
struct edge *pop_root(struct edge **heap, size_t N) {
    if (N <= 1) return heap[0];
    struct edge *root = heap[0];
    swap(heap, 0, N-1);
    sink(heap, N-1, 0);
    return root;
}

struct edge **prim(struct graph *graph) {
    int start_node = 0;
    int *reached = calloc(graph->n_nodes, sizeof(int));
    reached[start_node] = 1;

    struct edge **outgoing_edges
        = calloc(graph->n_edges, sizeof(struct edge*));
    int n_outgoing_edges = 0;
    for (struct edge *edge = graph->out[start_node]; edge; edge = edge->next_src) {
        outgoing_edges[n_outgoing_edges++] = edge;
        swim(outgoing_edges, n_outgoing_edges, n_outgoing_edges - 1);
    }

    struct edge **MST = calloc(graph->n_nodes, sizeof(struct edge*));
    int n_MST = 0;

    while (n_MST != (graph->n_nodes - 1)) {
        if (n_outgoing_edges == 0) {
            free(MST); MST = 0;
            break;
        }
        struct edge *edge = pop_root(outgoing_edges, n_outgoing_edges--);
        if (reached[edge->dst]) continue;
        reached[edge->dst] = 1;
        outgoing_edges[n_outgoing_edges++] = edge;
        MST[n_MST++] = edge;
        for (struct edge *other = graph->out[edge->dst]; other; other = other->next_src) {
            outgoing_edges[n_outgoing_edges++] = other;
            swim(outgoing_edges, n_outgoing_edges, n_outgoing_edges - 1);
        }
    }
    free(reached);
    free(outgoing_edges);
    return MST;
}

//////// QUICKSORT CODE
// NOTE: this is biased!
size_t random_between(size_t lo, size_t hi) { return (random()%(hi-lo))+lo; }
size_t partition(struct edge **A, size_t lo, size_t hi) {
    swap(A, lo, random_between(lo, hi));
    float v = A[lo]->weight;
    size_t i = lo, j = hi;
    while (1) {
        while (A[++i]->weight < v) if ((i + 1) >= hi) break;
        while (A[--j]->weight > v) ;
        if (i >= j) { swap(A, lo, j); return j; }
        swap(A, i, j);
    }
}
void quicksort(struct edge **A, size_t lo, size_t hi) {
    if ((hi - lo) <= 1) return;
    size_t p = partition(A, lo, hi);
    quicksort(A, lo, p);
    quicksort(A, p+1, hi);
}

//////// UNION FIND CODE
struct node {
    int id;
    struct node *parent;
    int weight;
};

struct node *find_root(struct node *node) {
    if (!(node->parent)) return node;
    return node->parent = find_root(node->parent);
}
struct node *union_nodes(struct node *node1, struct node *node2) {
    node1 = find_root(node1);
    node2 = find_root(node2);
    int new_weight = node1->weight + node2->weight;
    if (node1->weight < node2->weight)  node1->parent = node2;
    else                                node2->parent = node1;
    find_root(node1)->weight = new_weight;
    return find_root(node1);
}
int same_set(struct node *node1, struct node *node2) {
    return find_root(node1) == find_root(node2);
}

struct edge **kruskal(struct graph *graph) {
    struct edge **all = calloc(graph->n_edges, sizeof(struct edge *));
    int n_all = 0;
    for (struct edge *edge = graph->all; edge; edge = edge->next_all)
        all[n_all++] = edge;
    quicksort(all, 0, n_all);

    struct node *uf_nodes = calloc(graph->n_nodes, sizeof(struct node));
    for (int i = 0; i < graph->n_nodes; i++)
        uf_nodes[i] = (struct node){i, 0, 1};

    struct edge **MST = calloc(graph->n_nodes, sizeof(struct edge*));
    int n_MST = 0;

    for (int i = 0; i < n_all; i++) {
        struct edge *edge = all[i];
        if (!same_set(&uf_nodes[edge->src], &uf_nodes[edge->dst])) {
            MST[n_MST++] = edge;
            union_nodes(&uf_nodes[edge->src], &uf_nodes[edge->dst]);
        }
    }
    free(all); free(uf_nodes);
    if (n_MST != graph->n_nodes - 1) { free(MST); MST = 0; }
    return MST;
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
        float weight;
        struct edge *edge, **edges;

        printf("Enter [p]rim, [k]ruskal, [i]nsert edge, [r]emove edge, [b]ulk insert edges: ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 'p':
        case 'k':
            if (choice == 'p') edges = prim(graph);
            else               edges = kruskal(graph);
            if (!edges) printf("Graph was not connected.\n");
            else {
                for (int i = 0; i < graph->n_nodes - 1; i++) {
                    struct edge *edge = edges[i];
                    printf("Edge %d <-> %d: weight %f\n", edge->src, edge->dst,
                           edge->weight);
                }
                free(edges);
            }
            break;
        case 'i':
            printf("Enter the source node: "); scanf("%d", &src);
            printf("Enter the destination node: "); scanf("%d", &dst);
            printf("Enter the weight: "); scanf("%f", &weight);
            if (!(0 <= src && src < n && 0 <= dst && dst < n)) {
                printf("Invalid input.\n");
                break;
            }
            if (find_edge(graph, src, dst)) {
                printf("Edge already exists, please remove it first!\n");
                break;
            }
            new_edge(graph, src, dst, weight);
            new_edge(graph, dst, src, weight);
            break;
        case 'b':
            while (1) {
                scanf("%d %d %f", &src, &dst, &weight);
                if (src == -1) break;
                if (!(0 <= src && src < n && 0 <= dst && dst < n)) {
                    printf("Invalid input.\n");
                    break;
                }
                if (find_edge(graph, src, dst)) {
                    printf("Edge already exists, please remove it first!\n");
                    break;
                }
                new_edge(graph, src, dst, weight);
                new_edge(graph, dst, src, weight);
            }
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

            if (src != dst) delete_edge(graph, find_edge(graph, dst, src));
            break;
        }
        printf("Graph edges afterwards:\n");
        print_graph(graph);
    }
    
}
