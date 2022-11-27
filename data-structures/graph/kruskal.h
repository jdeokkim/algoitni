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

#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define EDGES_INIT_CAPACITY 32

/* | 자료형 선언 및 정의... | */

/* 가중 그래프의 간선을 나타내는 구조체. */
typedef struct Edge {
    int v, w;    // 간선의 두 정점.
    int weight;  // 간선의 가중치.
} Edge;

/* 간선 리스트로 표현된 그래프를 나타내는 추상 자료형. */
typedef struct EdgeGraph EdgeGraph;

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
EdgeGraph *eg_create(void);

/* 그래프에 할당된 메모리를 해제한다. */
void eg_release(EdgeGraph *eg);

/* 그래프에 새로운 간선을 추가한다. */
void eg_add_edge(EdgeGraph *eg, Edge e);

/* 크루스칼 알고리즘을 이용하여, 최소 신장 트리를 구한다. */
EdgeGraph *eg_kruskal(EdgeGraph *eg, int n);

#endif // `KRUSKAL_H`

#ifdef KRUSKAL_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 유니온-파인드를 나타내는 구조체. */
typedef struct UnionFind {
    int *id;    // 각 사이트의 부모 링크.
    int *size;  // 각 뿌리 노드의 크기.
    int count;  // 컴포넌트의 개수.
} UnionFind;

/* 간선 리스트로 표현된 그래프를 나타내는 구조체. */
struct EdgeGraph {
    Edge *edges;      // 간선 리스트.
    size_t length;    // 간선의 현재 개수.
    size_t capacity;  // 간선의 최대 개수.
};

/* | 라이브러리 함수... | */

/* (유니온-파인드를 생성한다.) */
static UnionFind *_uf_create(int n) {
    UnionFind *uf = malloc(sizeof(*uf));

    uf->id = malloc(n * sizeof(*(uf->id)));
    uf->size = malloc(n * sizeof(*(uf->size)));

    for (int i = 0; i < n; i++) {
        uf->id[i] = i;
        uf->size[i] = 1;
    }

    return uf;
}

/* (유니온-파인드에 할당된 메모리를 해제한다.) */
static void _uf_release(UnionFind *uf) {
    if (uf == NULL) return;

    free(uf->size), free(uf->id), free(uf);
}

/* (유니온-파인드의 컴포넌트 개수를 반환한다.) */
static int _uf_count(UnionFind *uf) {
    return (uf != NULL) ? uf->count : 0;
}

/* (유니온-파인드의 주어진 사이트의 부모 링크를 반환한다.) */
static int _uf_find(UnionFind *uf, int p) {
    if (uf == NULL) return -1;

    while (p != uf->id[p]) p = uf->id[p];

    return p;
}

/* (유니온-파인드의 주어진 두 사이트가 같은 컴포넌트에 속하는지 확인한다.) */
static int _uf_connected(UnionFind *uf, int p, int q) {
    return (uf != NULL) && (_uf_find(uf, p) == _uf_find(uf, q));
}

/* (유니온-파인드의 주어진 두 사이트를 연결한다.) */
static void _uf_unite(UnionFind *uf, int p, int q) {
    if (uf == NULL) return;

    int i = _uf_find(uf, p);
    int j = _uf_find(uf, q);

    if (i == j) return;

    if (uf->size[i] < uf->size[j]) uf->id[i] = j, uf->size[j] += uf->size[i];
    else uf->id[j] = i, uf->size[i] += uf->size[j];
}

/* 그래프를 생성한다. */
EdgeGraph *eg_create(void) {
    EdgeGraph *eg = malloc(sizeof(*eg));

    eg->capacity = EDGES_INIT_CAPACITY;
    eg->length = 0;

    eg->edges = malloc(eg->capacity * sizeof(*(eg->edges)));

    return eg;
}

/* 그래프에 할당된 메모리를 해제한다. */
void eg_release(EdgeGraph *eg) {
    if (eg == NULL) return;

    free(eg->edges), free(eg);
}

/* 그래프에 새로운 간선을 추가한다. */
void eg_add_edge(EdgeGraph *eg, Edge e) {
    if (eg == NULL) return;

    if (eg->length >= eg->capacity) {
        size_t new_capacity = 2 * eg->capacity;

        Edge *new_edges = realloc(
            eg->edges, 
            new_capacity * sizeof(*new_edges)
        );

        if (new_edges == NULL) return;

        eg->capacity = new_capacity;
        eg->edges = new_edges;
    }

    eg->edges[eg->length++] = e;
}

/* (그래프의 두 간선의 가중치를 비교한다.) */
static int _compare_edges(const void *a, const void *b) {
    const Edge e1 = *(const Edge *) a;
    const Edge e2 = *(const Edge *) b;

    return (e1.weight > e2.weight) - (e1.weight < e2.weight);
}

/* 크루스칼 알고리즘을 이용하여, 최소 신장 트리를 구한다. */
EdgeGraph *eg_kruskal(EdgeGraph *eg, int n) {
    EdgeGraph *mst = eg_create();
    
    UnionFind *uf = _uf_create(n);

    // 1단계: 그래프의 간선을 가중치 오름차순으로 정렬한다.
    qsort(eg->edges, eg->length, sizeof(*(eg->edges)), _compare_edges);

    // 2단계: 그래프의 모든 간선을 하나씩 살펴보고, 간선을 하나씩 선택한다.
    for (int i = 0; i < eg->length; i++) {
        int v = eg->edges[i].v;
        int w = eg->edges[i].w;

        // 3단계: 사이클이 존재하는지 확인한다.
        if (_uf_connected(uf, v, w)) continue;

        _uf_unite(uf, v, w);

        // 4단계: 간선을 결과 그래프에 추가한다.
        eg_add_edge(mst, eg->edges[i]);
    }

    _uf_release(uf);

    return mst;
}

#endif // `KRUSKAL_IMPLEMENTATION`