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
/// @file dev-inl_test.cc
/// @author Issam SAID
/// @brief Unit testing file for the devices manipulation private functions.
///
#include "hiCL/flags.h"
#include "__api/plt-inl.h"
#include "__api/dev-inl.h"
#include "gtest/gtest.h"

namespace {

    class DevInlTest : public ::testing::Test {
    protected:
        unsigned int nb_plts;
        unsigned int nb_devs;
        cl_platform_id plt_ids[8];
        cl_device_id dev_ids[8];
        cl_device_type type;
        bool has_cpu = false, has_gpu = false, has_acc = false;
        virtual void SetUp() { 
            unsigned int i;
            clGetPlatformIDs(0, NULL, &nb_plts);
            clGetPlatformIDs(nb_plts, plt_ids, NULL);
            clGetDeviceIDs(plt_ids[0], CL_DEVICE_TYPE_ALL, 0, NULL, &nb_devs);
            clGetDeviceIDs(plt_ids[0], 
                           CL_DEVICE_TYPE_ALL, nb_devs, dev_ids, NULL);
            for (i = 0; i < nb_devs; ++i) {
                clGetDeviceInfo(dev_ids[i], 
                                CL_DEVICE_TYPE, sizeof(type), &type, NULL);
                if ( CL_DEVICE_TYPE_CPU & type) has_cpu = true;
                if ( CL_DEVICE_TYPE_GPU & type) has_gpu = true;
                if ( CL_DEVICE_TYPE_ACCELERATOR & type) has_acc = true;
            }
        }

        virtual void TearDown() {}
    };

    TEST_F(DevInlTest, check_hidev_type_flags) {
        flags_t flags = DEFAULT;
        ASSERT_TRUE(__API_DEV_CHECK_TYPE_FLAGS(flags));
        flags = ALL;
        ASSERT_TRUE(__API_DEV_CHECK_TYPE_FLAGS(flags));
        flags = CPU;
        ASSERT_TRUE(__API_DEV_CHECK_TYPE_FLAGS(flags));
        flags = GPU;
        ASSERT_TRUE(__API_DEV_CHECK_TYPE_FLAGS(flags));
        flags = ACCELERATOR;
        ASSERT_TRUE(__API_DEV_CHECK_TYPE_FLAGS(flags));
        flags = CPU | GPU;
        ASSERT_FALSE(__API_DEV_CHECK_TYPE_FLAGS(flags));
    }

    TEST_F(DevInlTest, checkhidev_t_index_flags) {
        flags_t flags = DEFAULT;
        ASSERT_TRUE(__API_DEV_CHECK_INDEX_FLAGS(flags));
        flags = FIRST;
        ASSERT_TRUE(__API_DEV_CHECK_INDEX_FLAGS(flags));
        flags = SECOND;
        ASSERT_TRUE(__API_DEV_CHECK_INDEX_FLAGS(flags));
        flags = THIRD;
        ASSERT_TRUE(__API_DEV_CHECK_INDEX_FLAGS(flags));
        flags = FOURTH;
        ASSERT_TRUE(__API_DEV_CHECK_INDEX_FLAGS(flags));
        flags = FIRST | SECOND;
        ASSERT_TRUE(__API_DEV_CHECK_INDEX_FLAGS(flags));
        flags = DEFAULT | SECOND;
        ASSERT_FALSE(__API_DEV_CHECK_INDEX_FLAGS(flags));
    }

    TEST_F(DevInlTest, __api_dev_flags_to_type) {
        ASSERT_EQ(__api_dev_flags_to_type(DEFAULT), CL_DEVICE_TYPE_DEFAULT);
        ASSERT_EQ(__api_dev_flags_to_type(CPU), CL_DEVICE_TYPE_CPU);
        ASSERT_EQ(__api_dev_flags_to_type(GPU), CL_DEVICE_TYPE_GPU);
        ASSERT_EQ(__api_dev_flags_to_type(ALL), CL_DEVICE_TYPE_ALL);
        ASSERT_EQ(__api_dev_flags_to_type(ACCELERATOR), 
                                          CL_DEVICE_TYPE_ACCELERATOR);
    }

    TEST_F(DevInlTest, __api_dev_flags_to_index) {
        ASSERT_EQ(__api_dev_flags_to_index(DEFAULT), 0);
        ASSERT_EQ(__api_dev_flags_to_index(FIRST),   0);
        ASSERT_EQ(__api_dev_flags_to_index(SECOND),  1);
        ASSERT_EQ(__api_dev_flags_to_index(THIRD),   2);
        ASSERT_EQ(__api_dev_flags_to_index(FOURTH),  3);
        ASSERT_EQ(__api_dev_flags_to_index(FIFTH),   4);
        ASSERT_EQ(__api_dev_flags_to_index(SIXTH),   5);
        ASSERT_EQ(__api_dev_flags_to_index(SEVENTH), 6);
        ASSERT_EQ(__api_dev_flags_to_index(EIGHTH),  7);
    }

    TEST_F(DevInlTest, __api_dev_count) {
        ASSERT_EQ((__api_dev_count(plt_ids[0])), nb_devs);
    }

    TEST_F(DevInlTest, __api_dev_query) {
        __api_dev_query(plt_ids[0], dev_ids, nb_devs);
        ASSERT_TRUE(dev_ids[0] != NULL);
    }

    TEST_F(DevInlTest, __api_dev_select_by_type) {
        if(has_cpu) {
            ASSERT_GE(__api_dev_select_by_type(dev_ids, nb_devs, CPU), 1);
            clGetDeviceInfo(dev_ids[0], 
                            CL_DEVICE_TYPE, sizeof(type), &type, NULL);
            ASSERT_EQ(type, CL_DEVICE_TYPE_CPU);
        }
        if(has_gpu) {
            ASSERT_GE(__api_dev_select_by_type(dev_ids, nb_devs, GPU), 1);
            clGetDeviceInfo(dev_ids[0], 
                            CL_DEVICE_TYPE, sizeof(type), &type, NULL);
            ASSERT_EQ(type, CL_DEVICE_TYPE_GPU);
        }
        if(has_acc) {
            ASSERT_GE(__api_dev_select_by_type(dev_ids, 
                                               nb_devs, ACCELERATOR), 1);
            clGetDeviceInfo(dev_ids[0], 
                            CL_DEVICE_TYPE, sizeof(type), &type, NULL);
            ASSERT_EQ(type, CL_DEVICE_TYPE_ACCELERATOR);
        }
    }

    TEST_F(DevInlTest, __api_dev_select_by_index) {
        if(nb_devs >= 1) {
            ASSERT_GE(__api_dev_select_by_index(dev_ids, nb_devs, FIRST), 1);
        }
        if(nb_devs >= 2) {
            ASSERT_GE(__api_dev_select_by_index(dev_ids, 
                                                nb_devs, FIRST | SECOND), 2);
        }
    }

    TEST_F(DevInlTest, __api_dev_select) {
        if(has_cpu) {
            ASSERT_EQ(__api_dev_select(dev_ids, nb_devs, CPU | FIRST), 1);
            clGetDeviceInfo(dev_ids[0], 
                            CL_DEVICE_TYPE, sizeof(type), &type, NULL);
            ASSERT_EQ(type, CL_DEVICE_TYPE_CPU);
        } else {
            ASSERT_EQ(__api_dev_select(dev_ids, nb_devs, CPU), 0);
        }
        if(has_gpu) {    
            ASSERT_EQ(__api_dev_select(dev_ids, nb_devs, GPU | FIRST), 1);
            clGetDeviceInfo(dev_ids[0], 
                            CL_DEVICE_TYPE, sizeof(type), &type, NULL);
            ASSERT_EQ(type, CL_DEVICE_TYPE_GPU);
        } else {
            ASSERT_EQ(__api_dev_select(dev_ids, nb_devs, GPU), 0);
        }
        if(has_acc) {
            ASSERT_EQ(__api_dev_select(dev_ids, nb_devs, ACCELERATOR), 1);
            clGetDeviceInfo(dev_ids[0], 
                            CL_DEVICE_TYPE, sizeof(type), &type, NULL);
            ASSERT_EQ(type, CL_DEVICE_TYPE_ACCELERATOR);
        } else {
            ASSERT_EQ(__api_dev_select(dev_ids, nb_devs, ACCELERATOR), 0);
        }
    }

    TEST_F(DevInlTest, __api_dev_extension_supported) {
        ASSERT_TRUE(__api_dev_extension_supported(dev_ids[0],
                    (char*)"cl_khr_global_int32_base_atomics"));
    }

    TEST_F(DevInlTest, __api_dev_available) {
        ASSERT_TRUE(__api_dev_available(dev_ids[0]));
    }

    TEST_F(DevInlTest, __api_dev_compiler_available) {
        ASSERT_TRUE(__api_dev_compiler_available(dev_ids[0]));
    }

    TEST_F(DevInlTest, __api_dev_info) {
        __api_dev_info(dev_ids[0], stdout);
    }

}  // namespace
