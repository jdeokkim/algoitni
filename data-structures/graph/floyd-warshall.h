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

#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 인접 행렬로 표현된 그래프를 나타내는 추상 자료형. */
typedef struct MatGraph MatGraph;

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
MatGraph *mg_create(int V);

/* 그래프에 할당된 메모리를 해제한다. */
void mg_release(MatGraph *mg);

/* 그래프에 새로운 간선을 추가한다. */
void mg_add_edge(MatGraph *mg, int u, int v, int w);

/* 플로이드-워셜 알고리즘을 이용하여, 최단 경로 행렬을 업데이트한다. */
void mg_floyd_warshall(MatGraph *mg);

#endif // `FLOYD_WARSHALL_H`

#ifdef FLOYD_WARSHALL_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 인접 행렬로 표현된 그래프를 나타내는 구조체. */
struct MatGraph {
    int **adjacency; // 인접 행렬.
    int **distance;  // 최단 경로의 가중치.
    int V;           // 정점의 개수.
};

/* | 라이브러리 함수... | */

/* 그래프를 생성한다. */
MatGraph *mg_create(int V) {
    MatGraph *mg = malloc(sizeof(*mg));

    mg->adjacency = malloc((V + 1) * sizeof(*(mg->adjacency)));
    mg->distance = malloc((V + 1) * sizeof(*(mg->distance)));

    for (int i = 0; i <= V; i++) {
        mg->adjacency[i] = calloc(V + 1, sizeof(*(mg->adjacency[i])));
        mg->distance[i] = calloc(V + 1, sizeof(*(mg->distance[i])));
    }

    mg->V = V;

    return mg;
}

/* 그래프에 할당된 메모리를 해제한다. */
void mg_release(MatGraph *mg) {
    if (mg == NULL) return;

    free(mg->distance), free(mg->adjacency), free(mg);
}

/* 그래프에 새로운 간선을 추가한다. */
void mg_add_edge(MatGraph *mg, int u, int v, int w) {
    if (mg == NULL) return;

    mg->adjacency[u][v] = w;
}

/* 플로이드-워셜 알고리즘을 이용하여, 최단 경로 행렬을 업데이트한다. */
void mg_floyd_warshall(MatGraph *mg) {
    if (mg == NULL) return;

    for (int i = 1; i <= mg->V; i++) {
        for (int j = 1; j <= mg->V; j++) {
            if (i == j) mg->distance[i][j] = 0;
            else if (mg->adjacency[i][j]) mg->distance[i][j] = mg->adjacency[i][j];
            else mg->distance[i][j] = INT_MAX;
        }
    }

    for (int k = 1; k <= mg->V; k++)
        for (int i = 1; i <= mg->V; i++)
            for (int j = 1; j <= mg->V; j++) {
                if (mg->distance[i][k] == INT_MAX || mg->distance[k][j] == INT_MAX) 
                    continue;
                
                if (mg->distance[i][j] > mg->distance[i][k] + mg->distance[k][j])
                    mg->distance[i][j] = mg->distance[i][k] + mg->distance[k][j];
            }
}

#endif // `FLOYD_WARSHALL_IMPLEMENTATION`