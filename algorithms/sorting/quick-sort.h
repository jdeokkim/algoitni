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

#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* | 자료형 선언 및 정의... | */

/* 정렬할 원소의 자료형. */
typedef int T;

/* | 라이브러리 함수... | */

/* 주어진 배열을 퀵 정렬한다. */
void quick_sort(T *values, size_t n);

#endif // `QUICK_SORT_H`

#ifdef QUICK_SORT_IMPLEMENTATION

/* | 전역 변수 정의... | */

static int initialized = 0;

/* | 라이브러리 함수... | */

/* 주어진 배열의 모든 원소를 무작위로 섞는다. */
static void fisher_yates_shuffle(T *values, size_t n) {
    if (values == NULL || n <= 1) return;

    if (!initialized) srand((unsigned int) time(NULL));

    for (int i = n - 1; i >= 1; i--) {
        int j = rand() % n;

        T value = values[i];

        values[i] = values[j];
        values[j] = value;
    }
}

/* 주어진 배열에서 임의의 항목을 선택하고, 이 배열을 적절하게 분할한다.*/
static int quick_sort_partition(T *values, int low, int high) {
    if (values == NULL || low >= high) return high + 1;

    int i = low, j = high + 1;

    // 기준이 되는 값을 선택한다.
    T pivot = values[low];

    for (;;) {
        // 배열의 왼쪽에서부터 오른쪽으로 살펴본다.
        while (values[++i] < pivot) ;

        // 배열의 오른쪽에서부터 왼쪽으로 살펴본다. 
        while (pivot < values[--j]) ;

        if (i >= j) break;

        T value = values[i];

        values[i] = values[j];
        values[j] = value;
    }

    T value = values[low];

    values[low] = values[j];
    values[j] = value;

    return j;
}

/* (주어진 배열을 퀵 정렬한다.) */
static void quick_sort_helper(T *values, int low, int high) {
    if (values == NULL || low >= high) return;

    // 주어진 배열을 임의의 항목을 기준으로 적절하게 분할한다.
    int j = quick_sort_partition(values, low, high);

    quick_sort_helper(values, low, j - 1);
    quick_sort_helper(values, j + 1, high);
}

/* 주어진 배열을 퀵 정렬한다. */
void quick_sort(T *values, size_t n) {
    if (values == NULL || n <= 1) return;

    // 분할 기준 항목의 무작위성을 위해 배열을 섞는다.
    fisher_yates_shuffle(values, n);

    quick_sort_helper(values, 0, n - 1);
}

#endif // `QUICK_SORT_IMPLEMENTATION`