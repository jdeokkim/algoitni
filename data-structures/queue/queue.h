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

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 큐에 저장되는 항목의 자료형. */
typedef int Item;

/* 큐를 나타내는 추상 자료형. */
typedef struct Queue Queue;

/* | 라이브러리 함수... | */

/* 큐를 생성한다. */
Queue *queue_create(void);

/* 큐에 할당된 메모리를 해제한다. */
void queue_release(Queue *q);

/* 큐에 새로운 항목을 추가한다. */
bool queue_enqueue(Queue *q, Item i);

/* 큐에서 가장 먼저 추가된 항목을 꺼낸다. */
bool queue_dequeue(Queue *q, Item *const i);

/* 큐가 비어 있는지 확인한다. */
bool queue_is_empty(Queue *q);

/* 큐에 저장된 항목의 개수를 반환한다. */
size_t queue_size(Queue *q);

#endif // `QUEUE_H`

#ifdef QUEUE_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 연결 리스트의 노드를 나타내는 구조체. */
typedef struct QueueNode {
    struct QueueNode *next;  // 이 노드의 다음 노드.
    Item value;              // 노드에 저장된 값.
} QueueNode;

/* 큐를 나타내는 구조체. */
struct Queue {
    QueueNode *head;  // 연결 리스트의 첫 번째 노드.
    QueueNode *tail;  // 연결 리스트의 마지막 노드.
    size_t length;    // 항목의 현재 개수.
};

/* | 라이브러리 함수... | */

/* 연결 리스트의 노드를 생성한다. */
static QueueNode *_queue_node_create(Item i) {
    QueueNode *node = malloc(sizeof(*node));

    node->next = NULL;
    node->value = i;

    return node;
}

/* 연결 리스트의 노드에 할당된 메모리를 해제한다. */
static void _queue_node_release(QueueNode *node) {
    free(node);
}

/* 큐를 생성한다. */
Queue *queue_create(void) {
    return calloc(1, sizeof(Queue));
}

/* 큐에 할당된 메모리를 해제한다. */
void queue_release(Queue *q) {
    if (q == NULL) return;

    q->length = 0;

    QueueNode *node = q->head;

    while (node != NULL) {
        QueueNode *next = node->next;

        _queue_node_release(node);

        node = next;
    }

    free(q);
}

/* 큐에 새로운 항목을 추가한다. */
bool queue_enqueue(Queue *q, Item i) {
    if (q == NULL) return false;

    if (q->head == NULL) {
        q->head = q->tail = _queue_node_create(i);
    } else {
        QueueNode *node = _queue_node_create(i);

        q->tail->next = node;
        q->tail = node;
    }

    q->length++;

    return true;
}

/* 큐에서 가장 먼저 추가된 항목을 꺼낸다. */
bool queue_dequeue(Queue *q, Item *const i) {
    if (queue_is_empty(q) || i == NULL) return false;

    QueueNode *node = q->head->next;

    *i = q->head->value;

    _queue_node_release(q->head);

    if (node == NULL) q->head = q->tail = NULL;
    else q->head = node;

    q->length--;

    return true;
}

/* 큐가 비어 있는지 확인한다. */
bool queue_is_empty(Queue *q) {
    return (q == NULL) || (q->length <= 0);
}

/* 큐에 저장된 항목의 개수를 반환한다. */
size_t queue_size(Queue *q) {
    return (q != NULL) ? q->length : 0;
}

#endif // `QUEUE_IMPLEMENTATION`