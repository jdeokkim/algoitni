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

#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 정렬할 원소의 자료형. */
typedef int T;

/* | 라이브러리 함수... | */

/* 주어진 배열을 삽입 정렬한다. */
void insertion_sort(T *values, size_t n);

#endif // `INSERTION_SORT_H`

#ifdef INSERTION_SORT_IMPLEMENTATION

/* | 라이브러리 함수... | */

/* 주어진 배열을 삽입 정렬한다. */
void insertion_sort(T *values, size_t n) {
    if (values == NULL || n <= 1) return;

    /*
        삽입 정렬에서는 먼저 원소를 하나 선택하고,
        적절한 위치를 찾을 때까지 그 원소의 앞에 있는
        모든 원소를 한 칸씩 오른쪽으로 미는 과정을
        거치게 된다.
        
        삽입 정렬은 부분적으로 정렬된 배열에 적용될 때 
        최적의 성능을 보여주며, 배열의 크기가 10 이하일
        때도 좋은 성능을 보여준다.
    */

    // 두 번째 원소부터 선택한다.
    for (int i = 1; i < n; i++) {
        // 선택한 원소의 앞에 있는 모든 원소를 한 칸씩 오른쪽으로 민다.
        for (int j = i; j > 0 && values[j] < values[j - 1]; j--) {
            T value = values[j];

            values[j] = values[j - 1];
            values[j - 1] = value;
        }
    }
}

#endif // `INSERTION_SORT_IMPLEMENTATION`