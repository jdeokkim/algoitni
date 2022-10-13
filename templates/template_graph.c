/*
    Copyright (c) 2022 Jaedeok Kim (https://github.com/jdeokkim)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>

#define GRAPH_ADJACENCY_INIT_CAPACITY 8

typedef struct GQNode {
    struct GQNode *next;
    int value;
} GQNode;

typedef struct GQueue {
    GQNode *head, *tail;
    size_t length;
} GQueue;

typedef struct Graph {
    struct {
        int *ptr;
        size_t length;
        size_t capacity;
    } *adjacency;
    GQueue *queue;
    int *visited;
    size_t vertex_count;
    size_t edge_count;
} Graph;

typedef void (*GraphNodeSearchCb)(int v);

Graph *graph_create(int c);
void graph_release(Graph *g);
int graph_add_edge(Graph *g, int v, int w);
void graph_dfs(Graph *g, int v, GraphNodeSearchCb func);
void graph_bfs(Graph *g, int v, GraphNodeSearchCb func);
size_t graph_adjacent_vertices(Graph *g, int v, int *a);

int main(void) {
    /* TODO: ... */

    return 0;
}

static GQNode *gq_node_create(int value) {
    GQNode *node = malloc(sizeof(*node));

    node->next = NULL;
    node->value = value;

    return node;
}

static void gq_node_release(GQNode *node) {
    free(node);
}

static GQueue *gq_create(void) {
    return calloc(1, sizeof(GQueue));
}

static void gq_release(GQueue *q) {
    if (q == NULL) return;

    q->length = 0;

    GQNode *node = q->head;

    while (node != NULL) {
        GQNode *next = node->next;

        gq_node_release(node);

        node = next;
    }

    free(q);
}

static int gq_enqueue(GQueue *q, int i) {
    if (q == NULL) return 0;

    if (q->head == NULL) {
        q->head = q->tail = gq_node_create(i);
    } else {
        GQNode *node = gq_node_create(i);

        q->tail->next = node;
        q->tail = node;
    }

    q->length++;

    return 1;
}

static int gq_dequeue(GQueue *q, int *const i) {
    if (q == NULL || q->length <= 0 || i == NULL) return 0;

    GQNode *node = q->head->next;

    *i = q->head->value;

    gq_node_release(q->head);

    if (node == NULL) q->head = q->tail = NULL;
    else q->head = node;

    q->length--;

    return 1;
}

Graph *graph_create(int c) {
    Graph *g = malloc(sizeof(*g));

    g->adjacency = malloc(c * sizeof(*(g->adjacency)));

    for (int i = 0; i < c; i++) {
        g->adjacency[i].length = 0;
        g->adjacency[i].capacity = GRAPH_ADJACENCY_INIT_CAPACITY;

        g->adjacency[i].ptr = malloc(
            g->adjacency[i].capacity * sizeof(*(g->adjacency[i].ptr))
        );
    }

    g->queue = gq_create();
    g->visited = malloc(c * sizeof(*(g->visited)));

    g->vertex_count = c;
    g->edge_count = 0;

    return g;
}

void graph_release(Graph *g) { 
    if (g == NULL) return;

    for (int i = 0; i < g->vertex_count; i++)
        free(g->adjacency[i].ptr);

    gq_release(g->queue);

    free(g->visited), free(g->adjacency), free(g);
}

int graph_add_edge(Graph *g, int v, int w) {
    if (g == NULL || v >= g->vertex_count 
        || w >= g->vertex_count) return 0;

    if (g->adjacency[v].length >= g->adjacency[v].capacity) {
        const size_t new_capacity = 2 * g->adjacency[v].capacity;

        int *new_ptr = realloc(
            g->adjacency[v].ptr, 
            new_capacity * sizeof(*(g->adjacency[v].ptr))
        );

        if (new_ptr == NULL) return 0;

        g->adjacency[v].capacity = new_capacity;
        g->adjacency[v].ptr = new_ptr;
    }

    g->adjacency[v].ptr[g->adjacency[v].length++] = w;

    if (g->adjacency[w].length >= g->adjacency[w].capacity) {
        const size_t new_capacity = 2 * g->adjacency[w].capacity;

        int *new_ptr = realloc(
            g->adjacency[w].ptr, 
            new_capacity * sizeof(*(g->adjacency[w].ptr))
        );

        if (new_ptr == NULL) return 0;

        g->adjacency[w].capacity = new_capacity;
        g->adjacency[w].ptr = new_ptr;
    }

    g->adjacency[w].ptr[g->adjacency[w].length++] = v;

    g->edge_count++;

    return 1;
}

static void _graph_dfs_helper(Graph *g, int v, GraphNodeSearchCb func) {
    g->visited[v] = 1;

    if (func != NULL) func(v);

    for (int i = 0; i < g->adjacency[v].length; i++) {
        int w = g->adjacency[v].ptr[i];

        if (g->visited[w]) continue;

        _graph_dfs_helper(g, w, func);
    }
}

void graph_dfs(Graph *g, int v, GraphNodeSearchCb func) {
    if (g == NULL || v >= g->vertex_count) return;

    for (int i = 0; i < g->vertex_count; i++)
        g->visited[i] = 0;

    _graph_dfs_helper(g, v, func);
}

void graph_bfs(Graph *g, int v, GraphNodeSearchCb func) {
    if (g == NULL || v >= g->vertex_count) return;

    for (int i = 0; i < g->vertex_count; i++)
        g->visited[i] = 0;

    GQueue *q = g->queue;

    gq_enqueue(q, v);

    g->visited[v] = 1;

    int w = v;

    while (q->length > 0) {
        gq_dequeue(q, &w);

        if (func != NULL) func(w);

        for (int i = 0; i < g->adjacency[w].length; i++) {
            int u = g->adjacency[w].ptr[i];

            if (g->visited[u]) continue;

            g->visited[u] = 1;

            gq_enqueue(q, u);
        }
    }
}

size_t graph_adjacent_vertices(Graph *g, int v, int *a) {
    if (g == NULL || a == NULL) return 0;

    for (int i = 0; i < g->adjacency[v].length; i++)
        a[i] = g->adjacency[v].ptr[i];

    return g->adjacency[v].length;
}