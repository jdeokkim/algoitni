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

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 링 버퍼에 저장되는 항목의 자료형. */
typedef int Item;

/* 링 버퍼를 나타내는 추상 자료형. */
typedef struct RingBuffer RingBuffer;

/* | 라이브러리 함수... | */

/* 링 버퍼를 생성한다. */
RingBuffer *ring_buffer_create(int capacity);

/* 링 버퍼에 할당된 메모리를 해제한다. */
void ring_buffer_release(RingBuffer *q);

/* 링 버퍼에 새로운 항목을 추가한다. */
bool ring_buffer_enqueue(RingBuffer *q, Item i);

/* 링 버퍼에서 가장 먼저 추가된 항목을 꺼낸다. */
bool ring_buffer_dequeue(RingBuffer *q, Item *const i);

/* 링 버퍼가 비어 있는지 확인한다. */
bool ring_buffer_is_empty(RingBuffer *q);

/* 링 버퍼가 포화 상태인지 확인한다. */
bool ring_buffer_is_full(RingBuffer *q);

/* 링 버퍼에 저장된 항목의 개수를 반환한다. */
size_t ring_buffer_size(RingBuffer *q);

#endif // `RING_BUFFER_H`

#ifdef RING_BUFFER_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 링 버퍼를 나타내는 구조체. */
struct RingBuffer {
    Item *ptr;        // 항목의 배열.
    size_t capacity;  // 항목의 최대 개수.
    int start, end;   // 첫 번째와 마지막 항목의 인덱스.
};

/* | 라이브러리 함수... | */

/* 링 버퍼를 생성한다. */
RingBuffer *ring_buffer_create(int capacity) {
    RingBuffer *q = malloc(sizeof(*q));

    q->start = q->end = 0;
    
    // 포화 상태 구분을 위해 빈 공간을 추가적으로 할당한다.
    q->capacity = capacity + 1;

    q->ptr = malloc(q->capacity * sizeof(*(q->ptr)));

    return q;
}

/* 링 버퍼에 할당된 메모리를 해제한다. */
void ring_buffer_release(RingBuffer *q) {
    if (q == NULL) return;

    q->start = q->end = 0;

    free(q->ptr);
    free(q);
}

/* 링 버퍼에 새로운 항목을 추가한다. */
bool ring_buffer_enqueue(RingBuffer *q, Item i) {
    if (q == NULL || ring_buffer_is_full(q)) return false;

    q->end = (q->end + 1) % q->capacity;
    q->ptr[q->end] = i;

    return true;
}

/* 링 버퍼에서 가장 먼저 추가된 항목을 꺼낸다. */
bool ring_buffer_dequeue(RingBuffer *q, Item *const i) {
    if (ring_buffer_is_empty(q) || i == NULL) return false;

    q->start = (q->start + 1) % q->capacity;
    *i = q->ptr[q->start];

    return true;
}

/* 링 버퍼가 비어 있는지 확인한다. */
bool ring_buffer_is_empty(RingBuffer *q) {
    return (q == NULL) || (q->start == q->end);
}

/* 링 버퍼가 포화 상태인지 확인한다. */
bool ring_buffer_is_full(RingBuffer *q) {
    return (q != NULL) && (q->start == (q->end + 1) % q->capacity);
}

/* 링 버퍼에 저장된 항목의 개수를 반환한다. */
size_t ring_buffer_size(RingBuffer *q) {
    return (q->end - q->start);
}

#endif // `RING_BUFFER_IMPLEMENTATION`