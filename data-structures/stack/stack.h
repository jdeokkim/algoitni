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

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define STACK_INIT_CAPACITY 32

/* | 자료형 선언 및 정의... | */

/* 스택에 저장되는 항목의 자료형. */
typedef int Item;

/* 스택을 나타내는 추상 자료형. */
typedef struct Stack Stack;

/* | 라이브러리 함수... | */

/* 스택을 생성한다. */
Stack *stack_create(void);

/* 스택에 할당된 메모리를 해제한다. */
void stack_release(Stack *s);

/* 스택에 새로운 항목을 추가한다. */
bool stack_push(Stack *s, Item i);

/* 스택에서 가장 최근에 추가된 항목을 꺼낸다. */
bool stack_pop(Stack *s, Item *const i);

/* 스택이 비어 있는지 확인한다. */
bool stack_is_empty(Stack *s);

/* 스택에 저장된 항목의 개수를 반환한다. */
size_t stack_size(Stack *s);

#endif // `STACK_H`

#ifdef STACK_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 스택을 나타내는 구조체. */
struct Stack {
    Item *ptr;        // 항목의 배열.
    size_t length;    // 항목의 현재 개수.
    size_t capacity;  // 항목의 최대 개수.
};

/* | 라이브러리 함수... | */

/* 스택을 생성한다. */
Stack *stack_create(void) {
    Stack *s = malloc(sizeof(*s));

    s->length = 0;
    s->capacity = STACK_INIT_CAPACITY;

    s->ptr = malloc(s->capacity * sizeof(*(s->ptr)));

    return s;
}

/* 스택에 할당된 메모리를 해제한다. */
void stack_release(Stack *s) {
    if (s == NULL) return;

    s->length = s->capacity = 0;

    free(s->ptr);
    free(s);
}

/* 스택에 새로운 항목을 추가한다. */
bool stack_push(Stack *s, Item i) {
    if (s == NULL) return false;

    if (s->length >= s->capacity) {
        size_t new_capacity = 2 * s->capacity;

        Item *new_ptr = realloc(s->ptr, new_capacity * sizeof(*new_ptr));

        if (new_ptr == NULL) return false;

        s->capacity = new_capacity;
        s->ptr = new_ptr;
    }

    s->ptr[s->length++] = i;

    return true;
}

/* 스택에서 가장 최근에 추가된 항목을 꺼낸다. */
bool stack_pop(Stack *s, Item *const i) {
    if (stack_is_empty(s) || i == NULL) return false;

    *i = s->ptr[--s->length];

    return true;
}

/* 스택이 비어 있는지 확인한다. */
bool stack_is_empty(Stack *s) {
    return (s == NULL) || (s->length <= 0);
}

/* 스택에 저장된 항목의 개수를 반환한다. */
size_t stack_size(Stack *s) {
    return (s != NULL) ? s->length : 0;
}

#endif // `STACK_IMPLEMENTATION`