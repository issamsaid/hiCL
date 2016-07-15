#ifndef __API_RBT_INL_H_
#define __API_RBT_INL_H_
///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
/// All rights reserved.
///
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
/// funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permetted provided that the following conditions
/// are met:
///
/// 1. Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright
///    notice, this list of conditions and the following disclaimer in the
///    documentation and/or other materials provided with the distribution.
/// 3. Neither the name of the UPMC nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
/// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
/// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
/// LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// @file __api/rbt-inl.h
/// @author Issam SAID
/// @brief X-Macros for generic red block tree data structures.
/// @details A red black tree has 4 invariants:
///   1. A node is either red or black.
///   2. The root is always black. 
///      This rule is sometimes omitted. Since the root can 
///      always be changed from red to black, but not necessarily 
///      vice-versa, this rule has little effect on analysis.
///   3. Every red node must have two black child nodes.
///   4. Every path from a given node to any of its descendant 
///      leaves contains the same number of black nodes.
///
#include "__api/config/guard.h"
#include "__api/config/private.h"
#include <stdbool.h>
#include <assert.h>

CPPGUARD_BEGIN()

#define BLACK              0
#define RED                1

#define RB_SUCCESS         0
#define RB_FAILURE        -1
#define RB_OUT_OF_MEMORY  -2
#define RB_DUPLICATE_KEY  -3
#define RB_NODE_NOT_FOUND -4

///
/// declaration
///
#define DEFINE_RBT_BUILTINS(kT, vT)                             \
    typedef  int (*rbt_kcmp_##kT##_##vT)(kT src, kT dst);       \
    typedef void (*rbt_vdst_##kT##_##vT)(vT value);             \
    typedef void (*rbt_kwlk_##kT##_##vT)(kT key, int color);    \
    typedef void (*rbt_vwlk_##kT##_##vT)(vT value);             \
    typedef void (*rbt_kvwlk_##kT##_##vT)(kT key, vT value);

#define DEFINE_RBN(kT, vT)                      \
    typedef struct _rbn_##kT##_##vT {           \
        struct _rbn_##kT##_##vT *left;          \
        struct _rbn_##kT##_##vT *right;         \
        struct _rbn_##kT##_##vT *parent;        \
        int color;                              \
        kT key;                                 \
        vT value;                               \
    } rbn_##kT##_##vT

#define DEFINE_RBT(kT, vT)                      \
    typedef struct _rbt_##kT##_##vT {           \
        rbn_##kT##_##vT *root;                  \
        rbn_##kT##_##vT *nil;                   \
        rbt_vdst_##kT##_##vT fn_vdst;           \
        rbt_kcmp_##kT##_##vT fn_kcmp;           \
        size_t size;                            \
    } rbt_##kT##_##vT

#define DEFINE_CREATE_RBT(kT, vT)                               \
    int create_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,            \
                               rbt_kcmp_##kT##_##vT fn_kcmp,    \
                               rbt_vdst_##kT##_##vT fn_vdst )

#define DEFINE_DELETE_RBT(kT, vT)                       \
    int delete_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt)

#define DEFINE_RESET_RBT(kT, vT)                    \
    int reset_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt)

#define DEFINE_WALK_RBT(kT, vT)                                         \
    void walk_key_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,                 \
                                  rbt_kwlk_##kT##_##vT fn_kwlk);        \
    void walk_value_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,               \
                                    rbt_vwlk_##kT##_##vT fn_vwlk);      \
    void walk_key_value_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,           \
                                        rbt_kvwlk_##kT##_##vT fn_kvwlk);

#define DEFINE_EMPTY_RBT(kT, vT)                        \
    bool empty_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt)

#define DEFINE_MAX_RBT(kT, vT)                                  \
    rbn_##kT##_##vT *max_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,  \
                                         rbn_##kT##_##vT *n)

#define DEFINE_MIN_RBT(kT, vT)                                  \
    rbn_##kT##_##vT *min_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,  \
                                         rbn_##kT##_##vT *n)

#define DEFINE_CHK_RBT(kT, vT)                                  \
    void chk_invariants_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt)

#define DEFINE_INSERT_RBN(kT, vT)                                       \
    int insert_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, kT key, vT value)

#define DEFINE_FIND_RBN(kT, vT)                                         \
    rbn_##kT##_##vT *find_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, kT key)

#define DEFINE_SKIP_RBN(kT, vT)                                         \
    rbn_##kT##_##vT *skip_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, kT key)

#define DEFINE_REMOVE_RBN(kT, vT)                               \
    int remove_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, kT key)

#define DEFINE_SUCC_RBN(kT, vT)                                 \
    rbn_##kT##_##vT *succ_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, \
                                          rbn_##kT##_##vT *n)

#define DEFINE_PREV_RBN(kT, vT)                                 \
    rbn_##kT##_##vT *prev_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, \
                                          rbn_##kT##_##vT *n)

#define GENERATE_RBT_HEADER(kT, vT)             \
    DEFINE_RBN(kT, vT);                         \
    DEFINE_RBT_BUILTINS(kT, vT);                \
    DEFINE_RBT(kT, vT);                         \
    DEFINE_CREATE_RBT(kT, vT);                  \
    DEFINE_DELETE_RBT(kT, vT);                  \
    DEFINE_RESET_RBT(kT, vT);                   \
    DEFINE_WALK_RBT(kT, vT);                    \
    DEFINE_MAX_RBT(kT, vT);                     \
    DEFINE_MIN_RBT(kT, vT);                     \
    DEFINE_EMPTY_RBT(kT, vT);                   \
    DEFINE_CHK_RBT(kT, vT);                     \
    DEFINE_INSERT_RBN(kT, vT);                  \
    DEFINE_FIND_RBN(kT, vT);                    \
    DEFINE_SKIP_RBN(kT, vT);                    \
    DEFINE_REMOVE_RBN(kT, vT);                  \
    DEFINE_SUCC_RBN(kT, vT);                    \
    DEFINE_PREV_RBN(kT, vT)

///
/// implemetation
///
#define IMPLEMENT_CREATE_RBT(kT, vT)                                    \
    int create_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,                    \
                               rbt_kcmp_##kT##_##vT fn_kcmp,            \
                               rbt_vdst_##kT##_##vT fn_vdst) {          \
        rbt->nil = (rbn_##kT##_##vT*)malloc(sizeof(rbn_##kT##_##vT));   \
        if (rbt->nil == NULL) return RB_OUT_OF_MEMORY;                  \
        rbt->nil->left   = rbt->nil;                                    \
        rbt->nil->right  = rbt->nil;                                    \
        rbt->nil->parent = NULL;                                        \
        rbt->nil->color  = BLACK;                                       \
        rbt->fn_kcmp     = fn_kcmp;                                     \
        rbt->fn_vdst     = fn_vdst;                                     \
        rbt->root        = rbt->nil;                                    \
        rbt->size        = 0;                                           \
        return RB_SUCCESS;                                              \
    }                                                            

#define IMPLEMENT_DELETE_RBT(kT, vT)                            \
    int delete_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt) {          \
        rbn_##kT##_##vT *n = rbt->root;                         \
        while (n != rbt->nil) {                                 \
            if (n->left != rbt->nil) {                          \
                n = n->left;                                    \
            } else if(n->right != rbt->nil) {                   \
                n = n->right;                                   \
            } else {                                            \
                rbt->size = rbt->size-1;                        \
                if (rbt->fn_vdst) { rbt->fn_vdst(n->value); }   \
                if (n->parent) {                                \
                    n = n->parent;                              \
                    if (n->left != rbt->nil){                   \
                        free(n->left);                          \
                        n->left = rbt->nil;                     \
                    } else if (n->right != rbt->nil) {          \
                        free(n->right );                        \
                        n->right = rbt->nil;                    \
                    }                                           \
                } else {                                        \
                    free(n);                                    \
                    n = rbt->nil;                               \
                }                                               \
            }                                                   \
        }                                                       \
        free(rbt->nil);                                         \
        return RB_SUCCESS;                                      \
    }

#define IMPLEMENT_RESET_RBT(kT, vT)                             \
    int reset_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt) {           \
        rbn_##kT##_##vT *n = rbt->root;                         \
        while (n != rbt->nil) {                                 \
            if (n->left != rbt->nil) {                          \
                n = n->left;                                    \
            } else if(n->right != rbt->nil) {                   \
                n = n->right;                                   \
            } else {                                            \
                if (n->parent) {                                \
                    n = n->parent;                              \
                    if (n->left != rbt->nil){                   \
                        free(n->left);                          \
                        n->left = rbt->nil;                     \
                    } else if (n->right != rbt->nil) {          \
                        free(n->right );                        \
                        n->right = rbt->nil;                    \
                    }                                           \
                } else {                                        \
                    free(n);                                    \
                    n = rbt->nil;                               \
                }                                               \
            }                                                   \
        }                                                       \
        free(rbt->nil);                                         \
        rbt->size = 0;                                          \
        return RB_SUCCESS;                                      \
    }

#define IMPLEMENT_LEFT_ROTATE_RBT(kT, vT)                           \
    PRIVATE void left_rotate_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,  \
                                             rbn_##kT##_##vT *n) {  \
        rbn_##kT##_##vT* y;                                         \
        y = n->right;                                               \
        n->right = y->left;                                         \
        if (y->left != rbt->nil)                                    \
            y->left->parent = n;                                    \
        if (y != rbt->nil)                                          \
            y->parent = n->parent;                                  \
        if (n->parent){                                             \
            if (n == n->parent->left)                               \
                n->parent->left = y;                                \
            else                                                    \
                n->parent->right = y;                               \
        } else { rbt->root = y; }                                   \
        y->left = n;                                                \
        if (n != rbt->nil) n->parent = y;                           \
    }

#define IMPLEMENT_RIGHT_ROTATE_RBT(kT, vT)                          \
    PRIVATE void right_rotate_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt, \
                                              rbn_##kT##_##vT *n) { \
        rbn_##kT##_##vT* y = n->left;                               \
        n->left = y->right;                                         \
        if (y->right != rbt->nil)                                   \
            y->right->parent = n;                                   \
        if (y != rbt->nil)                                          \
            y->parent = n->parent;                                  \
        if (n->parent) {                                            \
            if (n == n->parent->right)                              \
                n->parent->right = y;                               \
            else                                                    \
                n->parent->left = y;                                \
        } else { rbt->root = y; }                                   \
        y->right = n;                                               \
        if (n != rbt->nil) n->parent = y;                           \
    }

///
/// @def IMPLEMENT_FIXINS_RBT
/// @brief statisfy the invariants of the red black tree.
/// @detail 
/// 
/// CASE 1 uncle and parent are RED:
///    - switch the color of the parent to BLACK
///    - switch the color of the uncle to BLACK
///    - switch the color of the grand parent to RED
///    - if root is RED switch its color to BLACK
///
/// CASE 2 parent is RED, uncle is BLACK and:
///    2.1: parent is left of grand parent and child is left of parent
///       - right rotate around the grand parent
///       - switch the color of the grand parent to RED
///       - switch the color of the parent to BLACK
///    2.2: parent is right of grand parent and child is right of parent
///       - left rotate around the grand parent
///       - switch the color of the grand parent to RED
///       - switch the color of the parent to BLACK
///
/// CASE 3 parent is RED, uncle is BLACK and child, 
///        parent, and grandpa arenot aligned:
///       - do rotation to bring it to CASE 2
///       - process CASE 2
///
#define IMPLEMENT_FIXINS_RBT(kT, vT)                                    \
    PRIVATE void fixins_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,           \
                                        rbn_##kT##_##vT *n) {           \
        rbn_##kT##_##vT *uncle;                                         \
        rbn_##kT##_##vT *child = n;                                     \
        while (child != rbt->root && child->parent->color == RED) {     \
            if (child->parent == child->parent->parent->left) {         \
                /* symetric to the else case */                         \
                uncle = child->parent->parent->right;                   \
                if (uncle->color == RED) {                              \
                    /* CASE 1 */                                        \
                    child->parent->color         = BLACK;               \
                    uncle->color                 = BLACK;               \
                    child->parent->parent->color = RED;                 \
                    child                        = child->parent->parent; \
                } else {                                                \
                    /* CASE 3 */                                        \
                    if (child == child->parent->right){                 \
                        child  = child->parent;                         \
                        left_rotate_rbt_##kT##_##vT(rbt, child);        \
                    }                                                   \
                    /* CASE 2 */                                        \
                    child->parent->color         = BLACK;               \
                    child->parent->parent->color = RED;                 \
                    right_rotate_rbt_##kT##_##vT(rbt,                   \
                                                 child->parent->parent); \
                }                                                       \
            } else {                                                    \
                uncle = child->parent->parent->left;                    \
                if (uncle->color == RED) {                              \
                    /* CASE 1 */                                        \
                    child->parent->color         = BLACK;               \
                    uncle->color                 = BLACK;               \
                    child->parent->parent->color = RED;                 \
                    child                        = child->parent->parent; \
                } else {                                                \
                    /* CASE 3 */                                        \
                    if (child == child->parent->left) {                 \
                        child  = child->parent;                         \
                        right_rotate_rbt_##kT##_##vT(rbt, child);       \
                    }                                                   \
                    /* CASE 2 */                                        \
                    child->parent->color         = BLACK;               \
                    child->parent->parent->color = RED;                 \
                    left_rotate_rbt_##kT##_##vT(rbt,                    \
                                                child->parent->parent); \
                }                                                       \
            }                                                           \
        }                                                               \
        rbt->root->color = BLACK;                                       \
    }

#define IMPLEMENT_FIXRMV_RBT(kT, vT)                                    \
    PRIVATE void fixrmv_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,           \
                                        rbn_##kT##_##vT *n) {           \
        while (n != rbt->root && n->color == BLACK) {                   \
            if (n == n->parent->left) {                                 \
                rbn_##kT##_##vT* w = n->parent->right;                  \
                if (w->color == RED) {                                  \
                    w->color         = BLACK;                           \
                    n->parent->color = RED;                             \
                    left_rotate_rbt_##kT##_##vT(rbt, n->parent);        \
                    w = n->parent->right;                               \
                }                                                       \
                if (w->left->color == BLACK && w->right->color == BLACK) { \
                    w->color = RED;                                     \
                    n = n->parent;                                      \
                } else {                                                \
                    if (w->right->color == BLACK)  {                    \
                        w->left->color = BLACK;                         \
                        w->color       = RED;                           \
                        right_rotate_rbt_##kT##_##vT(rbt, w);           \
                        w = n->parent->right;                           \
                    }                                                   \
                    w->color = n->parent->color;                        \
                    n->parent->color = BLACK;                           \
                    w->right->color = BLACK;                            \
                    left_rotate_rbt_##kT##_##vT(rbt, n->parent);        \
                    n = rbt->root;                                      \
                }                                                       \
            } else {                                                    \
                rbn_##kT##_##vT* w = n->parent->left;                   \
                if (w->color == RED) {                                  \
                    w->color = BLACK;                                   \
                    n->parent->color = RED;                             \
                    right_rotate_rbt_##kT##_##vT(rbt, n->parent);       \
                    w = n->parent->left;                                \
                }                                                       \
                if (w->right->color == BLACK && w->left->color == BLACK) { \
                    w->color = RED;                                     \
                    n = n->parent;                                      \
                } else {                                                \
                    if (w->left->color == BLACK) {                      \
                        w->right->color = BLACK;                        \
                        w->color = RED;                                 \
                        left_rotate_rbt_##kT##_##vT(rbt, w);            \
                        w = n->parent->left;                            \
                    }                                                   \
                    w->color = n->parent->color;                        \
                    n->parent->color = BLACK;                           \
                    w->left->color = BLACK;                             \
                    right_rotate_rbt_##kT##_##vT(rbt, n->parent);       \
                    n = rbt->root;                                      \
                }                                                       \
            }                                                           \
        }                                                               \
        n->color = BLACK;                                               \
    }

#define IMPLEMENT_MAX_RBT(kT, vT)                               \
    rbn_##kT##_##vT *max_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,  \
                                         rbn_##kT##_##vT *n) {  \
        rbn_##kT##_##vT *max = n;                               \
        while(max->right != rbt->nil) max = max->right;         \
        return max;                                             \
    }

#define IMPLEMENT_MIN_RBT(kT, vT)                               \
    rbn_##kT##_##vT *min_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,  \
                                         rbn_##kT##_##vT *n) {  \
        rbn_##kT##_##vT *min = n;                               \
        while(min->left != rbt->nil) min = min->left;           \
        return min;                                             \
    }

#define IMPLEMENT_EMPTY_RBT(kT, vT)                     \
    bool empty_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt) {  \
        return rbt->size == 0;                          \
    }

#define IMPLEMENT_WALK_RBT(kT, vT)                                      \
    PRIVATE void rec_walk_key_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,     \
                                              rbn_##kT##_##vT *root,    \
                                              rbt_kwlk_##kT##_##vT fn_kwlk) { \
        if (root == rbt->nil) return;                                   \
        if (fn_kwlk) fn_kwlk(root->key, root->color);                   \
        if (root->left != rbt->nil)                                     \
            rec_walk_key_rbt_##kT##_##vT(rbt, root->left, fn_kwlk);     \
        if (root->right != rbt->nil)                                    \
            rec_walk_key_rbt_##kT##_##vT(rbt, root->right, fn_kwlk);    \
    }                                                                   \
    PRIVATE void rec_walk_value_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,   \
                                                rbn_##kT##_##vT *root,  \
                                                rbt_vwlk_##kT##_##vT fn_vwlk) { \
        if (root == rbt->nil) return;                                   \
        if (fn_vwlk) fn_vwlk(root->value);                              \
        if (root->left != rbt->nil)                                     \
            rec_walk_value_rbt_##kT##_##vT(rbt, root->left, fn_vwlk);   \
        if (root->right != rbt->nil)                                    \
            rec_walk_value_rbt_##kT##_##vT(rbt, root->right, fn_vwlk);  \
    }                                                                   \
    PRIVATE void rec_walk_key_value_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt, \
                                                    rbn_##kT##_##vT *root, \
                                                    rbt_kvwlk_##kT##_##vT \
                                                    fn_kvwlk) {         \
        if (root == rbt->nil) return;                                   \
        if (fn_kvwlk) fn_kvwlk(root->key, root->value);                  \
        if (root->left != rbt->nil)                                     \
            rec_walk_key_value_rbt_##kT##_##vT(rbt, root->left, fn_kvwlk); \
        if (root->right != rbt->nil)                                    \
            rec_walk_key_value_rbt_##kT##_##vT(rbt, root->right, fn_kvwlk); \
    }                                                                   \
    void walk_key_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,                 \
                                  rbt_kwlk_##kT##_##vT fn_kwlk) {       \
        rec_walk_key_rbt_##kT##_##vT(rbt, rbt->root, fn_kwlk);          \
    }                                                                   \
    void walk_value_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,               \
                                    rbt_vwlk_##kT##_##vT fn_vwlk) {     \
        rec_walk_value_rbt_##kT##_##vT(rbt, rbt->root, fn_vwlk);        \
    }                                                                   \
    void walk_key_value_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,           \
                                        rbt_kvwlk_##kT##_##vT fn_kvwlk) { \
        rec_walk_key_value_rbt_##kT##_##vT(rbt, rbt->root, fn_kvwlk);   \
    }

#define IMPLEMENT_INSERT_RBN(kT, vT)                            \
    int insert_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt,            \
                               kT key, vT value) {              \
        int ret;                                                \
        rbn_##kT##_##vT *parent = NULL;                         \
        rbn_##kT##_##vT *i      = rbt->root;                    \
        rbn_##kT##_##vT *n    =                                 \
            (rbn_##kT##_##vT*)malloc(sizeof(rbn_##kT##_##vT));  \
        if (n == NULL) return RB_OUT_OF_MEMORY;                 \
        n->left  = rbt->nil;                                    \
        n->right = rbt->nil;                                    \
        n->color = RED;                                         \
        n->key   = key;                                         \
        n->value = value;                                       \
        while (i != rbt->nil) {                                 \
            ret = rbt->fn_kcmp(key, i->key);                    \
            if (ret == 0)  return RB_DUPLICATE_KEY;             \
            parent = i;                                         \
            i = (ret < 0) ? i->left : i->right;                 \
        }                                                       \
        n->parent = parent;                                     \
        if (parent) {                                           \
            ret = rbt->fn_kcmp(key, parent->key);               \
            if (ret < 0)                                        \
                parent->left = n;                               \
            else                                                \
                parent->right = n;                              \
        } else {                                                \
            rbt->root = n;                                      \
        }                                                       \
        fixins_rbt_##kT##_##vT(rbt, n);                         \
        rbt->size++;                                            \
        return RB_SUCCESS;                                      \
    }                                               

#define IMPLEMENT_FIND_RBN(kT, vT)                              \
    rbn_##kT##_##vT* find_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, \
                                          kT key) {             \
        int ret = 0;                                            \
        rbn_##kT##_##vT* n = rbt->root;                         \
        while (n != rbt->nil) {                                 \
            if ((ret = rbt->fn_kcmp(key, n->key)) == 0) break;  \
            else n = ret < 0 ? n->left : n->right;              \
        }                                                       \
        return n;                                               \
    }

#define IMPLEMENT_SKIP_RBN(kT, vT)                                       \
    rbn_##kT##_##vT *skip_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, kT key) {\
        if (empty_rbt_##kT##_##vT(rbt)) return NULL;                     \
        rbn_##kT##_##vT* n     = find_rbn_##kT##_##vT(rbt, key);         \
        rbn_##kT##_##vT* kid   = NULL;                                   \
        rbn_##kT##_##vT* pleaf = NULL;                                   \
        if (n == rbt->nil) return n;                                     \
        if (n->left  == rbt->nil || n->right == rbt->nil) {              \
            pleaf = n;                                                   \
        } else {                                                         \
            pleaf = n->right;                                            \
            while (pleaf->left != rbt->nil) pleaf = pleaf->left;         \
        }                                                                \
        if (pleaf->left != rbt->nil) kid = pleaf->left;                  \
        else kid = pleaf->right;                                        \
                                                                        \
        kid->parent = pleaf->parent;                                    \
        if (pleaf->parent) {                                            \
            if (pleaf == pleaf->parent->left)                           \
                pleaf->parent->left = kid;                              \
            else                                                        \
                pleaf->parent->right = kid;                             \
        } else rbt->root = kid;                                         \
        if (pleaf != n) {                                               \
            kT ktmp;                                                    \
            vT vtmp;                                                    \
            ktmp          = n->key;                                     \
            n->key        = pleaf->key;                                 \
            pleaf->key    = ktmp;                                       \
            vtmp          = n->value;                                   \
            n->value      = pleaf->value;                               \
            pleaf->value = vtmp;                                        \
        }                                                               \
                                                                        \
        if (pleaf->color == BLACK) fixrmv_rbt_##kT##_##vT(rbt, kid);    \
        rbt->size--;                                                    \
        return pleaf;                                                   \
    }

#define IMPLEMENT_REMOVE_RBN(kT, vT)                            \
    int remove_rbn_##kT##_##vT(rbt_##kT##_##vT* rbt, kT key) {  \
        if (empty_rbt_##kT##_##vT(rbt)) return RB_SUCCESS;      \
        rbn_##kT##_##vT* n  = skip_rbn_##kT##_##vT(rbt, key);   \
        if (n == rbt->nil) return RB_NODE_NOT_FOUND;            \
        if (rbt->fn_vdst) { rbt->fn_vdst(n->value); }           \
        free(n);                                                \
        return RB_SUCCESS;                                      \
    }

#define IMPLEMENT_SUCC_RBN(kT, vT)                              \
    rbn_##kT##_##vT* succ_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, \
                                          rbn_##kT##_##vT *n) { \
        rbn_##kT##_##vT *succ = NULL;                           \
        if (n->right != rbt->nil)                               \
            return min_rbt_##kT##_##vT(rbt, n->right);          \
        succ = n->parent;                                       \
        while(succ != NULL     &&                               \
              succ != rbt->nil &&                               \
              n  == succ->right) {                              \
            n  = succ;                                          \
            succ = succ->parent;                                \
        }                                                       \
        return succ;                                            \
    }

#define IMPLEMENT_PREV_RBN(kT, vT)                              \
    rbn_##kT##_##vT* prev_rbn_##kT##_##vT(rbt_##kT##_##vT *rbt, \
                                          rbn_##kT##_##vT *n) { \
        rbn_##kT##_##vT *prev = NULL;                           \
        if (n->left != rbt->nil)                                \
            return max_rbt_##kT##_##vT(rbt, n->left);           \
        prev = n->parent;                                       \
        while(prev != NULL     &&                               \
              prev != rbt->nil &&                               \
              n  == prev->left) {                               \
            n  = prev;                                          \
            prev = prev->parent;                                \
        }                                                       \
        return prev;                                            \
    }

///
/// @def IMPLEMENT_CHK_RBT
/// @brief verify the red black properties
///
#define IMPLEMENT_CHK_RBT(kT, vT)                               \
    IMPLEMENT_CHK_RBT_INVARIANT_1(kT, vT);                      \
    IMPLEMENT_CHK_RBT_INVARIANT_2(kT, vT);                      \
    IMPLEMENT_CHK_RBT_INVARIANT_3(kT, vT);                      \
    IMPLEMENT_CHK_RBT_INVARIANT_4_HELPER(kT, vT);               \
    IMPLEMENT_CHK_RBT_INVARIANT_4(kT, vT);                      \
    void chk_invariants_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt) { \
        chk_invariant_1_rbt_##kT##_##vT(rbt, rbt->root);        \
        chk_invariant_2_rbt_##kT##_##vT(rbt);                   \
        chk_invariant_3_rbt_##kT##_##vT(rbt, rbt->root);        \
        chk_invariant_4_rbt_##kT##_##vT(rbt, rbt->root);        \
    }

///
/// 1. A node is either red or black.
///
#define IMPLEMENT_CHK_RBT_INVARIANT_1(kT, vT)                   \
    void chk_invariant_1_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,  \
                                         rbn_##kT##_##vT *n) {  \
        assert(n->color == RED || n->color == BLACK);           \
        if (n == rbt->nil) return;                              \
        chk_invariant_1_rbt_##kT##_##vT(rbt, n->left);          \
        chk_invariant_1_rbt_##kT##_##vT(rbt, n->right);         \
    }

///
/// 2. The root is black.
///    This rule is sometimes omitted. Since the root can 
///    always be changedfrom red to black, but not necessarily 
///    vice-versa, this rule has little effect on analysis.
///
#define IMPLEMENT_CHK_RBT_INVARIANT_2(kT, vT)                       \
    void chk_invariant_2_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt) {    \
        assert(rbt->root->color == BLACK);                          \
    }

///
/// 3. Every red node must have two black child nodes.
///
#define IMPLEMENT_CHK_RBT_INVARIANT_3(kT, vT)                   \
    void chk_invariant_3_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,  \
                                         rbn_##kT##_##vT* n) {  \
        if (n->color == RED) {                                  \
            assert(n->left->color   == BLACK);                  \
            assert(n->right->color  == BLACK);                  \
            assert(n->parent->color == BLACK);                  \
        }                                                       \
        if (n == rbt->nil) return;                              \
        chk_invariant_3_rbt_##kT##_##vT(rbt, n->left);          \
        chk_invariant_3_rbt_##kT##_##vT(rbt, n->right);         \
    }

///
/// 4. Every path from a given node to any of its descendant 
///    leaves contains the same number of black nodes.
///
#define IMPLEMENT_CHK_RBT_INVARIANT_4(kT, vT)                       \
    void chk_invariant_4_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,      \
                                         rbn_##kT##_##vT *root) {   \
        int path_black_count = -1;                                  \
        rec_chk_invariant_4_rbt_##kT##_##vT(rbt, root, 0,           \
                                            &path_black_count);     \
        printf("tree height: %d\n", path_black_count);              \
    }

#define IMPLEMENT_CHK_RBT_INVARIANT_4_HELPER(kT, vT)                    \
    void rec_chk_invariant_4_rbt_##kT##_##vT(rbt_##kT##_##vT *rbt,      \
                                             rbn_##kT##_##vT *n,        \
                                             int black_count,           \
                                             int *path_black_count) {   \
        if (n->color == BLACK) black_count++;                           \
        if (n == rbt->nil) {                                            \
            if (*path_black_count == -1)                                \
                *path_black_count = black_count;                        \
            else                                                        \
                assert(black_count == *path_black_count);               \
            return;                                                     \
        }                                                               \
        rec_chk_invariant_4_rbt_##kT##_##vT(rbt, n->left,               \
                                            black_count,                \
                                            path_black_count);          \
        rec_chk_invariant_4_rbt_##kT##_##vT(rbt, n->right,              \
                                            black_count,                \
                                            path_black_count);          \
    }

#define GENERATE_RBT_BODY(kT, vT)               \
    IMPLEMENT_CREATE_RBT(kT, vT);               \
    IMPLEMENT_DELETE_RBT(kT, vT);               \
    IMPLEMENT_RESET_RBT(kT, vT);                \
    IMPLEMENT_LEFT_ROTATE_RBT(kT, vT);          \
    IMPLEMENT_RIGHT_ROTATE_RBT(kT, vT);         \
    IMPLEMENT_FIXINS_RBT(kT, vT);               \
    IMPLEMENT_FIXRMV_RBT(kT, vT);               \
    IMPLEMENT_WALK_RBT(kT, vT)                  \
    IMPLEMENT_MAX_RBT(kT, vT);                  \
    IMPLEMENT_MIN_RBT(kT, vT);                  \
    IMPLEMENT_EMPTY_RBT(kT, vT);                \
    IMPLEMENT_CHK_RBT(kT, vT);                  \
    IMPLEMENT_INSERT_RBN(kT, vT);               \
    IMPLEMENT_SKIP_RBN(kT, vT);                 \
    IMPLEMENT_REMOVE_RBN(kT, vT);               \
    IMPLEMENT_FIND_RBN(kT, vT);                 \
    IMPLEMENT_SUCC_RBN(kT, vT);                 \
    IMPLEMENT_PREV_RBN(kT, vT);

CPPGUARD_END();    

#endif // __API_RBT_INL_H_
