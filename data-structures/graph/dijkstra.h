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

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define ADJACENCY_INIT_CAPACITY  16
#define HEAP_INIT_CAPACITY       16

/* | 자료형 선언 및 정의... | */

/* 인접 리스트로 표현된 그래프를 나타내는 추상 자료형. */
typedef struct Graph Graph;

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
Graph *graph_create(int vertex_count);

/* 그래프에 할당된 메모리를 해제한다. */
void graph_release(Graph *g);

/* 그래프에 새로운 간선을 추가한다. */
void graph_add_edge(Graph *g, int v, int w, int weight);

/* 다익스트라 알고리즘을 이용하여, 다른 정점까지의 최단 경로를 계산한다. */
void graph_dijkstra(Graph *g, int v);

#endif // `DIJKSTRA_H`

#ifdef DIJKSTRA_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 끝 정점과 가중치를 나타내는 구조체. */
typedef struct VertexData {
    int other;   // 끝 정점.
    int weight;  // 가중치. 
} VertexData;

/* 힙을 나타내는 추상 자료형. */
typedef struct Heap {
    VertexData *ptr;     // 노드의 배열.
    size_t length;       // 노드의 현재 개수.
    size_t capacity;     // 노드의 최대 개수.
} Heap;

/* 인접 리스트로 표현된 그래프를 나타내는 구조체. */
struct Graph {
    struct {
        VertexData *ptr;
        size_t length;
        size_t capacity;
    } *adjacency;            // 인접 리스트.
    Heap *heap;              // 우선 순위 큐.
    int *distance;           // 최단 경로의 가중치.
    int *processed;          // 최단 경로 계산 여부.
    int vertex_count;        // 정점의 개수.
    int edge_count;          // 간선의 개수.
};

/* | 라이브러리 함수... | */

/* (힙을 생성한다.) */
Heap *_heap_create(void) {
    Heap *h = malloc(sizeof(*h));

    h->length = 0;
    h->capacity = HEAP_INIT_CAPACITY + 1;

    h->ptr = malloc(h->capacity * sizeof(*(h->ptr)));

    return h;
}

/* (힙에 할당된 메모리를 해제한다.) */
void _heap_release(Heap *h) {
    if (h == NULL) return;

    h->length = h->capacity = 0;

    free(h->ptr);
    free(h);
}

/* (힙을 상향식으로 복구한다.) */
static void _heap_swim(Heap *h, int k) {
    if (h == NULL) return;

    // 힙-정렬 조건을 만족할 때까지 자식 노드를 위로 보낸다.
    while (k > 1 && h->ptr[k >> 1].weight < h->ptr[k].weight) {
        VertexData t = h->ptr[k >> 1];

        h->ptr[k >> 1] = h->ptr[k];
        h->ptr[k] = t;

        k >>= 1;
    }
}

/* (힙에 새로운 항목을 추가한다.) */
void _heap_insert(Heap *h, VertexData i) {
    if (h == NULL) return;

    if (h->length >= h->capacity - 1) {
        size_t new_capacity = 2 * h->capacity;

        VertexData *new_ptr = realloc(h->ptr, new_capacity * sizeof(*new_ptr));

        if (new_ptr == NULL) return;

        h->capacity = new_capacity;
        h->ptr = new_ptr;
    }

    // 0번째 인덱스의 항목은 사용하지 않는다.
    h->ptr[++h->length] = i;

    _heap_swim(h, h->length);
}

/* (힙을 하향식으로 복구한다.) */
static void _heap_sink(Heap *h, int k) {
    if (h == NULL) return;

    // 힙의 가장 아래까지 내려간다.
    while ((k << 1) <= h->length) {
        int j = k << 1;

        if (j < h->length && h->ptr[j].weight < h->ptr[j + 1].weight) j++;
        if (h->ptr[k].weight >= h->ptr[j].weight) break;

        VertexData t = h->ptr[k];

        h->ptr[k] = h->ptr[j];
        h->ptr[j] = t;

        k = j;
    }
}


/* (힙이 비어 있는지 확인한다.) */
int _heap_is_empty(Heap *h) {
    return (h == NULL) || (h->length <= 0);
}

/* (힙의 루트 노드를 삭제하고, 그 노드의 데이터를 반환한다.) */
VertexData _heap_delete_root(Heap *h) {
    if (_heap_is_empty(h)) return (VertexData) { -1 };

    VertexData result = h->ptr[1];

    // 힙의 루트 노드를 마지막 노드과 교환한다.
    h->ptr[1] = h->ptr[h->length];
    h->ptr[h->length--] = result;

    _heap_sink(h, 1);

    return result;
}

/* 그래프를 생성한다. */
Graph *graph_create(int vertex_count) {
    Graph *g = malloc(sizeof(*g));

    g->adjacency = malloc(vertex_count * sizeof(*(g->adjacency)));

    for (int i = 0; i < vertex_count; i++) {
        g->adjacency[i].length = 0;
        g->adjacency[i].capacity = ADJACENCY_INIT_CAPACITY;

        g->adjacency[i].ptr = malloc(
            g->adjacency[i].capacity * sizeof(*(g->adjacency[i].ptr))
        );
    }

    g->heap = _heap_create();

    g->distance = malloc(vertex_count * sizeof(*(g->distance)));
    g->processed = calloc(vertex_count, sizeof(*(g->processed)));

    g->vertex_count = vertex_count;
    g->edge_count = 0;

    return g;
}

/* 그래프에 할당된 메모리를 해제한다. */
void graph_release(Graph *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->vertex_count; i++)
        free(g->adjacency[i].ptr);

    _heap_release(g->heap);

    free(g->processed), free(g->distance), free(g->adjacency), free(g);
}

/* 그래프에 새로운 간선을 추가한다. */
void graph_add_edge(Graph *g, int v, int w, int weight) {
    if (g == NULL) return;

    if (g->adjacency[v].length >= g->adjacency[v].capacity) {
        size_t new_capacity = 2 * g->adjacency[v].capacity;

        VertexData *new_ptr = realloc(
            g->adjacency[v].ptr,
            new_capacity * sizeof(*new_ptr)
        );

        if (new_ptr == NULL) return;

        g->adjacency[v].ptr = new_ptr;
        g->adjacency[v].capacity = new_capacity;
    }

    g->adjacency[v].ptr[g->adjacency[v].length].other = w;
    g->adjacency[v].ptr[g->adjacency[v].length].weight = weight;

    g->adjacency[v].length++;
}

/* 다익스트라 알고리즘을 이용하여, 다른 정점까지의 최단 경로를 계산한다. */
void graph_dijkstra(Graph *g, int v) {
    if (g == NULL) return;

    // 1단계: 최단 경로 가중치 배열을 초기화한다.
    for (int i = 0; i < g->vertex_count; i++)
        g->distance[i] = INT_MAX;

    // 2단계: 시작 정점의 가중치를 0으로 변경한다.
    g->distance[v] = 0;

    // 3단계: 우선 순위 큐에 시작 정점을 삽입한다.
    _heap_insert(g->heap, (VertexData) { v, 0 });

    // 4단계: 우선 순위 큐에서 정점을 하나씩 제거하고,
    // 그 정점과 인접한 정점을 찾으면서 최단 경로를 업데이트한다.
    while (!_heap_is_empty(g->heap)) {
        VertexData top = _heap_delete_root(g->heap);

        if (g->processed[top.other]) continue;

        g->processed[top.other] = 1;

        for (int i = 0; i < g->adjacency[top.other].length; i++) {
            int w = g->adjacency[top.other].ptr[i].other;
            int weight = g->adjacency[top.other].ptr[i].weight;

            if (g->distance[top.other] == INT_MAX) continue;

            if (g->distance[w] > g->distance[top.other] + weight) {
                g->distance[w] = g->distance[top.other] + weight;

                _heap_insert(g->heap, (VertexData) { w, g->distance[w] });
            }
        }
    }

    for (int i = 1; i < g->vertex_count; i++)
        printf("%d\n", g->distance[i]);
}

#endif // `DIJKSTRA_IMPLEMENTATION`