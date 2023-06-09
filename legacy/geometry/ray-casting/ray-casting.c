/*
    Copyright (c) 2023 Jaedeok Kim (https://github.com/jdeokkim)

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

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/ray-casting.out` */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define TARGET_FPS        60

#define SCREEN_WIDTH      800
#define SCREEN_HEIGHT     600

#define MAX_VERTEX_COUNT  8

#define MAX_RAY_COUNT     128

typedef struct { 
    Vector2 vertices[MAX_VERTEX_COUNT];
    int n;
} Polygon;

typedef struct { Vector2 origin, direction; } Ray2;

typedef struct { Vector2 point; float dsq; } RaycastHit;

inline Vector2 vector2_add(Vector2 v1, Vector2 v2) {
    return (Vector2) { v1.x + v2.x, v1.y + v2.y };
}

inline Vector2 vector2_subtract(Vector2 v1, Vector2 v2) {
    return (Vector2) { v1.x - v2.x, v1.y - v2.y };
}

inline Vector2 vector2_scalar_multiply(Vector2 v, float s) {
    return (Vector2) { v.x * s, v.y * s };
}

inline float vector2_cross(Vector2 v1, Vector2 v2) {
    return (v1.x * v2.y) - (v1.y * v2.x);
}

inline float vector2_dot(Vector2 v1, Vector2 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}

inline float vector2_distance_sq(Vector2 v1, Vector2 v2) {
    const Vector2 v = vector2_subtract(v1, v2);

    return vector2_dot(v, v);
}

int compute_intersection_rays(Ray2 k, Ray2 l, float *_t) {
    const float rXs = vector2_cross(k.direction, l.direction);

    const Vector2 qp = vector2_subtract(l.origin, k.origin);

    const float qpXs = vector2_cross(qp, l.direction);
    const float qpXr = vector2_cross(qp, k.direction);

    if (rXs != 0.0f) {
        const float _rXs = 1.0f / rXs;

        const float t = qpXs * _rXs, u = qpXr * _rXs;

        if ((t >= 0.0f && t <= 1.0f) && (u >= 0.0f && u <= 1.0f)) {
            if (_t != NULL) *_t = t;

            return 1;
        }

        return 0;
    } else {
        if (qpXr != 0.0f) return 0;

        const float rDr = vector2_dot(k.direction, k.direction);
        const float sDr = vector2_dot(l.direction, k.direction);

        const float _rDr = 1.0f / rDr;

        const float qpDr = vector2_dot(qp, k.direction);

        float k, t0 = qpDr * _rDr, t1 = t0 + sDr * _rDr;

        if (sDr < 0.0f) k = t0, t0 = t1, t1 = k;

        if ((t0 < 0.0f && t1 == 0.0f) || (t0 == 1.0f && t1 > 1.0f)) {
            if (_t != NULL) *_t = (t0 == 1.0f) ? 1.0f : 0.0f;

            return 1;
        }

        return 2 * (t1 >= 0.0f && t0 <= 1.0f);
    }
}

int raycast_poly(const Polygon *s, Ray2 k, RaycastHit *r) {
    if (s == NULL || s->n < 3) return 0;

    int result = 0, intersection_count = 0;

    if (r != NULL) r->point.x = r->point.y = r->dsq = INT_MAX;

    for (int j = s->n - 1, i = 0; i < s->n; j = i, i++) {
        const Ray2 l = { 
            .origin = s->vertices[i],
            .direction = vector2_subtract(s->vertices[j], s->vertices[i])
        };

        float t = 0.0f;

        int intersects = compute_intersection_rays(k, l, &t);

        if (intersects) {
            result = 1, intersection_count++;

            if (r != NULL && intersects == 1) {
                const Vector2 point = vector2_add(
                    k.origin, 
                    vector2_scalar_multiply(k.direction, t)
                );

                const float dsq = vector2_distance_sq(k.origin, point);

                if (r->dsq > dsq) r->point = point, r->dsq = dsq;
            }
        }
    }

    return result + (intersection_count & 1);
}

static void DrawPolygon(Polygon *s, Color c) {
    for (int j = s->n - 1, i = 0; i < s->n; j = i, i++)
        DrawLineEx(s->vertices[j], s->vertices[i], 2.0f, c);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "6pm-crew/play");

    SetTargetFPS(TARGET_FPS);

    Polygon s = { 
        .n = 4,
        .vertices = {
            300.0f, 100.0f,
            500.0f, 100.0f,
            500.0f, 500.0f,
            200.0f, 300.0f
        }  
    };

    Ray2 m = { 
        .origin = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f }
    };

    while (!WindowShouldClose()) {
        m.origin = GetMousePosition();

        BeginDrawing();
        
        ClearBackground(BLACK);

        DrawFPS(8, 8);

        {
            const Vector2 position = GetMousePosition();

            for (int i = 0; i < MAX_RAY_COUNT; i++) {
                m.direction = Vector2Rotate(
                    (Vector2) { SCREEN_WIDTH, 0.0f }, 
                    (i + 1) * (DEG2RAD * (360.0f / MAX_RAY_COUNT))
                );

                RaycastHit r;

                const int result = raycast_poly(&s, m, &r);

                DrawLineEx(
                    m.origin,
                    result ? r.point : vector2_add(m.origin, m.direction),
                    1.0f,
                    (result == 2) ? ColorAlpha(RED, 0.75f) : ColorAlpha(WHITE, 0.75f)
                );
            }
        }

        DrawCircleV(m.origin, 4.0f, ColorAlpha(WHITE, 0.75f));

        DrawPolygon(&s, GREEN);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}