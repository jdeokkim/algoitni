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

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/convex-hull.out` */

#include "raylib.h"

#define CONVEX_HULL_IMPLEMENTATION
#include "convex-hull.h"

#define TARGET_FPS       60

#define SCREEN_WIDTH     800
#define SCREEN_HEIGHT    600

#define MAX_POINT_COUNT  100

static int hull_point_count = 0;

static void generate_points(Vector2 *points, Vector2 *hull, size_t n);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jdeokkim/algoitni | convex-hull.c");

    SetTargetFPS(TARGET_FPS);

    Vector2 *points = RL_MALLOC(MAX_POINT_COUNT * sizeof(*points));
    Vector2 *hull = RL_MALLOC(MAX_POINT_COUNT * sizeof(*hull));

    generate_points(points, hull, MAX_POINT_COUNT);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R))
            generate_points(points, hull, MAX_POINT_COUNT);

        BeginDrawing();
        
        ClearBackground(BLACK);

        for (int i = 0; i < MAX_POINT_COUNT; i++)
            DrawCircleV(points[i], 2.0f, WHITE);

        if (hull_point_count > 0) {
            for (int i = 0; i < hull_point_count; i++) {
                DrawCircleV(hull[i], 4.0f, GREEN);

                DrawLineEx(hull[i], hull[(i + 1) % hull_point_count], 2.0f, GREEN);
            }
        }

        DrawFPS(8, 8);
        
        EndDrawing();
    }

    RL_FREE(hull), RL_FREE(points);

    CloseWindow();

    return 0;
}

static void generate_points(Vector2 *points, Vector2 *hull, size_t n) {
    if (points == NULL || n == 0) return;

    for (int i = 0; i < n; i++) {
        const int offset = GetRandomValue(50, 250);

        points[i].x = GetRandomValue(offset, SCREEN_WIDTH - offset);
        points[i].y = GetRandomValue(offset, SCREEN_HEIGHT - offset);
    }

    // TODO: ...
    hull_point_count = jarvis_march(points, MAX_POINT_COUNT, hull);
}