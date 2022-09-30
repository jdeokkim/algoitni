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

#define BST_IMPLEMENTATION
#include "bst.h"

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/bst.out` */

static int on_bst_node_compare(Key k1, Key k2);
static void on_bst_node_search(const BSTNode *node);

int main(void) {
    BST *bst = bst_create(on_bst_node_compare);

    bst_put(bst, (Key) 50, (Value) 1);
    bst_put(bst, (Key) 20, (Value) 2);
    bst_put(bst, (Key) 70, (Value) 3);
    bst_put(bst, (Key) 10, (Value) 4);
    bst_put(bst, (Key) 30, (Value) 5);

    Value value;

    if (bst_get(bst, (Key) 70, &value))
        printf("%d\n", (int) value);

    bst_dfs_preorder(bst_root(bst), on_bst_node_search); printf("_\n");
    bst_dfs_postorder(bst_root(bst), on_bst_node_search); printf("_\n");
    bst_dfs_inorder(bst_root(bst), on_bst_node_search); printf("_\n");

    bst_release(bst);

    return 0;
}

static int on_bst_node_compare(Key k1, Key k2) {
    return (k1 > k2) - (k1 < k2);
}

static void on_bst_node_search(const BSTNode *node) {
    printf("%d (%d) -> ", (int) node->key, (int) node->value);
}