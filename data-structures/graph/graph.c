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

#define GRAPH_IMPLEMENTATION
#include "graph.h"

#define MAX_VERTEX_COUNT 6

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/graph.out` */

static void on_graph_node_search(int v);

int main(void) {
    Graph *g = graph_create(MAX_VERTEX_COUNT);

    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 1, 2);
    graph_add_edge(g, 0, 3);
    graph_add_edge(g, 3, 4);
    graph_add_edge(g, 2, 5);

    int *values = malloc(MAX_VERTEX_COUNT * sizeof(*values));
    
    for (int v = 0; v < graph_vertex_count(g); v++) {
        size_t count = graph_adjacent_vertices(g, v, values);

        printf("%d: ", v);

        for (int i = 0; i < count; i++)
            printf("%d, ", values[i]);

        printf("_\n");
    }

    graph_dfs(g, 0, on_graph_node_search); printf("_\n");

    free(values);

    graph_release(g);

    return 0;
}

static void on_graph_node_search(int v) {
    printf("%d -> ", v);
}