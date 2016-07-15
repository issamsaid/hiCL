///
/// \copyright Copyright 2013-2016 UPMC All rights reserved.
///
/// \author Issam Said
/// \file base_test.cc
/// \version $Id$
/// \brief Defines a unit test for the hiCL flags.
///
#include "hiCL/flags.h"
#include "__api/plt-inl.h"
#include "__api/dev-inl.h"
#include "gtest/gtest.h"

extern agt cl;

namespace {

    class FlagsTest : public ::testing::Test {
    protected:
        virtual void SetUp() { }
        virtual void TearDown() { }
    };

    TEST_F(FlagsTest, flags_have) {
        flags_t flags = DEFAULT;
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, DEFAULT));
        flags = AMD;
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, AMD));
        flags = APPLE;
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, APPLE));
        flags = NVIDIA;
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, NVIDIA));
        flags = INTEL;
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, INTEL));
        flags = AMD | NVIDIA | INTEL | APPLE;
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, AMD));
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, NVIDIA));
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, APPLE));
        ASSERT_TRUE(__API_FLAGS_HAVE(flags, INTEL));
    }

    TEST_F(FlagsTest, check_plt_flags) {
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

    TEST_F(FlagsTest, check_dev_type_flags) {
        ASSERT_EQ(__api_dev_flags_to_type(DEFAULT), CL_DEVICE_TYPE_DEFAULT);
        ASSERT_EQ(__api_dev_flags_to_type(CPU), CL_DEVICE_TYPE_CPU);
        ASSERT_EQ(__api_dev_flags_to_type(GPU), CL_DEVICE_TYPE_GPU);
        ASSERT_EQ(__api_dev_flags_to_type(ALL), CL_DEVICE_TYPE_ALL);
        ASSERT_EQ(__api_dev_flags_to_type(ACCELERATOR), 
                                          CL_DEVICE_TYPE_ACCELERATOR);
    }

    TEST_F(FlagsTest, check_dev_index_flags) {
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

}  // namespace
