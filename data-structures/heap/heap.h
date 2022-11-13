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

#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define HEAP_INIT_CAPACITY 4

/* | 자료형 선언 및 정의... | */

/* 힙에 저장되는 항목의 자료형. */
typedef int Item;

/* 힙을 나타내는 추상 자료형. */
typedef struct Heap Heap;

/* 힙에 사용되는 비교 함수. */
typedef int (*HeapCompareCb)(Item i1, Item i2);

/* | 라이브러리 함수... | */

/* 힙을 생성한다. */
Heap *heap_create(HeapCompareCb func);

/* 힙에 할당된 메모리를 해제한다. */
void heap_release(Heap *h);

/* 힙에 새로운 항목을 추가한다. */
bool heap_insert(Heap *h, Item i);

/* 힙의 루트 노드를 삭제하고, 그 노드의 데이터를 반환한다. */
bool heap_delete_root(Heap *h, Item *const i);

/* 힙이 비어 있는지 확인한다. */
bool heap_is_empty(Heap *h);

/* 힙에 저장된 항목의 개수를 반환한다. */
size_t heap_size(Heap *h);

#endif // `HEAP_H`

#ifdef HEAP_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 힙을 나타내는 구조체. */
struct Heap {
    Item *ptr;           // 노드의 배열.
    size_t length;       // 노드의 현재 개수.
    size_t capacity;     // 노드의 최대 개수.
    HeapCompareCb func;  // 비교 함수.
};

/* | 라이브러리 함수... | */

/* 힙을 생성한다. */
Heap *heap_create(HeapCompareCb func) {
    Heap *h = malloc(sizeof(*h));

    h->func = func;

    h->length = 0;
    h->capacity = HEAP_INIT_CAPACITY + 1;

    h->ptr = malloc(h->capacity * sizeof(*(h->ptr)));

    return h;
}

/* 힙에 할당된 메모리를 해제한다. */
void heap_release(Heap *h) {
    if (h == NULL) return;

    h->length = h->capacity = 0;

    h->func = NULL;

    free(h->ptr);
    free(h);
}

/* 힙을 상향식으로 복구한다. */
static void _heap_swim(Heap *h, int k) {
    if (h == NULL || h->func == NULL) return;

    // 힙-정렬 조건을 만족할 때까지 자식 노드를 위로 보낸다.
    while (k > 1 && h->func(h->ptr[k >> 1], h->ptr[k]) < 0) {
        const Item t = h->ptr[k >> 1];

        h->ptr[k >> 1] = h->ptr[k];
        h->ptr[k] = t;

        k >>= 1;
    }
}

/* 힙에 새로운 항목을 추가한다. */
bool heap_insert(Heap *h, Item i) {
    if (h == NULL) return false;

    if (h->length >= h->capacity - 1) {
        size_t new_capacity = 2 * h->capacity;

        Item *new_ptr = realloc(h->ptr, new_capacity * sizeof(*new_ptr));

        if (new_ptr == NULL) return false;

        h->capacity = new_capacity;
        h->ptr = new_ptr;
    }

    // 0번째 인덱스의 항목은 사용하지 않는다.
    h->ptr[++h->length] = i;

    _heap_swim(h, h->length);

    return true;
}

/* 힙을 하향식으로 복구한다. */
static void _heap_sink(Heap *h, int k) {
    if (h == NULL || h->func == NULL) return;

    // 힙의 가장 아래까지 내려간다.
    while ((k << 1) <= h->length) {
        int j = k << 1;

        if (j < h->length && h->func(h->ptr[j], h->ptr[j + 1]) < 0) j++;
        if (h->func(h->ptr[k], h->ptr[j]) >= 0) break;

        const Item t = h->ptr[k];

        h->ptr[k] = h->ptr[j];
        h->ptr[j] = t;

        k = j;
    }
}

/* 힙의 루트 노드를 삭제하고, 그 노드의 데이터를 반환한다. */
bool heap_delete_root(Heap *h, Item *const i) {
    if (heap_is_empty(h) || i == NULL) return false;

    *i = h->ptr[1];

    // 힙의 루트 노드를 마지막 노드과 교환한다.
    h->ptr[1] = h->ptr[h->length];
    h->ptr[h->length--] = *i;

    _heap_sink(h, 1);

    return true;
}

/* 힙이 비어 있는지 확인한다. */
bool heap_is_empty(Heap *h) {
    return (h == NULL) || (h->length <= 0);
}

/* 힙에 저장된 항목의 개수를 반환한다. */
size_t heap_size(Heap *h) {
    return (h != NULL) ? h->length : 0;
}

#endif // `HEAP_IMPLEMENTATION`