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

#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 정렬할 원소의 자료형. */
typedef int T;

/* | 라이브러리 함수... | */

/* 주어진 배열을 버블 정렬한다. */
void bubble_sort(T *values, size_t n);

#endif // `BUBBLE_SORT_H`

#ifdef BUBBLE_SORT_IMPLEMENTATION

/* | 라이브러리 함수... | */

/* 주어진 배열을 버블 정렬한다. */
void bubble_sort(T *values, size_t n) {
    if (values == NULL || n <= 1) return;

    /*
        버블 정렬은 무조건 인접한 2개의 원소만을 비교하는
        "무식한" 정렬 방법이다.
    */

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - 1; j++)
            if (values[j] > values[j + 1]) {
                T value = values[j];

                values[j] = values[j + 1];
                values[j + 1] = value;
            }
}

#endif // `BUBBLE_SORT_IMPLEMENTATION`