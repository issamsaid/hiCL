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
/// @file rbt-inl_test.cc
/// @author Issam SAID
/// @brief Unit testing file for the generic red-black tree routines.
///
#include <hiCL/base.h>
#include <hiCL/mem.h>
#include <hiCL/dev.h>
#include <gtest/gtest.h>
#include <__api/util-inl.h>
#include <__api/mem-inl.h>
#include <__api/rbt-inl.h>

#define MEM_ALIGN 128

GENERATE_RBT_HEADER(int, int);
GENERATE_RBT_BODY(int, int);

namespace {

    class RbtTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
        }
        virtual void TearDown() { }
    };

    int  rbt_cmp(int a, int b) { return a-b; }

    void rbt_dst(int a) { printf("[RB TREE OP] destruct node '%d'\n", a); }

    void rbt_shw(int a, int color) {
        if (color == RED)
            printf(C_RED"[%d] "C_END, a);
        else
            printf("[%d] ", a);
    }

    TEST_F(RbtTest, create_delete) {
         rbt_int_int rbt;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(delete_rbt_int_int(&rbt), RB_SUCCESS);
    }

    TEST_F(RbtTest, insert) {
         rbt_int_int rbt;
         rbn_int_int *n, *l, *r;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_EQ(rbt.root, n);
         ASSERT_EQ(n->left, rbt.nil);
         ASSERT_EQ(n->right, rbt.nil);
         ASSERT_EQ(BLACK, n->color);
         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 5)) != NULL);
         ASSERT_EQ(BLACK, n->color);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n, rbt.root);
         ASSERT_EQ(n->left, l);
                 
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 5)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 2)) != NULL);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_EQ(BLACK, n->color);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(RED, r->color);
         ASSERT_EQ(n, rbt.root);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);

         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 5)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 2)) != NULL);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_EQ(BLACK, n->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(BLACK, r->color);
         ASSERT_EQ(n, rbt.root);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = l;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n->left, l);

         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt,  5)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  2)) != NULL);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt,  8)) != NULL);
         ASSERT_EQ(BLACK, n->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(BLACK, r->color);
         ASSERT_EQ(n, rbt.root);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = l;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n->left, l);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_EQ(RED, r->color);
         ASSERT_EQ(n->right, r);

         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt,  5)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  2)) != NULL);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt,  8)) != NULL);
         ASSERT_EQ(BLACK, n->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(BLACK, r->color);
         ASSERT_EQ(n, rbt.root);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = l;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n->left, l);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  7)) != NULL);
         ASSERT_EQ(RED, r->color);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt,  5)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  2)) != NULL);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt,  8)) != NULL);
         ASSERT_EQ(BLACK, n->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(RED, r->color);
         ASSERT_EQ(n, rbt.root);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = l;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n->left, l);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  7)) != NULL);
         ASSERT_EQ(BLACK, r->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = r;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  11)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n->left, l);
         
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt,  5)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  2)) != NULL);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt,  8)) != NULL);
         ASSERT_EQ(BLACK, n->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(RED, r->color);
         ASSERT_EQ(n, rbt.root);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = l;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(n->left, l);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 12)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  7)) != NULL);
         ASSERT_EQ(BLACK, r->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 11)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(RED, r->color);
         ASSERT_EQ(n->right, r);
         ASSERT_EQ(n->left, l);         
         ASSERT_EQ(rbt.size, 8);
         ASSERT_EQ(delete_rbt_int_int(&rbt), RB_SUCCESS);
    }

    TEST_F(RbtTest, skip) {
         rbt_int_int rbt;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);         
         chk_invariants_rbt_int_int(&rbt);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 8);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 2) != rbt.nil);         
         chk_invariants_rbt_int_int(&rbt);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 7);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 14) != rbt.nil);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 6);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 12) != rbt.nil);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 5);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 8) != rbt.nil);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 4);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 7) != rbt.nil);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 3);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 11) != rbt.nil);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 2);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 5) != rbt.nil);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 1);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 1) != rbt.nil);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 0);
         ASSERT_TRUE(skip_rbn_int_int(&rbt, 1) == NULL);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 0);
         ASSERT_EQ(delete_rbt_int_int(&rbt), RB_SUCCESS);
    }

    TEST_F(RbtTest, remove) {
         rbt_int_int rbt;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);         
         chk_invariants_rbt_int_int(&rbt);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 8);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 2), RB_SUCCESS);   
         chk_invariants_rbt_int_int(&rbt);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 7);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 14), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 6);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 12), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 5);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 8), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 4);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 7), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 3);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 11), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 2);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 5), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 1);
         ASSERT_EQ(remove_rbn_int_int(&rbt, 1), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt); 
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_EQ(rbt.size, 0);
         ASSERT_EQ(delete_rbt_int_int(&rbt), RB_SUCCESS);
    }
    
    TEST_F(RbtTest, right_rotate_root) {
         rbt_int_int rbt;
         rbn_int_int *n, *l, *r;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);         
         chk_invariants_rbt_int_int(&rbt);
         right_rotate_rbt_int_int(&rbt, rbt.root);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 2)) != NULL);
         ASSERT_EQ(rbt.root, n);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(n->left, l);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 5)) != NULL);
         ASSERT_EQ(n->right, r);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_EQ(n->right, r);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 12)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt,  7)) != NULL);
         ASSERT_EQ(BLACK, r->color);
         ASSERT_EQ(BLACK, l->color);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(n->right, r);
         n = r;
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 11)) != NULL);
         ASSERT_EQ(RED, l->color);
         ASSERT_EQ(RED, r->color);
         ASSERT_EQ(n->right, r);
         ASSERT_EQ(n->left, l);         
         ASSERT_EQ(delete_rbt_int_int(&rbt), RB_SUCCESS);
    }
   
    TEST_F(RbtTest, left_rotate_root) {
         rbt_int_int rbt;
         rbn_int_int *n, *l, *r;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt);
         left_rotate_rbt_int_int(&rbt, rbt.root);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_EQ(rbt.root, n);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 5)) != NULL);
         ASSERT_EQ(n->left, l);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 12)) != NULL);
         ASSERT_EQ(n->right, r);
         n = l;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 2)) != NULL);
         ASSERT_EQ(n->left, l);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 7)) != NULL);
         ASSERT_EQ(n->right, r);
         n = l;
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(n->left, l);
         
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 12)) != NULL);
         ASSERT_TRUE((l = find_rbn_int_int(&rbt, 11)) != NULL);
         ASSERT_TRUE((r = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_EQ(n->right, r);
         ASSERT_EQ(n->left, l);
         ASSERT_EQ(delete_rbt_int_int(&rbt), RB_SUCCESS);
    }
    
    TEST_F(RbtTest, mix_rotate_root) {
         rbt_int_int rbt;
         rbn_int_int *n;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         left_rotate_rbt_int_int(&rbt, rbt.root);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 8)) != NULL);
         right_rotate_rbt_int_int(&rbt, n);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         // ASSERT_TRUE((n = find_rbn_int_int(&rbt, 14)) != NULL);
         // ASSERT_EQ(rbt.root, n);
         // ASSERT_TRUE((l = find_rbn_int_int(&rbt, 8)) != NULL);
         // ASSERT_EQ(n->left, l);
         // ASSERT_TRUE((r = find_rbn_int_int(&rbt, 15)) != NULL);
         // ASSERT_EQ(n->right, r);
         // n = l;
         // ASSERT_TRUE((l = find_rbn_int_int(&rbt, 5)) != NULL);
         // ASSERT_EQ(n->left, l);
         // ASSERT_TRUE((r = find_rbn_int_int(&rbt, 11)) != NULL);
         // ASSERT_EQ(n->right, r);
         // n = l;
         // ASSERT_TRUE((l = find_rbn_int_int(&rbt, 2)) != NULL);
         // ASSERT_EQ(n->left, l);
         // ASSERT_TRUE((r = find_rbn_int_int(&rbt, 7)) != NULL);
         // ASSERT_EQ(n->right, r);
         // n = l;
         // ASSERT_TRUE((l = find_rbn_int_int(&rbt, 1)) != NULL);
         // ASSERT_EQ(n->left, l);
         ASSERT_EQ(delete_rbt_int_int(&rbt), RB_SUCCESS);
    }
 
    TEST_F(RbtTest, ordered) {
         rbt_int_int rbt;
         int i, T = 6;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         for (i=1; i<=T; ++i)
             ASSERT_EQ(insert_rbn_int_int(&rbt, i, i), RB_SUCCESS);
         walk_key_rbt_int_int(&rbt, rbt_shw); printf("\n");
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_EQ(BLACK, find_rbn_int_int(&rbt, 1)->color);
         ASSERT_EQ(BLACK, find_rbn_int_int(&rbt, 2)->color);
         ASSERT_EQ(RED, find_rbn_int_int(&rbt, 4)->color);
         ASSERT_EQ(BLACK, find_rbn_int_int(&rbt, 3)->color);
         ASSERT_EQ(BLACK, find_rbn_int_int(&rbt, 5)->color);
         ASSERT_EQ(RED, find_rbn_int_int(&rbt, 6)->color);
         ASSERT_EQ(rbt.root, find_rbn_int_int(&rbt, 2));
         ASSERT_EQ(rbt.root->left, find_rbn_int_int(&rbt, 1));
         ASSERT_EQ(rbt.root->right, find_rbn_int_int(&rbt, 4));
         delete_rbt_int_int(&rbt);
    }

    TEST_F(RbtTest, min) {
         rbt_int_int rbt;
         rbn_int_int *n;
         int i, T = 6;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         for (i=1; i<=T; ++i)
             ASSERT_EQ(insert_rbn_int_int(&rbt, i, i), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_EQ(1, min_rbt_int_int(&rbt, rbt.root)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 4)) != NULL);
         ASSERT_EQ(3, min_rbt_int_int(&rbt, n)->key);
         delete_rbt_int_int(&rbt);
    }

    TEST_F(RbtTest, max) {
         rbt_int_int rbt;
         rbn_int_int *n;
         int i, T = 6;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         for (i=1; i<=T; ++i)
             ASSERT_EQ(insert_rbn_int_int(&rbt, i, i), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_EQ(T, max_rbt_int_int(&rbt, rbt.root)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 4)) != NULL);
         ASSERT_EQ(T, max_rbt_int_int(&rbt, n)->key);
         delete_rbt_int_int(&rbt);
    }

    TEST_F(RbtTest, succ) {
         rbt_int_int rbt;
         rbn_int_int *n;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_EQ(11, succ_rbn_int_int(&rbt, n)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 11)) != NULL);
         ASSERT_EQ(12, succ_rbn_int_int(&rbt, n)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_EQ(NULL, succ_rbn_int_int(&rbt, n));
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 5)) != NULL);
         ASSERT_EQ(7, succ_rbn_int_int(&rbt, n)->key);
         delete_rbt_int_int(&rbt);
    }

    TEST_F(RbtTest, prev) {
         rbt_int_int rbt;
         rbn_int_int *n;
         ASSERT_EQ(create_rbt_int_int(&rbt, rbt_cmp, rbt_dst), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  8,  8), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  5,  5), RB_SUCCESS);         
         ASSERT_EQ(insert_rbn_int_int(&rbt,  2,  2), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  1,  1), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 14, 14), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt,  7,  7), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 11, 11), RB_SUCCESS);
         ASSERT_EQ(insert_rbn_int_int(&rbt, 12, 12), RB_SUCCESS);
         chk_invariants_rbt_int_int(&rbt);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 8)) != NULL);
         ASSERT_EQ(7, prev_rbn_int_int(&rbt, n)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 11)) != NULL);
         ASSERT_EQ(8, prev_rbn_int_int(&rbt, n)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 14)) != NULL);
         ASSERT_EQ(12, prev_rbn_int_int(&rbt, n)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 5)) != NULL);
         ASSERT_EQ(2, prev_rbn_int_int(&rbt, n)->key);
         ASSERT_TRUE((n = find_rbn_int_int(&rbt, 1)) != NULL);
         ASSERT_EQ(NULL, prev_rbn_int_int(&rbt, n));
         delete_rbt_int_int(&rbt);
    }

    TEST_F(RbtTest, memory_tree) {
         hicl_init(DEFAULT);
         hidev_t d    = hicl_dev_find(DEFAULT);
         size_t N = 64;
         rbt_address_t_himem_t rbt;
         int ret;
         unsigned int i, T=1000;
         float *h[T];
         himem_t    m[T];
         ASSERT_EQ(create_rbt_address_t_himem_t(&rbt, __api_address_cmp, NULL), 
                                               RB_SUCCESS);
         for (i=0; i<T; ++i) {
            ret = posix_memalign((void**)(&h[i]), MEM_ALIGN, N*sizeof(float));
            if (ret) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
         }
         for (i=0; i<T; ++i)
            m[i] = hicl_mem_wrap(d, h[i], N, HWA);
         for (i=0; i<T; ++i) {
            ASSERT_EQ(insert_rbn_address_t_himem_t(&rbt, (address_t)h[i], m[i]), 
                                                   RB_SUCCESS);
            ASSERT_EQ(m[i]->size*m[i]->unit_size, N*sizeof(float));
            ASSERT_TRUE(__API_MEM_HWA(m[i]->flags));
            ASSERT_TRUE(__API_MEM_FLOAT(m[i]->flags));
         }
         chk_invariants_rbt_address_t_himem_t(&rbt);
         for (i=0; i<T; ++i)
            ASSERT_EQ(find_rbn_address_t_himem_t(&rbt, h[i])->value, m[i]);
         ASSERT_EQ(delete_rbt_address_t_himem_t(&rbt), RB_SUCCESS);
         for (i=0; i<T; ++i)
            free(h[i]);
         hicl_release();
    }

}  // namespace
