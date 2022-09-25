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

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 자료형 선언 및 정의... | */

/* 이진 트리에 저장되는 항목의 자료형. */
typedef int Item;

/* 이진 트리의 노드를 나타내는 구조체. */
typedef struct BinaryNode {
    struct BinaryNode *left;   // 이 노드의 왼쪽 노드.
    struct BinaryNode *right;  // 이 노드의 오른쪽 노드.
    Item value;                // 노드에 저장된 값.
} BinaryNode;

/* 이진 트리를 나타내는 추상 자료형. */
typedef struct BinaryTree BinaryTree;

/* 이진 트리의 각 노드의 탐색 시에 호출되는 함수. */
typedef void (*BinaryTreeSearchCb)(const BinaryNode *node);

/* | 라이브러리 함수... | */

/* 이진 트리를 생성한다. */
BinaryTree *bt_create(Item i);

/* 이진 트리에 할당된 메모리를 해제한다. */
void bt_release(BinaryTree *bt);

/* 이진 트리의 노드에 왼쪽 자식 노드를 추가한다. */
BinaryNode *bt_insert_left(BinaryNode *node, Item i);

/* 이진 트리의 노드에 오른쪽 자식 노드를 추가한다. */
BinaryNode *bt_insert_right(BinaryNode *node, Item i);

/* 이진 트리의 노드를 전위 순회 방식으로 탐색한다. */
void bt_dfs_preorder(const BinaryNode *node, BinaryTreeSearchCb func);

/* 이진 트리의 노드를 후위 순회 방식으로 탐색한다. */
void bt_dfs_postorder(const BinaryNode *node, BinaryTreeSearchCb func);

/* 이진 트리의 노드를 중위 순회 방식으로 탐색한다. */
void bt_dfs_inorder(const BinaryNode *node, BinaryTreeSearchCb func);

/* 이진 트리의 루트 노드를 반환한다. */
BinaryNode *bt_root(BinaryTree *bt);

/* 이진 트리의 노드가 단말 노드인지 확인한다. */
bool bt_is_leaf(const BinaryNode *node);

#endif // `BINARY_TREE_H`

#ifdef BINARY_TREE_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 이진 트리를 나타내는 구조체. */
struct BinaryTree {
    BinaryNode *root;  // 이진 트리의 루트 노드.
};

/* | 라이브러리 함수... | */

/* 이진 트리의 노드를 생성한다. */
static BinaryNode *_bt_node_create(Item i) {
    BinaryNode *node = malloc(sizeof(*node));

    node->left = node->right = NULL;
    node->value = i;

    return node;
}

/* 이진 트리의 노드에 할당된 메모리를 해제한다. */
static void _bt_node_release(BinaryNode *node) {
    if (node == NULL) return;

    free(node);
}

/* 이진 트리를 생성한다. */
BinaryTree *bt_create(Item i) {
    BinaryTree *bt = malloc(sizeof(*bt));

    bt->root = _bt_node_create(i);

    return bt;
}

/* 이진 트리에 할당된 메모리를 해제한다. */
void bt_release(BinaryTree *bt) {
    if (bt == NULL) return;

    bt_dfs_postorder(bt_root(bt), (BinaryTreeSearchCb) _bt_node_release);

    free(bt);
}

/* 이진 트리의 노드에 왼쪽 자식 노드를 추가한다. */
BinaryNode *bt_insert_left(BinaryNode *node, Item i) {
    if (node == NULL) return NULL;

    node->left = _bt_node_create(i);

    return node->left;
}

/* 이진 트리의 노드에 오른쪽 자식 노드를 추가한다. */
BinaryNode *bt_insert_right(BinaryNode *node, Item i) {
    if (node == NULL) return NULL;

    node->right = _bt_node_create(i);

    return node->right;
}

/* 이진 트리의 노드를 전위 순회 방식으로 탐색한다. */
void bt_dfs_preorder(const BinaryNode *node, BinaryTreeSearchCb func) {
#if 1
    if (node == NULL) return;

    {
        if (func != NULL) func(node);

        bt_dfs_preorder(node->left, func);
        bt_dfs_preorder(node->right, func);
    }
#else
    /* TODO: ... */
#endif
}

/* 이진 트리의 노드를 후위 순회 방식으로 탐색한다. */
void bt_dfs_postorder(const BinaryNode *node, BinaryTreeSearchCb func) {
#if 1
    if (node == NULL) return;

    {
        bt_dfs_postorder(node->left, func);
        bt_dfs_postorder(node->right, func);

        if (func != NULL) func(node);
    }
#else
    /* TODO: ... */
#endif
}

/* 이진 트리의 노드를 중위 순회 방식으로 탐색한다. */
void bt_dfs_inorder(const BinaryNode *node, BinaryTreeSearchCb func) {
#if 1
    if (node == NULL) return;

    {
        bt_dfs_inorder(node->left, func);

        if (func != NULL) func(node);

        bt_dfs_inorder(node->right, func);
    }
#else
    /* TODO: ... */
#endif
}

/* 이진 트리의 루트 노드를 반환한다. */
BinaryNode *bt_root(BinaryTree *bt) {
    return bt->root;
}

/* 이진 트리의 노드가 단말 노드인지 확인한다. */
bool bt_is_leaf(const BinaryNode *node) {
    return (node != NULL) && (node->left == NULL && node->right == NULL);
}

#endif // `BINARY_TREE_IMPLEMENTATION`