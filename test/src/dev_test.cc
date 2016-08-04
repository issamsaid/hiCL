///
/// @copyright Copyright (c) 2013-2016, Université Pierre et Marie Curie
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
/// 3. Neither the name of the UPMC nor the names of its contributors may be
///    used to endorse or promote products derived from this software
///    without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
/// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
/// THE UPMC OR ITS CONTRIBUTERS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
/// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
/// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
/// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// 
/// @file dev_test.cc
/// @author Issam SAID
/// @brief Unit testing file for the devices manipulation private functions.
///
#include "hiCL/flags.h"
#include "hiCL/base.h"
#include "hiCL/dev.h"
#include "__api/plt-inl.h"
#include "__api/dev-inl.h"
#include "gtest/gtest.h"

extern hienv_t hicl;

namespace {

    class DevTest : public ::testing::Test {
    protected:
        unsigned int n;
        cl_device_id *dev_ids;
        bool has_cpu, has_gpu, has_acc;
        virtual void SetUp() { 
            unsigned int i;
            cl_device_type type;
            has_cpu = false; has_gpu = false; has_acc = false;
            hicl_init(ALL);
            n = __api_dev_count(hicl->platform_id); 
            dev_ids = (cl_device_id*)malloc(n*sizeof(cl_device_id));
            __api_dev_query(hicl->platform_id, dev_ids, n);
            for (i = 0; i < n; ++i) {
                __API_DEV_GET(dev_ids[i], CL_DEVICE_TYPE, type);
                if ( CL_DEVICE_TYPE_CPU & type) has_cpu = true;
                if ( CL_DEVICE_TYPE_GPU & type) has_gpu = true;
                if ( CL_DEVICE_TYPE_ACCELERATOR & type) has_acc = true;
            }
        }

        virtual void TearDown() { 
            free(dev_ids);    
            hicl_release(); 
        }
    };

    TEST_F(DevTest, hicl_dev_init) {
        hidev_t d = hicl_dev_init(dev_ids[0]);
        ASSERT_TRUE(d != NULL);
    }

    TEST_F(DevTest, hicl_dev_release) {
        hidev_t d = hicl_dev_init(dev_ids[0]);
        ASSERT_TRUE(d != NULL);
        hicl_dev_release(&d);   
        ASSERT_TRUE(d == NULL);
    }

    TEST_F(DevTest, hicl_dev_find_default) {
        hidev_t d0   = hicl_dev_find(DEFAULT);
        hidev_t d2   = hicl_dev_find(FIRST);
        ASSERT_EQ(d0, d2);
        ASSERT_GE(n, (unsigned int)1);
        ASSERT_GE(list_size_hidev_t(&hicl->devs), (unsigned int)1);
        ASSERT_GE(n, list_size_hidev_t(&hicl->devs));
    }

    TEST_F(DevTest, hicl_dev_find_all) {
        hidev_t d0   = hicl_dev_find(ALL);
        hidev_t d1   = hicl_dev_find(ALL | FIRST);
        hidev_t d2   = hicl_dev_find(FIRST);
        ASSERT_EQ(d0, d1);
        ASSERT_EQ(d1, d2);
        ASSERT_EQ(d0, d2);
    }

    TEST_F(DevTest, hicl_dev_find_cpu) {
        if (has_cpu) {
            hidev_t cpu1 = hicl_dev_find(CPU);
            hidev_t cpu2 = hicl_dev_find(CPU | FIRST);
            ASSERT_EQ(cpu1, cpu2);
            ASSERT_GE(n, (unsigned int)1);
        }
    }

    TEST_F(DevTest, hicl_dev_find_gpu) {
        if (has_gpu) {
            hidev_t gpu1 = hicl_dev_find(GPU);
            hidev_t gpu2 = hicl_dev_find(GPU | FIRST);
            ASSERT_EQ(gpu1, gpu2);
            ASSERT_GE(n, (unsigned int)1);
        }
    }

    TEST_F(DevTest, hicl_dev_find_accelerator) {
        if (has_acc) {
            hidev_t acc1 = hicl_dev_find(ACCELERATOR);
            hidev_t acc2 = hicl_dev_find(ACCELERATOR | FIRST);
            ASSERT_EQ(acc1, acc2);
            ASSERT_GE(n, (unsigned int)1);
        }
    }

    TEST_F(DevTest, hicl_dev_find_two_by_index) {
        if (n >= 2) {
            hidev_t first  = hicl_dev_find(FIRST);
            hidev_t second = hicl_dev_find(SECOND);
            ASSERT_NE(first, second);
        }
    }

    TEST_F(DevTest, hicl_dev_support) {
        hidev_t first  = hicl_dev_find(FIRST);
        ASSERT_FALSE(hicl_dev_support(first, (char*)"fake"));
        ASSERT_TRUE(hicl_dev_support(first,
                    (char*)"cl_khr_global_int32_base_atomics"));
    }

    TEST_F(DevTest, hicl_dev_info) {
        hidev_t first = hicl_dev_find(FIRST);
        hicl_dev_info(first);
    }

    TEST_F(DevTest, hicl_dev_wait) {
        hidev_t first = hicl_dev_find(FIRST);
        hicl_dev_wait(first);
    }

}  // namespace
