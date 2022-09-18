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

#ifndef DEQUE_H
#define DEQUE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 덱에 저장되는 항목의 자료형. */
typedef int Item;

/* 덱을 나타내는 추상 자료형. */
typedef struct Deque Deque;

/* | 라이브러리 함수... | */

/* 덱을 생성한다. */
Deque *deque_create(void);

/* 덱에 할당된 메모리를 해제한다. */
void deque_release(Deque *d);

/* 덱의 왼쪽 끝에 새로운 항목을 추가한다. */
bool deque_push_left(Deque *d, Item i);

/* 덱의 오른쪽 끝에 새로운 항목을 추가한다. */
bool deque_push_right(Deque *d, Item i);

/* 덱의 왼쪽 끝에 있는 항목을 꺼낸다. */
bool deque_pop_left(Deque *d, Item *const i);

/* 덱의 오른쪽 끝에 있는 항목을 꺼낸다. */
bool deque_pop_right(Deque *d, Item *const i);

/* 덱이 비어 있는지 확인한다. */
bool deque_is_empty(Deque *d);

/* 덱에 저장된 항목의 개수를 반환한다. */
size_t deque_size(Deque *d);

#endif // `DEQUE_H`

#ifdef DEQUE_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 연결 리스트의 노드를 나타내는 구조체. */
typedef struct DequeNode {
    struct QueueNode *prev;  // 이 노드의 이전 노드.
    struct QueueNode *next;  // 이 노드의 다음 노드.
    Item value;              // 노드에 저장된 값.
} DequeNode;

/* 덱을 나타내는 구조체. */
struct Deque {
    DequeNode *head;  // 덱의 연결 리스트의 첫 번째 노드.
    DequeNode *tail;  // 덱의 연결 리스트의 마지막 노드.
    size_t length;    // 덱에 저장된 항목의 현재 개수.
};

/* | 라이브러리 함수... | */

/* 덱을 생성한다. */
Deque *deque_create(void) {
    /* TODO: ... */
}

/* 덱에 할당된 메모리를 해제한다. */
void deque_release(Deque *d) {
    /* TODO: ... */
}

/* 덱의 왼쪽 끝에 새로운 항목을 추가한다. */
bool deque_push_left(Deque *d, Item i) {
    /* TODO: ... */
}

/* 덱의 오른쪽 끝에 새로운 항목을 추가한다. */
bool deque_push_right(Deque *d, Item i) {
    /* TODO: ... */
}

/* 덱의 왼쪽 끝에 있는 항목을 꺼낸다. */
bool deque_pop_left(Deque *d, Item *const i) {
    /* TODO: ... */
}

/* 덱의 오른쪽 끝에 있는 항목을 꺼낸다. */
bool deque_pop_right(Deque *d, Item *const i) {
    /* TODO: ... */
}

/* 덱이 비어 있는지 확인한다. */
bool deque_is_empty(Deque *d) {
    /* TODO: ... */
}

/* 덱에 저장된 항목의 개수를 반환한다. */
size_t deque_size(Deque *d) {
    /* TODO: ... */
}

#endif // `DEQUE_IMPLEMENTATION`