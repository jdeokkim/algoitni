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

#define STACK_INIT_CAPACITY 32

typedef int Item;

typedef struct Stack {
    Item *ptr;  
    size_t length;
    size_t capacity;
} Stack;

Stack *stack_create(void);
void stack_release(Stack *s);
int stack_push(Stack *s, Item i);
int stack_pop(Stack *s, Item *const i);
int stack_is_empty(Stack *s);

int main(void) {
    /* TODO: ... */

    return 0;
}

Stack *stack_create(void) {
    Stack *s = malloc(sizeof(*s));

    s->length = 0;
    s->capacity = STACK_INIT_CAPACITY;

    s->ptr = malloc(s->capacity * sizeof(*(s->ptr)));

    return s;
}

void stack_release(Stack *s) {
    if (s == NULL) return;

    s->length = s->capacity = 0;

    free(s->ptr);
    free(s);
}

int stack_push(Stack *s, Item i) {
    if (s == NULL) return 0;

    if (s->length >= s->capacity) {
        const size_t new_capacity = 2 * s->capacity;

        Item *new_ptr = realloc(s->ptr, new_capacity * sizeof(*new_ptr));

        if (new_ptr == NULL) return 0;

        s->capacity = new_capacity;
        s->ptr = new_ptr;
    }

    s->ptr[s->length++] = i;

    return 1;
}

int stack_pop(Stack *s, Item *const i) {
    if (stack_is_empty(s) || i == NULL) return 0;

    *i = s->ptr[--s->length];

    return 1;
}

int stack_is_empty(Stack *s) {
    return (s == NULL) || (s->length <= 0);
}