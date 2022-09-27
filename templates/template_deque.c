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

typedef struct DequeNode {
    struct DequeNode *prev;
    struct DequeNode *next; 
    Item value; 
} DequeNode;

typedef struct Deque {
    DequeNode *head;
    DequeNode *tail;
    size_t length;
} Deque;

Deque *deque_create(void);
void deque_release(Deque *d);
int deque_push_left(Deque *d, Item i);
int deque_push_right(Deque *d, Item i);
int deque_pop_left(Deque *d, Item *const i);
int deque_pop_right(Deque *d, Item *const i);
int deque_is_empty(Deque *d);

int main(void) {
    /* TODO: ... */

    return 0;
}

static DequeNode *_deque_node_create(Item i) {
    DequeNode *node = malloc(sizeof(*node));

    node->prev = node->next = NULL;
    node->value = i;

    return node;
}

static void _deque_node_release(DequeNode *node) {
    if (node == NULL) return;

    free(node);
}

Deque *deque_create(void) {
    return calloc(1, sizeof(Deque));
}

void deque_release(Deque *d) {
    if (d == NULL) return;

    d->length = 0;

    DequeNode *node = d->head;

    while (node != NULL) {
        _deque_node_release(node->prev);

        node = node->next;
    }

    _deque_node_release(d->tail);
    
    free(d);
}

int deque_push_left(Deque *d, Item i) {
    if (d == NULL) return 0;

    if (d->head == NULL) {
        d->head = d->tail = _deque_node_create(i);
    } else {
        DequeNode *node = _deque_node_create(i);

        node->next = d->head;
        d->head->prev = node;

        d->head = node;
    }

    d->length++;

    return 1;
}

int deque_push_right(Deque *d, Item i) {
    if (d == NULL) return 0;

    if (d->head == NULL) {
        d->head = d->tail = _deque_node_create(i);
    } else {
        DequeNode *node = _deque_node_create(i);

        node->prev = d->tail;
        d->tail->next = node;

        d->tail = node;
    }

    d->length++;

    return 1;
}

int deque_pop_left(Deque *d, Item *const i) {
    if (deque_is_empty(d) || i == NULL) return 0;

    DequeNode *node = d->head->next;

    *i = d->head->value;

    _deque_node_release(d->head);

    if (node == NULL) {
        d->head = d->tail = NULL;
    } else {
        node->prev = NULL;
        d->head = node;
    }

    d->length--;

    return 1;
}

int deque_pop_right(Deque *d, Item *const i) {
    if (deque_is_empty(d) || i == NULL) return 0;

    DequeNode *node = d->tail->prev;

    *i = d->tail->value;

    _deque_node_release(d->tail);

    if (node == NULL) {
        d->head = d->tail = NULL;
    } else {
        node->next = NULL;
        d->tail = node;
    }

    d->length--;

    return 1;
}

int deque_is_empty(Deque *d) {
    return (d == NULL) || (d->length <= 0);
}
