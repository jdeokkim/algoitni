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

/* | 매크로 정의... | */

#define BINARY_TREE_STACK_INIT_CAPACITY  16

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

/* 이진 트리에서 특정 값에 대응하는 노드를 반환한다. */
BinaryNode *bt_get(BinaryTree *bt, Item i);

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

/* 이진 트리의 탐색 스택을 나타내는 구조체. */
typedef struct BinaryStack {
    BinaryNode **ptr;  // 스택에 저장된 항목의 배열.
    size_t length;     // 스택에 저장된 항목의 현재 개수.
    size_t capacity;   // 스택에 저장 가능한 항목의 최대 개수.
} BinaryStack;

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

/* 이진 트리의 탐색 스택을 생성한다. */
static BinaryStack *_bs_create(void) {
    BinaryStack *s = malloc(sizeof(*s));

    s->length = 0;
    s->capacity = BINARY_TREE_STACK_INIT_CAPACITY;

    s->ptr = malloc(s->capacity * sizeof(*(s->ptr)));

    return s;
}

/* 이진 트리의 탐색 스택에 할당된 메모리를 해제한다. */
static void _bs_release(BinaryStack *s) {
    if (s == NULL) return;

    s->length = s->capacity = 0;

    free(s->ptr);
    free(s);
}

/* 이진 트리의 탐색 스택에 새로운 항목을 추가한다. */
static bool _bs_push(BinaryStack *s, BinaryNode *i) {
    if (s == NULL) return false;

    if (s->length >= s->capacity) {
        const size_t new_capacity = 2 * s->capacity;

        BinaryNode **new_ptr = realloc(s->ptr, new_capacity * sizeof(*new_ptr));

        if (new_ptr == NULL) return false;

        s->capacity = new_capacity;
        s->ptr = new_ptr;
    }

    s->ptr[s->length++] = i;

    return true;
}

/* 이진 트리의 탐색 스택에서 가장 최근에 추가된 항목을 꺼낸다. */
static BinaryNode *_bs_pop(BinaryStack *s) {
    return (s != NULL && s->length > 0) ? s->ptr[--s->length] : NULL;
}

/* 이진 트리의 노드를 전위 순회 방식으로 탐색한다. */
void bt_dfs_preorder(const BinaryNode *node, BinaryTreeSearchCb func) {
    if (node == NULL) return;
    
#if 0
    {
        if (func != NULL) func(node);

        bt_dfs_preorder(node->left, func);
        bt_dfs_preorder(node->right, func);
    }
#else
    BinaryStack *s = _bs_create();

    _bs_push(s, (BinaryNode *) node);

    while (s->length > 0) {
        BinaryNode *i = _bs_pop(s);

        if (func != NULL) func(i);
         
        // 오른쪽 자식 노드를 먼저 넣어야 스택의 맨 위에 
        // 왼쪽 자식 노드가 위치하게 된다.
        if (i->right != NULL) _bs_push(s, i->right);
        if (i->left != NULL) _bs_push(s, i->left);
    }

    _bs_release(s);
#endif
}

/* 이진 트리의 노드를 후위 순회 방식으로 탐색한다. */
void bt_dfs_postorder(const BinaryNode *node, BinaryTreeSearchCb func) {
#if 0
    if (node == NULL) return;

    {
        bt_dfs_postorder(node->left, func);
        bt_dfs_postorder(node->right, func);

        if (func != NULL) func(node);
    }
#else
    BinaryStack *s = _bs_create();

    BinaryNode *i = (BinaryNode *) node, *l = NULL;
    
    for (;;) {
        if (i != NULL) {
            _bs_push(s, i);

            i = i->left;
        } else {
            if (s->length <= 0) break;

            BinaryNode *top = s->ptr[s->length - 1];

            // 단말 노드가 아니면서 아직 오른쪽 자식을 확인해보지 않은 경우...?
            if (top->right != NULL && top->right != l) {
                i = top->right;
            } else {
                if (func != NULL) func(top);

                l = _bs_pop(s);
            }
        }
    };

    _bs_release(s);
#endif
}

/* 이진 트리의 노드를 중위 순회 방식으로 탐색한다. */
void bt_dfs_inorder(const BinaryNode *node, BinaryTreeSearchCb func) {
#if 0
    if (node == NULL) return;

    {
        bt_dfs_inorder(node->left, func);

        if (func != NULL) func(node);

        bt_dfs_inorder(node->right, func);
    }
#else
    BinaryStack *s = _bs_create();

    BinaryNode *i = (BinaryNode *) node;

    for (;;) {
        if (i != NULL) {
            _bs_push(s, i);

            i = i->left;
        } else {
            if (s->length <= 0) break;

            i = _bs_pop(s);

            if (func != NULL) func(i);

            i = i->right;
        }
    };

    _bs_release(s);
#endif
}

/* (이진 트리에서 특정 값에 대응하는 노드를 반환한다.) */
static BinaryNode *_bt_get_helper(BinaryNode *node, Item i) {
    if (node == NULL || node->value == i) return node;

    BinaryNode *result = _bt_get_helper(node->left, i);

    return (result != NULL) ? result : _bt_get_helper(node->right, i);
}

/* 이진 트리에서 특정 값에 대응하는 노드를 반환한다. */
BinaryNode *bt_get(BinaryTree *bt, Item i) {
    if (bt == NULL || bt->root == NULL) return NULL;

#if 1
    return _bt_get_helper(bt_root(bt), i);
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