///
/// @copyright Copyright (c) 2013-2016, Université Pierre et Marie Curie
/// All rights reserved.
/// 
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie, 
/// founded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
/// 
/// Redistribution and use in source and binary forms, with or without 
/// modification,
/// are permitted provided that the following conditions are met:
/// 
/// 1. Redistributions of source code must retain the above copyright notice, 
///    this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
/// 3. Neither the name of the UPMC nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
/// 
/// THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
/// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// 
/// @file base_test.cc
/// @author Issam SAID
/// @brief The unit testing file of the hiCL setup main routines.
///
#include "hiCL/flags.h"
#include "hiCL/base.h"
#include "hiCL/dev.h"
#include "__api/plt-inl.h"
#include "__api/dev-inl.h"
#include "gtest/gtest.h"

extern hienv_t hicl;

namespace {

    class BaseTest : public ::testing::Test {
    protected:
        virtual void SetUp() { }
        virtual void TearDown() { }
    };

    TEST_F(BaseTest, init_default) {
        ASSERT_TRUE(hicl == NULL);
        hicl_init(DEFAULT);
        ASSERT_TRUE(hicl != NULL);
        hicl_release();
        ASSERT_TRUE(hicl == NULL);
    }

    TEST_F(BaseTest, init_all) {
        unsigned int n;
        ASSERT_TRUE(hicl == NULL);
        hicl_init(ALL);
        n = __api_dev_count(hicl->platform_id);
        ASSERT_TRUE(hicl != NULL);
        hicl_release();
        ASSERT_GE(n, 0);
    }

    TEST_F(BaseTest, init_by_vendor) {
        ASSERT_TRUE(hicl == NULL);
        unsigned int f;
        char vendor[__API_STR_SIZE];
        cl_platform_id plt_ids[8];
        unsigned int nb_plts = __api_plt_count();
        cl_platform_id id;
        flags_t plt_vendors[] = {AMD, APPLE, INTEL, NVIDIA};
         __api_plt_query(plt_ids, nb_plts);
        id = __api_plt_select(plt_ids, nb_plts, DEFAULT);
        __API_PLT_GET_PTR(id, CL_PLATFORM_VENDOR, vendor);
        __api_strupper(vendor);
        for (f = 0; f < sizeof(plt_vendors)/sizeof(*plt_vendors); ++f) {
            if (strstr(vendor, __API_PLT_TYPE_STR(plt_vendors[f]))) {
                hicl_init(f);
                ASSERT_TRUE(hicl != NULL);
            }
        } 
        hicl_release();
        ASSERT_TRUE(hicl == NULL);
    }

    TEST_F(BaseTest, info) {
        hicl_init(DEFAULT);
        hicl_info();
        hicl_release();
    }

    TEST_F(BaseTest, load) {
        hicl_init(DEFAULT);
        hicl_load("data/foo.cl", "-DSTENCIL=29");
        hicl_load("data/bar.cl", "-DSTENCIL=29");
        ASSERT_EQ(list_size_hiknl_t(&hicl->knls), static_cast<unsigned int>(6));
        hicl_release();
    }

}  // namespace
