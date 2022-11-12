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

#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 유니온-파인드를 나타내는 추상 자료형. */
typedef struct UnionFind UnionFind;

/* | 라이브러리 함수... | */

/* 유니온-파인드를 생성한다. */
UnionFind *uf_create(int n);

/* 유니온-파인드에 할당된 메모리를 해제한다. */
void uf_release(UnionFind *uf);

/* 유니온-파인드의 컴포넌트 개수를 반환한다. */
int uf_count(UnionFind *uf);

/* 유니온-파인드의 주어진 두 사이트가 같은 컴포넌트에 속하는지 확인한다. */
bool uf_connected(UnionFind *uf, int p, int q);

/* 유니온-파인드의 주어진 두 사이트를 연결한다. */
void uf_unite(UnionFind *uf, int p, int q);

/* 유니온-파인드의 주어진 사이트의 부모 링크를 반환한다. */
int uf_find(UnionFind *uf, int p);

#endif // `UNION_FIND_H`

#ifdef UNION_FIND_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 가중 퀵-유니온을 사용하는 유니온-파인드를 나타내는 구조체. */
struct UnionFind {
    int *id;    // 각 사이트의 부모 링크.
    int *size;  // 각 뿌리 노드의 크기.
    int count;  // 컴포넌트의 개수.
};

/* | 라이브러리 함수... | */

/* 유니온-파인드를 생성한다. */
UnionFind *uf_create(int n) {
    UnionFind *uf = malloc(sizeof(*uf));

    uf->id = malloc(n * sizeof(*(uf->id)));
    uf->size = malloc(n * sizeof(*(uf->size)));

    for (int i = 0; i < n; i++) {
        uf->id[i] = i;
        uf->size[i] = 1;
    }

    return uf;
}

/* 유니온-파인드에 할당된 메모리를 해제한다. */
void uf_release(UnionFind *uf) {
    if (uf == NULL) return;

    free(uf->id), free(uf->size), free(uf);
}

/* 유니온-파인드의 컴포넌트 개수를 반환한다. */
int uf_count(UnionFind *uf) {
    return (uf != NULL) ? uf->count : 0;
}

/* 유니온-파인드의 주어진 두 사이트가 같은 컴포넌트에 속하는지 확인한다. */
bool uf_connected(UnionFind *uf, int p, int q) {
    if (uf == NULL) return false;

    return uf_find(uf, p) == uf_find(uf, q);
}

/* 유니온-파인드의 주어진 두 사이트를 연결한다. */
void uf_unite(UnionFind *uf, int p, int q) {
    if (uf == NULL) return;

    int i = uf_find(uf, p);
    int j = uf_find(uf, q);

    if (i == j) return;

    if (uf->size[i] < uf->size[j]) uf->id[i] = j, uf->size[j] += uf->size[i];
    else uf->id[j] = i, uf->size[i] += uf->size[j]; 

    uf->count--;
}

/* 유니온-파인드의 주어진 사이트의 부모 링크를 반환한다. */
int uf_find(UnionFind *uf, int p) {
    if (uf == NULL) return -1;

    while (p != uf->id[p]) p = uf->id[p];

    return p;
}

#endif // `UNION_FIND_IMPLEMENTATION`