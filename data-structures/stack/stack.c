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

#define STACK_IMPLEMENTATION
#include "stack.h"

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/stack.out` */

int main(void) {
    Stack *s = stack_create();

    stack_push(s, (Item) 10);
    stack_push(s, (Item) 20);
    stack_push(s, (Item) 30);

    for (;;) {
        Item i;

        if (!stack_pop(s, &i)) break;

        printf("%d (%ld)\n", i, stack_size(s));
    }

    stack_release(s);

    return 0;
}