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

#ifndef SHELL_SORT_H
#define SHELL_SORT_H

#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 정렬할 원소의 자료형. */
typedef int T;

/* | 라이브러리 함수... | */

/* 주어진 배열을 셸 정렬한다. */
void shell_sort(T *values, size_t n);

#endif // `SHELL_SORT_H`

#ifdef SHELL_SORT_IMPLEMENTATION

/* | 라이브러리 함수... | */

/* 주어진 배열을 셸 정렬한다. */
void shell_sort(T *values, size_t n) {
    if (values == NULL || n <= 1) return;

    /*
        셸 정렬은 서로 멀리 떨어진 원소 사이에도 교환이
        일어날 수 있도록 삽입 정렬을 개선한 알고리즘으로,
        적절한 간격 시퀀스 (gap sequence)를 이용하여
        배열을 h-정렬하면 더 효율적으로 정렬을 마칠 수 있다.

        셸 정렬은 코드 길이가 비교적 짧고, 추가적인 메모리
        공간을 사용하지 않으며, 비교적 괜찮은 성능을 보여주기
        때문에, 임베디드 환경에서 정렬 알고리즘이 필요할 때 
        사용해볼 수 있다.
    */

    // 삽입 정렬의 간격.
    int h = 1;

    // 여기서는 `O(n^(3/2))`의 Knuth's Sequence를 이용한다.
    while (h < n / 3) 
        h = 3 * h + 1; // 1, 4, 13, 40, 121, 364, ...

    while (h >= 1) {
        // 배열을 h-정렬한다.
        for (int i = h; i < n; i++) {
            for (int j = i; j >= h && values[j] < values[j - h]; j -= h) {
                T value = values[j];

                values[j] = values[j - 1];
                values[j - 1] = value;
            }
        }

        h /= 3;
    }
}

#endif // `SHELL_SORT_IMPLEMENTATION`