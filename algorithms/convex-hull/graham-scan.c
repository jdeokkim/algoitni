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

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/graham-scan.out` */

#include "raylib.h"

#define GRAHAM_SCAN_IMPLEMENTATION
#include "graham-scan.h"

#define TARGET_FPS       60

#define SCREEN_WIDTH     800
#define SCREEN_HEIGHT    600

#define MAX_POINT_COUNT  512

typedef struct {
    Vector2 *points;
    int count;
} PtArray;

static void GenerateHull(const PtArray *input, PtArray *output);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jdeokkim/algoitni | graham-scan.c");

    SetTargetFPS(TARGET_FPS);

    PtArray input = { .count = MAX_POINT_COUNT };
    PtArray output = { .count = MAX_POINT_COUNT };

    input.points = RL_MALLOC(input.count * sizeof(*(input.points)));
    output.points = RL_MALLOC(output.count * sizeof(*(output.points)));

    GenerateHull(&input, &output);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) GenerateHull(&input, &output);

        BeginDrawing();
        
        ClearBackground(BLACK);

        for (int i = 0; i < input.count; i++) {
            DrawCircleV(input.points[i], 2.0f, WHITE);

            DrawTextEx(
                GetFontDefault(),
                TextFormat("%d", i),
                (Vector2) { 
                    input.points[i].x + 6.0f,
                    input.points[i].y + 6.0f
                },
                10.0f,
                1.0f,
                ColorAlphaBlend(
                    GREEN, 
                    RED, 
                    Fade(WHITE, (float) i / input.count)
                )
            );
        }

        if (output.count > 0) {
            for (int i = 0; i < output.count; i++) {
                DrawCircleV(output.points[i], 4.0f, DARKGREEN);

                DrawLineEx(
                    output.points[i], 
                    output.points[(i + 1) % output.count], 
                    1.0f, 
                    DARKGREEN
                );
            }

            DrawLineEx(
                (Vector2) { 0.0f, output.points[0].y },
                (Vector2) { SCREEN_WIDTH, output.points[0].y },
                1.0f,
                Fade(DARKGREEN, 0.35f)
            );
        }

        DrawFPS(8, 8);
        
        EndDrawing();
    }

    RL_FREE(input.points);

    CloseWindow();

    return 0;
}

static void GenerateHull(const PtArray *input, PtArray *output) {
    if (input == NULL || output == NULL) return;

    for (int i = 0; i < input->count; i++) {
        const int offset = GetRandomValue(50, 250);

        input->points[i].x = GetRandomValue(offset, SCREEN_WIDTH - offset);
        input->points[i].y = GetRandomValue(offset, SCREEN_HEIGHT - offset);
    }

    output->count = graham_scan(input->points, input->count, output->points);
}