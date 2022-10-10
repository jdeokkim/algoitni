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

#define HEAP_INIT_CAPACITY 32

typedef int Item;

typedef int (*HeapCompareCb)(Item i1, Item i2);

typedef struct Heap {
    Item *ptr;
    HeapCompareCb func;
    size_t length;
    size_t capacity;
} Heap;

Heap *heap_create(HeapCompareCb func);
void heap_release(Heap *h);
int heap_insert(Heap *h, Item i);
int heap_delete_root(Heap *h, Item *const i);
int heap_is_empty(Heap *h);

int main(void) {
    /* TODO: ... */

    return 0;
}

Heap *heap_create(HeapCompareCb func) {
    Heap *h = malloc(sizeof(*h));

    h->func = func;

    h->length = 0;
    h->capacity = HEAP_INIT_CAPACITY; 

    h->ptr = malloc(h->capacity * sizeof(*(h->ptr)));

    return h;
}

void heap_release(Heap *h) {
    if (h == NULL) return;

    h->length = h->capacity = 0;

    h->func = NULL;

    free(h->ptr);
    free(h);
}

static void _heap_swim(Heap *h, int k) {
    if (h == NULL || h->func == NULL) return;

    while (k > 1 && h->func(h->ptr[k >> 1], h->ptr[k]) < 0) {
        const Item t = h->ptr[k >> 1];

        h->ptr[k >> 1] = h->ptr[k];
        h->ptr[k] = t;

        k >>= 1;
    }
}

int heap_insert(Heap *h, Item i) {
    if (h == NULL) return 0;

    h->ptr[++h->length] = i;

    _heap_swim(h, h->length);

    return 1;
}

static void _heap_sink(Heap *h, int k) {
    if (h == NULL || h->func == NULL) return;

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

int heap_delete_root(Heap *h, Item *const i) {
    if (heap_is_empty(h) || i == NULL) return 0;

    *i = h->ptr[1];

    h->ptr[1] = h->ptr[h->length];
    h->ptr[h->length--] = *i;

    _heap_sink(h, 1);

    return 1;
}

int heap_is_empty(Heap *h) {
    return (h == NULL) || (h->length <= 0);
}