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

#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 2차원 공간에서의 좌표를 나타내는 구조체. */
typedef struct Point { 
    int x, y; 
} Point;

/* 미로 그래프를 나타내는 추상 자료형. */
typedef struct Maze Maze;

/* | 라이브러리 함수... | */

/* 미로 그래프를 생성한다. */
Maze *maze_create(int width, int height);

/* 미로 그래프에 할당된 메모리를 해제한다. */
void maze_release(Maze *m);

/* 미로 그래프의 방문 기록을 삭제한다. */
void maze_reset(Maze *m);

/* 미로 그래프의 주어진 칸의 값을 반환한다. */
int maze_get_cell(Maze *m, Point p);

/* 미로 그래프의 주어진 칸의 방문 여부를 반환한다. */
int maze_get_visited(Maze *m, Point p);

/* 미로 그래프의 주어진 칸의 값을 변경한다. */
void maze_set_cell(Maze *m, Point p, int value);

/* 미로 그래프를 깊이 우선 탐색한다. */
void maze_dfs(Maze *m, Point p);

/* 미로 그래프를 너비 우선 탐색한다. */
void maze_bfs(Maze *m, Point p);

#endif // `MAZE_H`

#ifdef MAZE_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 큐를 나타내는 구조체. */
typedef struct Queue {
    Point *ptr;      // 큐에 저장된 항목 배열.
    int capacity;    // 큐에 저장 가능한 원소의 최대 개수.
    int start, end;  // 큐의 시작 및 끝 인덱스.
} Queue;

/* 미로 그래프를 나타내는 구조체. */
struct Maze {
    int **cells;    // 미로의 각 칸.
    int **visited;  // 각 칸의 방문 여부.
    Queue *queue;   // 너비 우선 탐색 큐.
    int width;      // 미로의 가로 길이.
    int height;     // 미로의 세로 길이.
};

/* | 전역 변수 선언 및 정의... | */

/* 미로 그래프의 현재 칸에서 상하좌우로 움직이기 위한 위치 변화량. */
static const int DELTA_X[4] = { 0, 1, 0, -1 }, DELTA_Y[4] = { 1, 0, -1, 0 };

/* | 라이브러리 함수... | */

/* (큐를 생성한다.) */
static Queue *_queue_create(int size) {
    Queue *q = malloc(sizeof(*q));
    
    q->capacity = size + 1;
    q->start = q->end = 0;

    q->ptr = malloc(q->capacity * sizeof(*(q->ptr)));

    return q;
}

/* (큐에 할당된 메모리를 해제한다.) */
static void _queue_release(Queue *q) {
    if (q == NULL) return;

    q->capacity = q->start = q->end = 0;

    free(q->ptr), free(q);
}

/* (큐가 비어 있는지 확인한다.) */
static int _queue_is_empty(Queue *q) {
    return (q == NULL) || (q->start == q->end);
}

/* (큐의 모든 항목을 제거한다.) */
static void _queue_clear(Queue *q) {
    if (q != NULL) q->start = q->end = 0;
}

/* (큐에 새로운 항목을 추가한다.) */
static void _queue_enqueue(Queue *q, Point p) {
    if (q == NULL) return;

    q->ptr[q->end] = p;
    q->end = (q->end + 1) % q->capacity;
}

/* (큐에서 가장 먼저 추가된 항목을 꺼낸다.) */
static Point _queue_dequeue(Queue *q) {
    if (q == NULL || q->start == (q->end + 1) % q->capacity) 
        return (Point) { -1, -1 };

    Point p = q->ptr[q->start];

    q->start = (q->start + 1) % q->capacity;

    return p;
}

/* 미로 그래프를 생성한다. */
Maze *maze_create(int width, int height) {
    Maze *m = malloc(sizeof(*m));

    m->cells = malloc(height * sizeof(*(m->cells)));
    m->visited = malloc(height * sizeof(*(m->visited)));

    m->queue = _queue_create(width * height);

    for (int i = 0; i < height; i++) {
        m->cells[i] = calloc(width, sizeof(*(m->cells[i])));
        m->visited[i] = calloc(width, sizeof(*(m->visited[i])));
    }

    m->width = width, m->height = height;

    return m;
}

/* 미로 그래프에 할당된 메모리를 해제한다. */
void maze_release(Maze *m) {
    if (m == NULL) return;

    for (int i = 0; i < m->height; i++)
        free(m->visited[i]), free(m->cells[i]);

    _queue_release(m->queue);

    free(m->visited), free(m->cells), free(m);
}

/* 미로 그래프의 방문 기록을 삭제한다. */
void maze_reset(Maze *m) {
    if (m == NULL) return;

    for (int y = 0; y < m->height; y++)
        for (int x = 0; x < m->width; x++)
            m->visited[y][x] = 0; 
}

/* 미로 그래프의 주어진 칸의 값을 반환한다. */
int maze_get_cell(Maze *m, Point p) {
    if (m == NULL) return 0;

    return m->cells[p.y][p.x];
}

/* 미로 그래프의 주어진 칸의 방문 여부를 반환한다. */
int maze_get_visited(Maze *m, Point p) {
    if (m == NULL) return 0;

    return m->visited[p.y][p.x];
}

/* 미로 그래프의 주어진 칸의 값을 변경한다. */
void maze_set_cell(Maze *m, Point p, int value) {
    if (m == NULL) return;

    m->cells[p.y][p.x] = value;
}

/* (미로 그래프를 깊이 우선 탐색한다.) */
static void _maze_dfs_helper(Maze *m, Point p) {
    if (m == NULL) return;

    m->visited[p.y][p.x] = 1;

    for (int i = 0; i < 4; i++) {
        int nx = p.x + DELTA_X[i];
        int ny = p.y + DELTA_Y[i];

        if (nx < 0 || nx >= m->width || ny < 0 || ny >= m->height)
            continue;

        if (m->cells[ny][nx] && !m->visited[ny][nx])
            _maze_dfs_helper(m, (Point) { nx, ny });
    }
}

/* 미로 그래프를 깊이 우선 탐색한다. */
void maze_dfs(Maze *m, Point p) {
    if (m == NULL) return;

    maze_reset(m), _maze_dfs_helper(m, p);
}

/* 미로 그래프를 너비 우선 탐색한다. */
void maze_bfs(Maze *m, Point p) {
    if (m == NULL) return;

    m->visited[p.y][p.x] = 1;

    _queue_clear(m->queue), _queue_enqueue(m->queue, p);

    while (!_queue_is_empty(m->queue)) {
        Point q = _queue_dequeue(m->queue);

        for (int i = 0; i < 4; i++) {
            int nx = q.x + DELTA_X[i];
            int ny = q.y + DELTA_Y[i];

            if (nx < 0 || nx >= m->width || ny < 0 || ny >= m->height)
                continue;

            if (m->cells[ny][nx] && !m->visited[ny][nx]) {
                m->visited[ny][nx] = m->visited[q.y][q.x] + 1;

                _queue_enqueue(m->queue, (Point) { nx, ny });
            }
        }
    }
}

#endif // `MAZE_IMPLEMENTATION`