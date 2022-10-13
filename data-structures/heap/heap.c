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

#define HEAP_IMPLEMENTATION
#include "heap.h"

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/heap.out` */

static int on_heap_compare(Item i1, Item i2);

int main(void) {
    Heap *h = heap_create(on_heap_compare);

    heap_insert(h, (Item) 23);
    heap_insert(h, (Item) 56);
    heap_insert(h, (Item) 11);
    heap_insert(h, (Item) 9);
    heap_insert(h, (Item) 56);
    heap_insert(h, (Item) 99);
    heap_insert(h, (Item) 27);
    heap_insert(h, (Item) 34);

    for (int i = 1; i <= h->length; i++)
        printf("%d -> ", h->ptr[i]);

    printf("_\n");

    for (;;) {
        Item i;

        if (!heap_delete_root(h, &i)) break;

        printf("%d (%ld)\n", i, heap_size(h));
    }

    heap_release(h);
    
    return 0;
}

static int on_heap_compare(Item i1, Item i2) {
    return (i1 > i2) - (i1 < i2);
}