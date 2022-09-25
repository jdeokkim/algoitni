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

#define BINARY_TREE_IMPLEMENTATION
#include "binary_tree.h"

/* `valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./bin/binary_tree.out` */

void on_binary_node_search(const BinaryNode *node);

int main(void) {
    BinaryTree *bt = bt_create((Item) 10);

    {
        BinaryNode *twenty = bt_insert_left(bt_root(bt), 20);
        
        bt_insert_left(twenty, 30);
        bt_insert_right(twenty, 50);

        BinaryNode *fourty = bt_insert_right(bt_root(bt), 40);

        bt_insert_right(fourty, 70);
    }

    bt_dfs_preorder(bt_root(bt), on_binary_node_search); printf("_\n");
    bt_dfs_postorder(bt_root(bt), on_binary_node_search); printf("_\n");
    bt_dfs_inorder(bt_root(bt), on_binary_node_search); printf("_\n");

    bt_release(bt);
    
    return 0;
}

void on_binary_node_search(const BinaryNode *node) {
    printf("%d (%d) -> ", node->value, bt_is_leaf(node));
}