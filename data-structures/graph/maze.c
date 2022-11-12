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

#define MAZE_IMPLEMENTATION
#include "maze.h"

#define MAZE_WIDTH   6
#define MAZE_HEIGHT  4

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/maze.out` */

static const int INPUT[MAZE_HEIGHT][MAZE_WIDTH] = {
    { 1, 0, 1, 1, 1, 1 },
    { 1, 0, 1, 0, 1, 0 },
    { 1, 0, 1, 0, 1, 1 },
    { 1, 1, 1, 0, 1, 1 }
};  

int main(void) {
    Maze *m = maze_create(MAZE_WIDTH, MAZE_HEIGHT);

    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            maze_set_cell(m, (Point) { x, y }, INPUT[y][x]);

    maze_bfs(m, (Point) { 0, 0 });

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++)
            printf(
                "%3d ", 
                maze_get_visited(
                    m, 
                    (Point) { x, y }
                )
            );

        printf("\n");
    }

    maze_release(m);
    
    return 0;
}