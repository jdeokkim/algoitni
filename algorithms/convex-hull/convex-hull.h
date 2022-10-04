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

#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <stdbool.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

#ifndef RAYLIB_H

/* 2차원 벡터를 나타내는 구조체. */
typedef struct Vector2 {
    long long int x;  // 2차원 벡터의 X 좌표.
    long long int y;  // 2차원 벡터의 Y 좌표.
} Vector2;

#endif // `RAYLIB_H`

/* | 라이브러리 함수... | */

/* 선물 포장 알고리즘을 이용하여, 볼록 껍질을 생성한다. */
int jarvis_march(const Vector2 *arr, int n, Vector2 *hull);

/* 그레이엄 스캔 알고리즘을 이용하여, 볼록 껍질을 생성한다. */
int graham_scan(const Vector2 *arr, int n, Vector2 *hull);

#endif // `CONVEX_HULL_H`

#ifdef CONVEX_HULL_IMPLEMENTATION

/* | 라이브러리 함수... | */

/* 세 점이 반시계 방향으로 정렬되어 있는지 확인한다. */
static int vector2_ccw(Vector2 v1, Vector2 v2, Vector2 v3) {
    /*
       (v1)
        *
         \
          \
           \
            *-----------*
           (v2)        (v3)

        세 점이 반시계 방향?
        - `v1`과 `v2`를 잇는 직선의 기울기가 `v1`과 `v2`를 
          잇는 직선의 기울기보다 크다!
    */

    const long long int lhs = (v2.y - v1.y) * (v3.x - v1.x);
    const long long int rhs = (v3.y - v1.y) * (v2.x - v1.x);

    return (lhs > rhs) - (lhs < rhs);
}

/* 두 점 사이의 거리의 제곱을 반환한다. */
static long long int vector2_length_sqr(Vector2 v1, Vector2 v2) {
    return (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y);
} 

/* 선물 포장 알고리즘을 이용하여, 볼록 껍질을 생성한다. */
int jarvis_march(const Vector2 *arr, int n, Vector2 *hull) {
    if (arr == NULL || hull == NULL || n < 3) return 0;
    
    int leftmost_index = 0, result = 0;

    // 먼저 가장 왼쪽에 있는 점을 찾는다.
    for (int i = 1; i < n; i++)
        if (arr[leftmost_index].x > arr[i].x)
            leftmost_index = i;

    // 이 점이 볼록 껍질의 첫 번째 점이 된다.
    hull[result++] = arr[leftmost_index];

    int current_index, next_index;

    current_index = next_index = leftmost_index;

    for (;;) {
        // 점 하나를 선택한다.
        for (int i = 0; i < n; i++) {
            if (i == current_index) continue;

            next_index = i;

            break;
        }

        // 기준점과 선택한 점 사이에 더 적합한 점이 있는지 확인한다.
        for (int i = 0; i < n; i++) {
            if (i == current_index || i == next_index) continue;

            const int direction = vector2_ccw(arr[current_index], arr[i], arr[next_index]);

            // 세 점이 일직선 위에 있을 경우, 거리의 제곱을 비교한다.
            const int compare_lengths = vector2_length_sqr(arr[current_index], arr[i]) 
                > vector2_length_sqr(arr[current_index], arr[next_index]);

            if (direction > 0 || (direction == 0 && compare_lengths))
                next_index = i;
        }

        // 첫 번째 점으로 다시 되돌아왔다면, 볼록 껍질 생성이 완료된 것이다.
        if (next_index == leftmost_index) break;

        current_index = next_index;
        
        hull[result++] = arr[next_index];
    }

    return result;
}

/* 그레이엄 스캔 알고리즘을 이용하여, 볼록 껍질을 생성한다. */
int graham_scan(const Vector2 *arr, int n, Vector2 *hull) {
    if (arr == NULL || hull == NULL || n < 3) return 0;

    /* TODO: ... */
}

#endif // `CONVEX_HULL_IMPLEMENTATION`