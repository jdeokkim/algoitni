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

#ifndef GRAHAM_SCAN_H
#define GRAHAM_SCAN_H

/* | 자료형 선언 및 정의... | */

#ifndef RAYLIB_H

/* 2차원 벡터를 나타내는 구조체. */
typedef struct Vector2 {
    float x;  // 2차원 벡터의 X 좌표.
    float y;  // 2차원 벡터의 Y 좌표.
} Vector2;

#endif

/* | 라이브러리 함수... | */

/* 그레이엄 스캔 알고리즘을 이용하여, 볼록 껍질을 생성한다. */
int graham_scan(Vector2 *points, int n, Vector2 *result);

#endif // `GRAHAM_SCAN_H`

#ifdef GRAHAM_SCAN_IMPLEMENTATION

#include <float.h>
#include <math.h>
#include <stdlib.h>

/* | 라이브러리 함수... | */

/* 두 부동 소수점 값이 서로 근접한지 확인한다. */
static int approx_equals(float f1, float f2) {
    return fabsf(f1 - f2) <= fmaxf(1.0f, fmaxf(f1, f2)) * FLT_EPSILON;
}

/* 세 점이 반시계 방향으로 정렬되어 있는지 확인한다. */
static int vector2_ccw(Vector2 v1, Vector2 v2, Vector2 v3) {
    const float lhs = (v2.y - v1.y) * (v3.x - v1.x);
    const float rhs = (v3.y - v1.y) * (v2.x - v1.x);

    return (lhs > rhs) - (lhs < rhs);
}

/* 두 점을 "각 점과 정렬 기준점을 이은 직선이 X축과 이루는 각도"를 기준으로 비교한다. */
static int compare_points(const void *a, const void *b) {
    const Vector2 *p = a, *q = b;

    int ccw = vector2_ccw((Vector2) { 0.0f }, *p, *q);

    if (ccw == 0) return (q->x * q->x + q->y * q->y) >= (p->x * p->x + p->y * p->y) ? 1 : -1;
    else return (ccw == 1) ? 1 : -1;
}

/* 그레이엄 스캔 알고리즘을 이용하여, 볼록 껍질을 생성한다. */
int graham_scan(Vector2 *points, int n, Vector2 *result) {
    if (points == NULL || n < 3 || result == NULL) return 0;

    int lowest_index = 0, count = 0;

    /*
        먼저 가장 아래에 있는 점을 찾는다. 이때, 그러한 점이 여러 개인 경우에는
        가장 왼쪽에 있는 점을 선택한다.
    */
    for (int i = 1; i < n; i++)
        if ((points[lowest_index].y > points[i].y)
            || (approx_equals(points[lowest_index].y, points[i].y)
                && points[lowest_index].x > points[i].x))
            lowest_index = i;

    const Vector2 temp = points[0];

    // 바로 이 점이 정렬 기준점이 된다.
    points[0] = points[lowest_index];
    points[lowest_index] = temp;

    // 각 점을 평행 이동시킨다.
    for (int i = 1; i < n; i++)
        points[i].x -= points[0].x, points[i].y -= points[0].y;

    // 모든 점을 "각 점과 정렬 기준점을 이은 직선이 X축과 이루는 각도" 오름차순으로 정렬한다.
    qsort(points + 1, n - 1, sizeof *points, compare_points);
    
    for (int i = 1; i < n; i++)
        points[i].x += points[0].x, points[i].y += points[0].y;

    /* TODO: ... */

    return count;
}

#endif // `GRAHAM_SCAN_IMPLEMENTATION`