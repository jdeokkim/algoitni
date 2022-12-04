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

#ifndef TOPO_SORT_H
#define TOPO_SORT_H

#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define ADJACENCY_INIT_CAPACITY  16

/* | 자료형 선언 및 정의... | */

/* 인접 리스트로 표현된 그래프를 나타내는 추상 자료형. */
typedef struct Graph Graph;

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
Graph *graph_create(int V);

/* 그래프에 할당된 메모리를 해제한다. */
void graph_release(Graph *g);

/* 그래프에 새로운 간선을 추가한다. */
void graph_add_edge(Graph *g, int u, int v, int w);

/* 그래프의 모든 노드를 위상 정렬한다.*/
void graph_topo_sort(Graph *g);

#endif // `TOPO_SORT_H`

#ifdef TOPO_SORT_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 간선을 나타내는 구조체. */
typedef struct Edge {
    int v, w;  // 간선의 끝 정점 및 가중치. 
} Edge;

/* 인접 리스트로 표현된 그래프를 나타내는 구조체. */
struct Graph {
    struct {
        Edge *ptr;
        size_t length;
        size_t capacity;
    } *adjacency;            // 인접 리스트.
    struct {
        int *ptr;
        size_t length;
    } stack;                 // 위상 정렬 스택.
    int *visited;            // 각 정점의 방문 여부.
    int vertex_count;        // 정점의 개수.
    int edge_count;          // 간선의 개수.
};

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
Graph *graph_create(int V) {
    Graph *g = malloc(sizeof(*g));

    g->adjacency = malloc(V * sizeof(*(g->adjacency)));

    for (int i = 0; i < V; i++) {
        g->adjacency[i].length = 0;
        g->adjacency[i].capacity = ADJACENCY_INIT_CAPACITY;

        g->adjacency[i].ptr = malloc(
            g->adjacency[i].capacity * sizeof(*(g->adjacency[i].ptr))
        );
    }

    g->stack.ptr = malloc(V * sizeof(*(g->stack.ptr)));
    g->stack.length = 0;

    g->visited = calloc(V, sizeof(*(g->visited)));
    g->vertex_count = V, g->edge_count = 0;

    return g;
}

/* 그래프에 할당된 메모리를 해제한다. */
void graph_release(Graph *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->vertex_count; i++)
        free(g->adjacency[i].ptr);

    free(g->stack.ptr), free(g->visited), free(g->adjacency), free(g);
}

/* 그래프에 새로운 간선을 추가한다. */
void graph_add_edge(Graph *g, int u, int v, int w) {
    if (g == NULL) return;

    if (g->adjacency[u].length >= g->adjacency[u].capacity) {
        size_t new_capacity = 2 * g->adjacency[u].capacity;

        Edge *new_ptr = realloc(
            g->adjacency[u].ptr,
            new_capacity * sizeof(*new_ptr)
        );

        if (new_ptr == NULL) return;

        g->adjacency[u].ptr = new_ptr;
        g->adjacency[u].capacity = new_capacity;
    }

    g->adjacency[u].ptr[g->adjacency[u].length].v = v;
    g->adjacency[u].ptr[g->adjacency[u].length].w = w;

    g->adjacency[u].length++;

    g->edge_count++;
}

/* (그래프를 깊이 우선 탐색한다.) */
static void _graph_dfs(Graph *g, int u) {
    if (g == NULL) return;

    g->visited[u] = 1;

    for (int i = 0; i < g->adjacency[u].length; i++) {
        int v = g->adjacency[u].ptr[i].v;

        if (!g->visited[v]) _graph_dfs(g, v);
    }

    // 위상 정렬 스택에 현재 노드를 추가한다.
    g->stack.ptr[g->stack.length++] = u;
}

/* 그래프의 모든 노드를 위상 정렬한다. */
void graph_topo_sort(Graph *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->vertex_count; i++)
        g->visited[i] = 0;

    g->stack.length = 0;

    // 위상 정렬의 본질은 결국 "깊이 우선 탐색"이다.
    for (int i = 0; i < g->vertex_count; i++)
        if (!g->visited[i]) _graph_dfs(g, i);

    for (int i = g->stack.length - 1; i > 0; i--)
        printf("%d -> ", g->stack.ptr[i]);

    printf("_\n");
}

#endif // `TOPO_SORT_IMPLEMENTATION`