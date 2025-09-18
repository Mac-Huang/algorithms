#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

void swap(int *A, size_t i, size_t j) {
    int tmp = A[i]; A[i] = A[j]; A[j] = tmp;
}

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

struct sssp_result {
    int start;
    float *dist;
    int *pred;
};

void print_path(int *pred, int end) {
    if (pred[end] != -1) print_path(pred, pred[end]);
    printf(" %d", end);
}

void print_result(struct graph *graph, struct sssp_result sssp) {
    for (int i = 0; i < graph->n_nodes; i++) {
        printf("% 3d -> % 3d, Length %.2f:", sssp.start, i, sssp.dist[i]);
        if (sssp.dist[i] < INFINITY)
            print_path(sssp.pred, i);
        printf("\n");
    }
}

void relax(float *dist, int *pred, struct edge *edge) {
    if (dist[edge->dst] > dist[edge->src] + edge->weight) {
        dist[edge->dst] = dist[edge->src] + edge->weight;
        pred[edge->dst] = edge->src;
    }
}

struct sssp_result bellman_ford(struct graph *graph, int start) {
    assert(!"unimplemented");
}

size_t edge_cases(size_t i, size_t d, size_t N) { return i < N ? i : d; }
size_t   left(size_t i, size_t N) { return edge_cases((2*i) + 1, i, N); }
size_t  right(size_t i, size_t N) { return edge_cases((2*i) + 2, i, N); }
size_t parent(size_t i, size_t N) { return i == 0 ? 0 : ((i-1) / 2); }
void sink(int *heap, int *node2heap, float *dist, size_t N, size_t i) {
    size_t l = left(i, N), r = right(i, N);
    while (dist[heap[i]] > dist[heap[l]] || dist[heap[i]] > dist[heap[r]]) {
        size_t new_i = dist[heap[l]] < dist[heap[r]] ? l : r;
        node2heap[heap[i]] = new_i;
        node2heap[heap[new_i]] = i;
        swap(heap, i, new_i);
        i = new_i;
        l = left(i, N);
        r = right(i, N);
    }
}
void swim(int *heap, int *node2heap, float *dist, size_t N, size_t i) {
    size_t p = parent(i, N);
    while (dist[heap[p]] > dist[heap[i]]) {
        node2heap[heap[p]] = i;
        node2heap[heap[i]] = p;
        swap(heap, i, p);
        i = p;
        p = parent(i, N);
    }
}
int pop_root(int *heap, int *node2heap, float *dist, size_t N) {
    if (N <= 1) return heap[0];
    int root = heap[0], end = heap[N-1];
    node2heap[end] = 0;
    swap(heap, 0, N-1);
    sink(heap, node2heap, dist, N-1, 0);
    return root;
}

struct sssp_result dijkstra(struct graph *graph, int start) {
    assert(!"unimplemented");
}

struct apsp_result {
    struct sssp_result *results;
};

struct apsp_result floyd_warshall(struct graph *graph) {
    // optimized, bottom-up version of Floyd-Warshall adapted from Wikipedia.
    // When starting the kth outer loop iteration, dist[i][j] is at most the
    // length of the shortest path from i to j going through only the nodes 0,
    // 1, ..., k-1.  (note: as is frequently the case with bottom-up version of
    // DP algorithms, this is not exactly the same as the top-down version in
    // the notes, but it's the same idea + the same asymptotic time
    // complexity.)
    float **dist = calloc(graph->n_nodes, sizeof(dist[0]));
    int **pred = calloc(graph->n_nodes, sizeof(pred[0]));
    for (int i = 0; i < graph->n_nodes; i++) {
        dist[i] = calloc(graph->n_nodes, sizeof(dist[0][0]));
        for (int j = 0; j < graph->n_nodes; j++) dist[i][j] = INFINITY;
        pred[i] = calloc(graph->n_nodes, sizeof(pred[0][0]));
        for (int j = 0; j < graph->n_nodes; j++) pred[i][j] = -1;
    }

    for (int i = 0; i < graph->n_nodes; i++) dist[i][i] = 0;
    for (struct edge *edge = graph->all; edge; edge = edge->next_all) {
        if (edge->weight < dist[edge->src][edge->dst]) {
            dist[edge->src][edge->dst] = edge->weight;
            pred[edge->src][edge->dst] = edge->src;
        }
    }

    for (int k = 0; k < graph->n_nodes; k++)
    for (int i = 0; i < graph->n_nodes; i++)
    for (int j = 0; j < graph->n_nodes; j++) {
        if (dist[i][j] > dist[i][k] + dist[k][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
            pred[i][j] = pred[k][j];
        }
    }

    struct sssp_result *results
        = calloc(graph->n_nodes, sizeof(struct sssp_result));
    for (int i = 0; i < graph->n_nodes; i++) {
        results[i].start = i;
        results[i].dist = dist[i];
        results[i].pred = pred[i];
    }
    free(dist);
    free(pred);
    return (struct apsp_result){results};
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
        struct sssp_result sssp;
        struct apsp_result apsp;
        struct edge *edge;

        printf("Enter [d]ijkstra, [b]ellman-ford, [f]loyd-warshall, [i]nsert edge, [r]emove edge, bul[k] insert edges: ");
        char choice = ' ';
        while (isspace(choice)) scanf("%c", &choice);
        switch (choice) {
        case 'b':
        case 'd':
            printf("Enter the source node: ");
            scanf("%d", &src);
            if (src < 0 || src >= n) {
                printf("Invalid node ID.\n");
                break;
            }
            if (choice == 'b') sssp = bellman_ford(graph, src);
            else               sssp = dijkstra(graph, src);
            print_result(graph, sssp);
            free(sssp.dist);
            free(sssp.pred);
            break;
        case 'f':
            apsp = floyd_warshall(graph);
            for (int i = 0; i < graph->n_nodes; i++)
                print_result(graph, apsp.results[i]);
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
            break;
        case 'k':
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
            break;
        }
        printf("Graph edges afterwards:\n");
        print_graph(graph);
    }
    
}
