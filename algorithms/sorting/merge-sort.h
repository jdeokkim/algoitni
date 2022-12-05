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

#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 정렬할 원소의 자료형. */
typedef int T;

/* | 라이브러리 함수... | */

/* 주어진 배열을 병합 정렬한다. */
void merge_sort(T *values, size_t n);

#endif // `MERGE_SORT_H`

#ifdef MERGE_SORT_IMPLEMENTATION

/* | 라이브러리 함수... | */

/* 주어진 배열의 부분 배열을 병합 정렬한다. */
static void merge_sort_helper(T *values, T *aux, int low, int high) {
    if (values == NULL || aux == NULL || low >= high) return;

    // 배열을 반으로 나눈다.
    int mid = (low + high) / 2;

    // 왼쪽 절반과 오른쪽 절반을 각각 정렬한다.
    merge_sort_helper(values, aux, low, mid);
    merge_sort_helper(values, aux, mid + 1, high);

    // 왼쪽 절반과 오른쪽 절반에서 각각 선택한 항목의 인덱스.
    int i = low, j = mid + 1;

    // 임시 메모리 공간에 부분 배열의 내용을 복사한다.
    for (int k = low; k <= high; k++)
        aux[k] = values[k];

    // 두 개의 정렬된 부분 배열들을 하나로 합친다. 
    for (int k = low; k <= high; k++) {
        /*  
            1. 왼쪽 절반이 비어 있는 경우?
            2. 오른쪽 절반이 비어 있는 경우?
            3. 선택한 항목보다 오른쪽에 있는 항목이 더 작을 경우?
            4. 그 외의 경우?
        */

        if (i > mid) values[k] = aux[j++];
        else if (j > high) values[k] = aux[i++];
        else if (aux[j] < aux[i]) values[k] = aux[j++];
        else values[k] = aux[i++];
    }
}

/* 주어진 배열을 병합 정렬한다. */
void merge_sort(T *values, size_t n) {
    if (values == NULL || n <= 1) return;

    /*
        병합 정렬은 두 개의 정렬된 배열을 하나의 큰 정렬된 배열로 
        합치는 작업을 재귀적으로 반복하여 배열의 모든 원소를 정렬하는
        알고리즘이다. 병합 정렬의 평균 시간 복잡도는 `O(n * log n)`
        으로 효율적인 성능을 보여주지만, 배열의 크기에 비례하는 추가
        메모리 공간이 필요하다는 단점이 있다.

        병합 정렬의 원리는 아주 간단한데, 먼저 주어진 배열을 크기가 1인
        `n`개의 부분 배열로 나누고, 각각의 배열을 순환 호출을 이용해
        계속 합쳐 나가면 된다. 또한, 병합 정렬을 구현할 때는 작은 부분
        배열에 삽입 정렬을 이용하거나 배열이 이미 정렬되어 있는지를 확인
        하는 코드를 넣어 알고리즘의 실행 성능을 더 큰 폭으로 개선할 수 있다.
    */

    // 병합 정렬에 필요한 임시 메모리 공간을 할당한다.
    T *aux = malloc(n * sizeof *aux);

    merge_sort_helper(values, aux, 0, n - 1);

    // 임시 메모리 공간을 해제한다.
    free(aux);
}

#endif // `MERGE_SORT_IMPLEMENTATION`