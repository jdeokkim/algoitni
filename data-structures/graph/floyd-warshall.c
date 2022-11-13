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

#define FLOYD_WARSHALL_IMPLEMENTATION
#include "floyd-warshall.h"

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/floyd-warshall.out` */

int main(void) {
    MatGraph *mg = mg_create(5);

    mg_add_edge(mg, 1, 2, 2);
    mg_add_edge(mg, 1, 3, 3);
    mg_add_edge(mg, 1, 4, 1);
    mg_add_edge(mg, 1, 5, 10);
    mg_add_edge(mg, 2, 4, 2);
    mg_add_edge(mg, 3, 1, 8);
    mg_add_edge(mg, 3, 4, 1);
    mg_add_edge(mg, 3, 5, 1);
    mg_add_edge(mg, 4, 5, 3);
    mg_add_edge(mg, 5, 1, 7);
    mg_add_edge(mg, 5, 2, 4);

    mg_floyd_warshall(mg);

    for (int i = 1; i <= mg->size; i++) {
        for (int j = 1; j <= mg->size; j++)
            printf("%3d ", mg->distance[i][j]);

        printf("\n");
    }

    mg_release(mg);
    
    return 0;
}