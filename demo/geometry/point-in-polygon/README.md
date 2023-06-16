# 다각형 내부의 점 판정

## 삼각형 내부의 점 판정

<!--

<img src="" alt="삼각형 내부의 점 판정"><br>

-->

삼각형의 세 점 `a`, `b`, `c`가 반시계 방향으로 정렬되어 있다고 가정할 때, 임의의 점 `p`가 삼각형 내부에 있는지 확인하기 위해서는 `vector2_ccw(a, p, b) <= 0 && vector2_ccw(b, p, c) <= 0 && vector2_ccw(c, p, a) <= 0`의 값이 참인지를 확인하면 된다.

```c
inline int vector2_ccw(Vector2 v1, Vector2 v2, Vector2 v3) {
    const int lhs = (v2.y - v1.y) * (v3.x - v1.x);
    const int rhs = (v3.y - v1.y) * (v2.x - v1.x);

    // NOTE: `-1` if clockwise, `0` if collinear, `1` if counter-clockwise
    return (lhs > rhs) - (lhs < rhs);
}

int point_in_triangle(Polygon *s, Vector2 p) {
    if (s == NULL || s->n != 3) return 0;

    return (vector2_ccw(s->vertices[0], p, s->vertices[1]) <= 0)
        && (vector2_ccw(s->vertices[1], p, s->vertices[2]) <= 0)
        && (vector2_ccw(s->vertices[2], p, s->vertices[0]) <= 0);
}
```

<br />

## 볼록 다각형 내부의 점 판정

<!--

<img src="" alt="볼록 다각형 내부의 점 판정"><br>

-->

이분 탐색 알고리즘을 이용하면 볼록 다각형 내부의 점 판정을 $O(\log N)$의 평균 시간 복잡도로 끝낼 수 있다.

```c
int point_in_convex(Polygon *s, Vector2 p) {
    if (s == NULL || s->n < 3) return 0;

    const int c1 = vector2_ccw(s->vertices[0], p, s->vertices[1]);
    const int c2 = vector2_ccw(s->vertices[0], p, s->vertices[s->n - 1]);

    // 다각형의 어느 구간에도 속해있지 않는 점은 바깥에 있는 것으로 간주한다.
    if (c1 > 0 || c2 < 0) return 0;

    int low = 0, high = s->n - 1;
     
    // 이분 탐색 알고리즘으로 다각형의 특정 구간을 이루는 변을 찾는다.
    while (high - low > 1) {
        const int mid = low + (high - low) / 2;

        // 세 점이 이루는 각도가 시계 반대 방향일 경우, `p`는 이 구간에 속한다.
        if (vector2_ccw(s->vertices[0], p, s->vertices[mid]) > 0) high = mid;
        else low = mid;
    }

    const int r1 = vector2_ccw(s->vertices[0], p, s->vertices[low]);
    const int r2 = vector2_ccw(s->vertices[low], p, s->vertices[low + 1]);
    const int r3 = vector2_ccw(s->vertices[low + 1], p, s->vertices[0]);

    // 주어진 점이 다각형을 이루는 점 중에 하나인지 확인한다.
    if ((!r1 && !r2) || (!r1 && !r3) || (!r2 && !r3)) return -1;

    // 주어진 점이 다각형의 경계 위에 있는지 확인한다.
    if ((low == 1 && !r1 && (r2 * r3 >= 0))
        || (low + 1 == s->n - 1 && !r3 && (r1 * r2 >= 0))
        || (!r2 && (r1 * r3 >= 0))) return -1;

    return (r1 <= 0 && r2 <= 0 && r3 <= 0);
}
```

<br />

## 참고 자료

- [cp-algorithms.com: Check if point belongs to the convex polygon in `O(log N)`](https://cp-algorithms.com/geometry/point-in-convex-polygon.html)
- [zhu45.com: How to write binary search correctly](https://zhu45.org/posts/2018/Jan/12/how-to-write-binary-search-correctly/)

<br />