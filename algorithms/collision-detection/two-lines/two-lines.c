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

#define TWO_LINES_IMPLEMENTATION
#include "two-lines.h"

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/two-lines.out` */

int main(void) {
    Vec2 v;

    bool result = intersects(
        (Vec2) { 1.0, 1.0 },
        (Vec2) { 5.0, 5.0 },
        (Vec2) { 1.0, 5.0 },
        (Vec2) { 5.0, 1.0 },
        &v
    );

    if (result) printf("%d\n%f %f\n", result, v.x, v.y);
    else printf("%d\n", result);

    result = intersects(
        (Vec2) { 1.0, 1.0 },
        (Vec2) { 5.0, 5.0 },
        (Vec2) { 6.0, 10.0 },
        (Vec2) { 10.0, 6.0 },
        &v
    );

    if (result) printf("%d\n%f %f\n", result, v.x, v.y);
    else printf("%d\n", result);
    
    result = intersects(
        (Vec2) { 1.0, 1.0 },
        (Vec2) { 5.0, 5.0 },
        (Vec2) { 6.0, 6.0 },
        (Vec2) { 1.0, 5.0 },
        &v
    );

    if (result) printf("%d\n%f %f\n", result, v.x, v.y);
    else printf("%d\n", result);

    result = intersects(
        (Vec2) { 2.0, 8.0 },
        (Vec2) { 9.0, 23.0 },
        (Vec2) { 1.0, 10.0 },
        (Vec2) { 9.0, 8.0 },
        &v
    );

    if (result) printf("%d\n%.16f %.16f\n", result, v.x, v.y);
    else printf("%d\n", result);
    
    return 0;
}