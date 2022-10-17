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

#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define BST_STACK_INIT_CAPACITY 32

/* | 자료형 선언 및 정의... | */

/* 이진 탐색 트리의 키와 값의 자료형. */
typedef int Key, Value;

/* 이진 탐색 트리의 노드를 나타내는 구조체. */
typedef struct BSTNode {
    Key key;                // 노드의 키.
    Value value;            // 노드의 값.
    size_t size;            // 노드의 크기.
    struct BSTNode *left;   // 왼쪽 자식 노드.
    struct BSTNode *right;  // 오른쪽 자식 노드.
} BSTNode;

/* 이진 탐색 트리를 나타내는 추상 자료형. */
typedef struct BST BST;

/* 이진 탐색 트리의 연산에 사용되는 비교 함수. */
typedef int (*BSTNodeCompareCb)(Key k, Key l);

/* 이진 탐색 트리의 각 노드의 탐색 시에 호출되는 함수. */
typedef void (*BSTNodeSearchCb)(const BSTNode *node);

/* | 라이브러리 함수... | */

/* 이진 탐색 트리를 생성한다. */
BST *bst_create(BSTNodeCompareCb func);

/* 이진 탐색 트리에 할당된 메모리를 해제한다. */
void bst_release(BST *bst);

/* 이진 탐색 트리에 새로운 키-값 쌍을 추가한다. */
bool bst_put(BST *bst, Key k, Value v);

/* 이진 탐색 트리에서 주어진 키에 대응하는 값을 찾는다. */
bool bst_get(BST *bst, Key k, Value *const v);

/* 이진 탐색 트리에서 주어진 키에 대응하는 키-값 쌍을 삭제한다. */
bool bst_delete(BST *bst, Key k);

/* 이진 탐색 트리의 모든 노드를 삭제한다. */
void bst_clear(BST *bst);

/* 이진 탐색 트리의 노드를 전위 순회 방식으로 탐색한다. */
void bst_dfs_preorder(BST *bst, BSTNode *node, BSTNodeSearchCb func);

/* 이진 탐색 트리의 노드를 후위 순회 방식으로 탐색한다. */
void bst_dfs_postorder(BST *bst, BSTNode *node, BSTNodeSearchCb func);

/* 이진 탐색 트리의 노드를 중위 순회 방식으로 탐색한다. */
void bst_dfs_inorder(BST *bst, BSTNode *node, BSTNodeSearchCb func);

/* 이진 탐색 트리의 단말 노드의 개수를 반환한다. */
size_t bst_leaf_count(BSTNode *node);

/* 이진 탐색 트리의 높이를 반환한다. */
size_t bst_height(BSTNode *node);

/* 이진 탐색 트리의 모든 노드의 개수를 반환한다. */
size_t bst_size(BSTNode *node);

/* 이진 탐색 트리의 루트 노드를 반환한다. */
BSTNode *bst_root(BST *bst);

#endif // `BST_H`

#ifdef BST_IMPLEMENTATION

/* | 자료형 선언 및 정의... | */

/* 이진 탐색 트리의 순회용 스택을 나타내는 구조체. */
typedef struct BSTStack {
    BSTNode **ptr;    // 스택에 저장된 항목의 배열.
    size_t length;    // 스택에 저장된 항목의 현재 개수.
    size_t capacity;  // 스택에 저장 가능한 항목의 최대 개수.
} BSTStack;

/* 이진 탐색 트리를 나타내는 구조체. */
struct BST {
    BSTNode *root;          // 이진 탐색 트리의 루트 노드.
    BSTStack *stack;        // 이진 탐색 트리의 순회용 스택.
    BSTNodeCompareCb func;  // 항목의 대소 관계 비교를 위한 함수.
};

/* | 라이브러리 함수... | */

/* 스택을 생성한다. */
static BSTStack *_bs_create(void) {
    BSTStack *s = malloc(sizeof(*s));

    s->length = 0;
    s->capacity = BST_STACK_INIT_CAPACITY;

    s->ptr = malloc(s->capacity * sizeof(*(s->ptr)));

    return s;
}

/* 스택에 할당된 메모리를 해제한다. */
static void _bs_release(BSTStack *s) {
    if (s == NULL) return;

    s->length = s->capacity = 0;

    free(s->ptr);
    free(s);
}

/* 스택에 새로운 항목을 추가한다. */
static bool _bs_push(BSTStack *s, BSTNode *i) {
    if (s == NULL) return false;

    if (s->length >= s->capacity) {
        const size_t new_capacity = 2 * s->capacity;

        BSTNode **new_ptr = realloc(s->ptr, new_capacity * sizeof(*new_ptr));

        if (new_ptr == NULL) return false;

        s->capacity = new_capacity;
        s->ptr = new_ptr;
    }

    s->ptr[s->length++] = i;

    return true;
}

/* 스택에서 가장 최근에 추가된 항목을 꺼낸다. */
static bool _bs_pop(BSTStack *s, BSTNode **const i) {
    if (s == NULL || s->length <= 0 || i == NULL) return false;

    *i = s->ptr[--s->length];

    return true;
}

/* 이진 탐색 트리의 노드를 생성한다. */
static BSTNode *_bst_node_create(Key k, Value v) {
    BSTNode *node = malloc(sizeof(*node));

    node->key = k;
    node->value = v;
    node->size = 1;

    node->left = node->right = NULL;

    return node;
}

/* 이진 탐색 트리의 노드에 할당된 메모리를 해제한다. */
static void _bst_node_release(BSTNode *node) {
    free(node);
}

/* 이진 탐색 트리를 생성한다. */
BST *bst_create(BSTNodeCompareCb func) {
    BST *bst = malloc(sizeof(*bst));

    bst->root = NULL;
    bst->func = func;

    bst->stack = _bs_create();

    return bst;
}

/* 이진 탐색 트리에 할당된 메모리를 해제한다. */
void bst_release(BST *bst) {
    if (bst == NULL) return;

    bst_clear(bst);

    _bs_release(bst->stack);

    free(bst);
}

/* (이진 탐색 트리에 새로운 키-값 쌍을 추가한다.) */
static BSTNode *_bst_put_recursive_helper(
    BSTNode *node, BSTNodeCompareCb func, 
    Key k, Value v
) {
    if (node == NULL) return _bst_node_create(k, v);

    const int direction = func(k, node->key);

    if (direction < 0)
        node->left = _bst_put_recursive_helper(node->left, func, k, v);
    else if (direction > 0)
        node->right = _bst_put_recursive_helper(node->right, func, k, v);
    else node->value = v;

    // 노드의 크기를 미리 계산한다.
    node->size = 1 + bst_size(node->left) + bst_size(node->right);

    return node;
}

/* 이진 탐색 트리에 새로운 키-값 쌍을 추가한다. */
bool bst_put(BST *bst, Key k, Value v) {
    if (bst == NULL || bst->func == NULL) return false;

#if 0
    bst->root = _bst_put_recursive_helper(bst->root, bst->func, k, v);

    return true;
#else
    BSTNode *parent = NULL, *node = bst->root;

    if (node == NULL) {
        bst->root = _bst_node_create(k, v);

        return true;
    }

    while (node != NULL) {
        // 새로운 노드를 연결하기 위해 부모 노드를 설정한다.
        parent = node;

        const int direction = bst->func(k, node->key);

        if (direction < 0)
            node = node->left;
        else if (direction > 0)
            node = node->right;
        else {
            node->value = v;

            return true;
        }
    }

    node = bst->root;

    // 새로운 노드 생성이 확정되었으므로, 노드의 크기를 다시 계산한다.
    while (node != NULL) {
        node->size++;

        const int direction = bst->func(k, node->key);
        
        node = (direction < 0) ? node->left : node->right;
    }

    // 자식 노드의 최종 위치를 결정한다.
    const int direction = bst->func(k, parent->key);

    if (direction < 0) parent->left = _bst_node_create(k, v);
    else parent->right = _bst_node_create(k, v);

    return true;
#endif
}

/* (이진 탐색 트리에서 주어진 키에 대응하는 값을 찾는다.) */
static bool _bst_get_helper(BSTNode *node, BSTNodeCompareCb func, Key k, Value *const v) {
    if (node == NULL || func == NULL || v == NULL) return false;

#if 0
    const int direction = func(k, node->key);

    if (direction < 0) {
        return _bst_get_helper(node->left, func, k, v);
    } else if (direction > 0) {
        return _bst_get_helper(node->right, func, k, v);
    } else {
        *v = node->value;

        return true;
    }
#else
    while (node != NULL) {
        const int direction = func(k, node->key);

        if (direction < 0)
            node = node->left;
        else if (direction > 0)
            node = node->right;
        else {
            *v = node->value;

            return true;
        }
    }

    return false;
#endif
}

/* 이진 탐색 트리에서 주어진 키에 대응하는 값을 찾는다. */
bool bst_get(BST *bst, Key k, Value *const v) {
    if (bst == NULL || bst->root == NULL || v == NULL) return false;

    return _bst_get_helper(bst->root, bst->func, k, v);
}

/* (이진 탐색 트리에서 주어진 키에 대응하는 키-값 쌍을 삭제한다.) */
static BSTNode *_bst_delete_helper(BSTNode *node, BSTNodeCompareCb func, Key k) {
    if (node == NULL || func == NULL) return false;

    const int direction = func(k, node->key);

    if (direction < 0) 
        node->left = _bst_delete_helper(node->left, func, k);
    else if (direction > 0) 
        node->right = _bst_delete_helper(node->right, func, k);
    else {
        if (node->right == NULL) {
            BSTNode *left = node->left;

            _bst_node_release(node);

            return left;
        } else if (node->left == NULL) {
            BSTNode *right = node->right;

            _bst_node_release(node);

            return right;
        } else {
            // 삭제할 노드를 미리 저장해놓는다.
            BSTNode *temp = node, *parent = NULL;

            // 일반적으로, 삭제할 노드의 왼쪽 서브 트리에서 가장 큰 값 또는
            // 오른쪽 서브 트리에서 가장 작은 값을 후계자 노드로 선택한다.
            node = temp->right;

            while (node->left != NULL) {
                parent = node;
                node = node->left;
            }

            // 후계자 노드의 부모가 삭제할 노드일 경우...?
            if (parent == NULL) {
                node->right = NULL;
            } else {
                parent->left = NULL;

                node->right = temp->right;
                node->right->size--;
            }

            // 후계자 노드의 왼쪽 부분을 연결한다.
            node->left = temp->left;

            // 노드를 삭제한다.
            _bst_node_release(temp);
        }
    }

    // 노드의 크기를 미리 계산한다.
    node->size = 1 + bst_size(node->left) + bst_size(node->right);

    return node;
}

/* 이진 탐색 트리에서 주어진 키에 대응하는 키-값 쌍을 삭제한다. */
bool bst_delete(BST *bst, Key k) {
    if (bst == NULL) return false;

    bst->root = _bst_delete_helper(bst->root, bst->func, k);

    return true;
}

/* 이진 탐색 트리의 모든 노드를 삭제한다. */
void bst_clear(BST *bst) {
    bst_dfs_postorder(bst, bst->root, (BSTNodeSearchCb) _bst_node_release);

    bst->root = NULL;
}

/* 이진 탐색 트리의 노드를 전위 순회 방식으로 탐색한다. */
void bst_dfs_preorder(BST *bst, BSTNode *node, BSTNodeSearchCb func) {
    if (node == NULL) return;
    
#if 0
    {
        if (func != NULL) func(node);

        bst_dfs_preorder(bst, node->left, func);
        bst_dfs_preorder(bst, node->right, func);
    }
#else
    _bs_push(bst->stack, node);

    while (bst->stack->length > 0) {
        BSTNode *i = NULL;

        _bs_pop(bst->stack, &i);

        if (func != NULL) func(i);
         
        // 오른쪽 자식 노드를 먼저 넣어야 스택의 맨 위에 
        // 왼쪽 자식 노드가 위치하게 된다.
        if (i->right != NULL) _bs_push(bst->stack, i->right);
        if (i->left != NULL) _bs_push(bst->stack, i->left);
    }

    bst->stack->length = 0;
#endif
}

/* 이진 탐색 트리의 노드를 후위 순회 방식으로 탐색한다. */
void bst_dfs_postorder(BST *bst, BSTNode *node, BSTNodeSearchCb func) {
        if (node == NULL) return;

#if 0
    {
        bst_dfs_postorder(bst, node->left, func);
        bst_dfs_postorder(bst, node->right, func);

        if (func != NULL) func(node);
    }
#else
    BSTNode *last = NULL;
    
    for (;;) {
        if (node != NULL) {
            _bs_push(bst->stack, node);

            node = node->left;
        } else {
            if (bst->stack->length <= 0) break;

            BSTNode *parent = bst->stack->ptr[bst->stack->length - 1];

            // 단말 노드가 아니면서 아직 오른쪽 자식을 확인해보지 않은 경우...?
            if (parent->right != NULL && parent->right != last) {
                node = parent->right;
            } else {
                if (func != NULL) func(parent);

                _bs_pop(bst->stack, &last);
            }
        }
    }

    bst->stack->length = 0;
#endif
}

/* 이진 탐색 트리의 노드를 중위 순회 방식으로 탐색한다. */
void bst_dfs_inorder(BST *bst, BSTNode *node, BSTNodeSearchCb func) {
#if 0
    if (node == NULL) return;

    {
        bst_dfs_inorder(bst, node->left, func);

        if (func != NULL) func(node);

        bst_dfs_inorder(bst, node->right, func);
    }
#else
    for (;;) {
        if (node != NULL) {
            _bs_push(bst->stack, node);

            node = node->left;
        } else {
            if (bst->stack->length <= 0) break;

            _bs_pop(bst->stack, &node);

            if (func != NULL) func(node);

            node = node->right;
        }
    };

    bst->stack->length = 0;
#endif
}

/* 이진 탐색 트리의 단말 노드의 개수를 반환한다. */
size_t bst_leaf_count(BSTNode *node) {
    if (node == NULL) return 0;
    else if (node->left == NULL && node->right == NULL) return 1;
    else return bst_leaf_count(node->left) + bst_leaf_count(node->right);
}

/* 이진 탐색 트리의 높이를 반환한다. */
size_t bst_height(BSTNode *node) {
    if (node == NULL) return 0;
    else if (node->left == NULL && node->right == NULL) return 1;
    else {
        const size_t left_height = bst_height(node->left);
        const size_t right_height = bst_height(node->right);

        return 1 + (left_height > right_height ? left_height : right_height);
    }
}

/* 이진 탐색 트리의 모든 노드의 개수를 반환한다. */
size_t bst_size(BSTNode *node) {
    return (node != NULL) ? node->size : 0;
}

/* 이진 탐색 트리의 루트 노드를 반환한다. */
BSTNode *bst_root(BST *bst) {
    return (bst != NULL) ? bst->root : NULL;
}

#endif // `BST_IMPLEMENTATION`