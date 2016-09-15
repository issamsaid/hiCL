///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
/// All rights reserved.
///
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
/// funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions
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
/// @file plt-inl_test.cc
/// @author Issam SAID
/// @brief Unit testing file for OpenCL platforms manipulation private routines.
///
#include "hiCL/flags.h"
#include "__api/plt-inl.h"
#include "gtest/gtest.h"

namespace {

    class PltInlTest : public ::testing::Test {
    protected:
        cl_platform_id plt_ids[8];
        unsigned int nb_plts;
        virtual void SetUp() { }
        virtual void TearDown() { }
    };

    TEST_F(PltInlTest, check_plt_flags) {
        flags_t flags = DEFAULT;
        ASSERT_TRUE(__API_PLT_CHECK_FLAGS(flags));
        flags = AMD;
        ASSERT_TRUE(__API_PLT_CHECK_FLAGS(flags));
        flags = APPLE;
        ASSERT_TRUE(__API_PLT_CHECK_FLAGS(flags));
        flags = INTEL;
        ASSERT_TRUE(__API_PLT_CHECK_FLAGS(flags));
        flags = NVIDIA;
        ASSERT_TRUE(__API_PLT_CHECK_FLAGS(flags));
        flags = AMD | NVIDIA;
        ASSERT_FALSE(__API_PLT_CHECK_FLAGS(flags));
    }

    TEST_F(PltInlTest, __api_plt_count) {
        ASSERT_GE((nb_plts = __api_plt_count()), 1);
    }

    TEST_F(PltInlTest, __api_plt_query) {
        __api_plt_query(plt_ids, nb_plts);
        ASSERT_TRUE(plt_ids[0] != NULL);
    }

    TEST_F(PltInlTest, __api_plt_select_default) {
        cl_platform_id id = __api_plt_select(plt_ids, nb_plts, DEFAULT);
        ASSERT_EQ(id, plt_ids[0]);
    }

    TEST_F(PltInlTest, __api_plt_select_no_flags) {
        cl_platform_id id = __api_plt_select(plt_ids, nb_plts, ALL);
        ASSERT_EQ(id, plt_ids[0]);
    }

    TEST_F(PltInlTest, __api_plt_select_by_vendor) {
        unsigned int f;
        char vendor[__API_STR_SIZE];
        flags_t plt_vendors[] = {AMD, APPLE, INTEL, NVIDIA};
        cl_platform_id id = __api_plt_select(plt_ids, nb_plts, DEFAULT);
        __API_PLT_GET_PTR(id, CL_PLATFORM_VENDOR, vendor);
        __api_strupper(vendor);
        for (f = 0; f < sizeof(plt_vendors)/sizeof(*plt_vendors); ++f) {
            if (strstr(vendor, __API_PLT_TYPE_STR(plt_vendors[f]))) {
                //printf("Test select %s\n", __API_PLT_TYPE_STR(plt_vendors[f]));
                ASSERT_FALSE(__api_plt_select(plt_ids, nb_plts, f) == NULL);
            }
        } 
    }

    TEST_F(PltInlTest, __api_plt_info) {
        __api_plt_info(plt_ids[0], stdout);
    }

}  // namespace