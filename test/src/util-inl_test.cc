///
/// @copyright Copyright (c) 2013-2016, Université Pierre et Marie Curie
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
/// @file util-inl_test.cc
/// @author Issam SAID
/// @brief Unit testing file for the hiCL private utilities.
///
#include "hiCL/flags.h"
#include "__api/util-inl.h"
#include "__api/config/util.h"
#include "gtest/gtest.h"

extern hienv_t hicl;

namespace {

    class UtilTest : public ::testing::Test {
    protected:
        virtual void SetUp() { }
        virtual void TearDown() { }
    };

    TEST_F(UtilTest, __api_strstep_one_word_with_delim) {
        char tmp[__API_STR_SIZE], buffer[__API_BUFFER_SIZE] = "test_strstep;";
        __api_strstep(tmp, buffer, ";");
        ASSERT_STREQ("test_strstep", tmp);
    }

    TEST_F(UtilTest, __api_strstep_one_word_no_delim) {
         char tmp[__API_STR_SIZE], buffer[__API_BUFFER_SIZE] = "test_strstep";
        __api_strstep(tmp, buffer, ";");
        ASSERT_STREQ("test_strstep", tmp);
    }

    TEST_F(UtilTest, __api_strstep_recursive_seq_with_delim) {
        char buffer[__API_BUFFER_SIZE] = "unpack_east;"                    \
                                         "kselect;unpack_back;"            \
                                         "unpack_north;unpack_front;"      \
                                         "unpack_south;unpack_west;"       \
                                         "pack_back;pack_north;pack_east;" \
                                         "pack_front;pack_south;pack_west;";
        char tmp[__API_STR_SIZE], *marker = buffer;
        unsigned int n=0;
        while((marker = __api_strstep(tmp, marker, ";")) != NULL) n++;
        ASSERT_EQ(n, 13);
    }

    TEST_F(UtilTest, __api_strstep_recursive_seq_no_delim) {
        char buffer[__API_BUFFER_SIZE] = "unpack_east;"                    \
                                         "kselect;unpack_back;"            \
                                         "unpack_north;unpack_front;"      \
                                         "unpack_south;unpack_west;"       \
                                         "pack_back;pack_north;pack_east;" \
                                         "pack_front;pack_south;pack_west";
        char tmp[__API_STR_SIZE], *marker = buffer;
        unsigned int n=0;
        while((marker = __api_strstep(tmp, marker, ";")) != NULL) n++;
        ASSERT_EQ(n, 13-1);
    }

    TEST_F(UtilTest, __api_strupper) {
        char buffer[__API_BUFFER_SIZE] = "amd intEl nviDia";
        __api_strupper(buffer);
        ASSERT_STREQ(buffer, "AMD INTEL NVIDIA");
    }
}  // namespace
