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

#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 정렬할 원소의 자료형. */
typedef int T;

/* | 라이브러리 함수... | */

/* 주어진 배열을 선택 정렬한다. */
void selection_sort(T *values, size_t n);

#endif // `SELECTION_SORT_H`

#ifdef SELECTION_SORT_IMPLEMENTATION

/* | 라이브러리 함수... | */

/* 주어진 배열을 선택 정렬한다. */
void selection_sort(T *values, size_t n) {
    if (values == NULL || n <= 1) return;

    /*
        선택 정렬에서는 먼저 배열에서 가장 작은 원소를 찾고,
        그 원소와 첫 번째 원소를 맞바꾼 다음, 다시 배열에서
        두 번째로 작은 원소를 찾아 두 번째 원소와 맞바꾸고,
        이러한 과정을 배열이 정렬될 때까지 반복한다.
    */

    for (int i = 0; i < n; i++) {
        // 아직 정렬이 안된 곳부터 확인한다.
        int min_index = i;

        // 가장 작은 항목을 찾는다.
        for (int j = i + 1; j < n; j++)
            if (values[min_index] > values[j])
                min_index = j;
        
        T min_value = values[min_index];

        values[min_index] = values[i];
        values[i] = min_value;
    }
}

#endif // `SELECTION_SORT_IMPLEMENTATION`