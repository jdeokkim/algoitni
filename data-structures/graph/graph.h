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

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define GRAPH_ADJACENCY_INIT_CAPACITY 8

/* | 자료형 선언 및 정의... | */

/* 그래프를 나타내는 추상 자료형. */
typedef struct Graph Graph;

/* 그래프의 각 노드의 탐색 시에 호출되는 함수. */
typedef void (*GraphNodeSearchCb)(int v);

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
Graph *graph_create(int c);

/* 그래프에 할당된 메모리를 해제한다. */
void graph_release(Graph *g);

/* 그래프에 정점 `v`와 `w`를 잇는 간선을 추가한다. */
bool graph_add_edge(Graph *g, int v, int w);

/* 그래프를 깊이 우선 방식으로 탐색한다. */
void graph_dfs(Graph *g, int v, GraphNodeSearchCb func);

/* 그래프를 너비 우선 방식으로 탐색한다. */
void graph_bfs(Graph *g, int v, GraphNodeSearchCb func);

/* 그래프의 정점 `v`와 연결된 모든 정점들을 반환한다. */
size_t graph_adjacent_vertices(Graph *g, int v, int *a);

/* 그래프의 정점의 개수를 반환한다. */
size_t graph_vertex_count(Graph *g);

/* 그래프의 정점의 개수를 반환한다. */
size_t graph_edge_count(Graph *g);

#endif // `GRAPH_H`

#ifdef GRAPH_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 그래프를 나타내는 구조체. */
struct Graph {
    struct {
        int *ptr;         // (각 정점에 연결된 다른 정점들의 배열.)
        size_t length;    // (배열에 저장된 항목의 현재 개수.)
        size_t capacity;  // (배열에 들어있는 항목의 개수.)
    } *adjacency;
    int *visited;         // 깊이 우선 탐색에서 각 정점의 방문 여부.
    size_t vertex_count;  // 모든 정점들의 개수.
    size_t edge_count;    // 모든 간선들의 개수.
};

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
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

    g->visited = malloc(c * sizeof(*(g->visited)));

    g->vertex_count = c;
    g->edge_count = 0;

    return g;
}

/* 그래프에 할당된 메모리를 해제한다. */
void graph_release(Graph *g) { 
    if (g == NULL) return;

    for (int i = 0; i < g->vertex_count; i++)
        free(g->adjacency[i].ptr);

    free(g->visited), free(g->adjacency), free(g);
}

/* 그래프에 정점 `v`와 `w`를 잇는 간선을 추가한다. */
bool graph_add_edge(Graph *g, int v, int w) {
    if (g == NULL || v >= g->vertex_count 
        || w >= g->vertex_count) return false;

    if (g->adjacency[v].length >= g->adjacency[v].capacity) {
        const size_t new_capacity = 2 * g->adjacency[v].capacity;

        int *new_ptr = realloc(
            g->adjacency[v].ptr, 
            new_capacity * sizeof(*(g->adjacency[v].ptr))
        );

        if (new_ptr == NULL) return false;

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

        if (new_ptr == NULL) return false;

        g->adjacency[w].capacity = new_capacity;
        g->adjacency[w].ptr = new_ptr;
    }

    g->adjacency[w].ptr[g->adjacency[w].length++] = v;

    g->edge_count++;

    return true;
}

/* (그래프를 깊이 우선 방식으로 탐색한다.) */
static void _graph_dfs_helper(Graph *g, int v, GraphNodeSearchCb func) {
    // 현재 노드를 방문한 것으로 표시한다.
    g->visited[v] = 1;

    if (func != NULL) func(v);

    for (int i = 0; i < g->adjacency[v].length; i++) {
        int w = g->adjacency[v].ptr[i];

        if (g->visited[w]) continue;

        _graph_dfs_helper(g, w, func);
    }
}

/* 그래프를 깊이 우선 방식으로 탐색한다. */
void graph_dfs(Graph *g, int v, GraphNodeSearchCb func) {
    if (g == NULL || v >= g->vertex_count) return;

    for (int i = 0; i < g->vertex_count; i++)
        g->visited[i] = 0;

    _graph_dfs_helper(g, v, func);
}

/* 그래프를 너비 우선 방식으로 탐색한다. */
void graph_bfs(Graph *g, int v, GraphNodeSearchCb func) {
    /* TODO: ... */
}

/* 그래프의 정점 `v`와 연결된 모든 정점들을 반환한다. */
size_t graph_adjacent_vertices(Graph *g, int v, int *a) {
    if (g == NULL || a == NULL) return 0;

    for (int i = 0; i < g->adjacency[v].length; i++)
        a[i] = g->adjacency[v].ptr[i];

    return g->adjacency[v].length;
}

/* 그래프의 정점의 개수를 반환한다. */
size_t graph_vertex_count(Graph *g) {
    return (g != NULL) ? g->vertex_count : 0;
}

/* 그래프의 정점의 개수를 반환한다. */
size_t graph_edge_count(Graph *g) {
    return (g != NULL) ? g->edge_count : 0;
}

#endif // `GRAPH_IMPLEMENTATION`