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

#ifndef TWO_LINES_H
#define TWO_LINES_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/* | 자료형 선언 및 정의... | */

/* 2차원 벡터를 나타내는 구조체. */
typedef struct Vec2 {
    double x;
    double y;
} Vec2;

/* | 라이브러리 함수... | */

/* 점 `p0`, `p1`을 잇는 선분과 점 `q0`, `q1`을 잇는 선분이 서로 만나는지 확인한다. */
bool intersects(Vec2 p0, Vec2 p1, Vec2 q0, Vec2 q1, Vec2 *const v);

#endif // `TWO_LINES_H`

#ifdef TWO_LINES_IMPLEMENTATION

/* 점 `p0`, `p1`을 잇는 선분과 점 `q0`, `q1`을 잇는 선분이 서로 만나는지 확인한다. */
bool intersects(Vec2 p0, Vec2 p1, Vec2 q0, Vec2 q1, Vec2 *const v) {
    Vec2 r = { p1.x - p0.x, p1.y - p0.y };
    Vec2 s = { q1.x - q0.x, q1.y - q0.y };

    double rXs = r.x * s.y - r.y * s.x;

    Vec2 qp = { q0.x - p0.x, q0.y - p0.y };

    double qpXs = qp.x * s.y - qp.y * s.x;
    double qpXr = qp.x * r.y - qp.y * r.x;

    // 두 선분의 기울기가 서로 같은 경우?
    if (rXs == 0.0) {
        // 두 선분이 한 직선 위에 있는 경우?
        if (qpXr == 0.0) {
            double rDr = r.x * r.x + r.y * r.y;
            double sDr = s.x * r.x + s.y * r.y;

            double inverse_rDr = (1.0 / rDr);

            double t0 = (qp.x * r.x + qp.y * r.y) * inverse_rDr;
            double t1 = t0 + sDr * inverse_rDr;

            if (sDr < 0) {
                double temp = t0;

                t0 = t1;
                t1 = temp;
            }
            
            // 단 하나의 점에서만 만나는 경우?
            if (t0 < 0.0 && t1 == 0.0 || t0 == 1.0 && t1 > 1.0) {
                if (v != NULL) {
                    if ((p0.x == q0.x && p0.y == q0.y) 
                        || (p0.x == q1.x && p0.y == q1.y)) *v = p0;
                    else if ((p1.x == q0.x && p1.y == q0.y) 
                        || (p1.x == q1.x && p1.y == q1.y)) *v = p1;
                }

                return 1;
            } else {
                return (t1 >= 0.0 && t0 <= 1.0);
            }
        }

        return false;
    }

    double inverse_rXs = 1.0 / rXs;

    double t = qpXs * inverse_rXs;
    double u = qpXr * inverse_rXs;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        if (v != NULL) {
            v->x = p0.x + t * r.x;
            v->y = p0.y + t * r.y;
        }

        return true;
    }

    return false;
}

#endif // `TWO_LINES_IMPLEMENTATION`