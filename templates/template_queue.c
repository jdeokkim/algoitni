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

typedef int Item;

typedef struct QueueNode {
    struct QueueNode *next;
    Item value;
} QueueNode;

typedef struct Queue {
    QueueNode *head, *tail;
    size_t length;
} Queue;

Queue *queue_create(void);
void queue_release(Queue *q);
int queue_enqueue(Queue *q, Item i);
int queue_dequeue(Queue *q, Item *const i);
int queue_is_empty(Queue *q);

int main(void) {
    /* TODO: ... */

    return 0;
}

static QueueNode *_queue_node_create(Item i) {
    QueueNode *node = malloc(sizeof(*node));

    node->next = NULL;
    node->value = i;

    return node;
}

static void _queue_node_release(QueueNode *node) {
    free(node);
}

Queue *queue_create(void) {
    return calloc(1, sizeof(Queue));
}

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

int queue_enqueue(Queue *q, Item i) {
    if (q == NULL) return 0;

    if (q->head == NULL) {
        q->head = q->tail = _queue_node_create(i);
    } else {
        QueueNode *node = _queue_node_create(i);

        q->tail->next = node;
        q->tail = node;
    }

    q->length++;

    return 1;
}

int queue_dequeue(Queue *q, Item *const i) {
    if (queue_is_empty(q) || i == NULL) return 0;

    QueueNode *node = q->head->next;

    *i = q->head->value;

    _queue_node_release(q->head);

    if (node == NULL) q->head = q->tail = NULL;
    else q->head = node;

    q->length--;

    return 1;
}

int queue_is_empty(Queue *q) {
    return (q == NULL) || (q->length <= 0);
}