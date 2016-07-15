///
/// \copyright Copyright 2013-2016 UPMC All rights reserved.
///
/// \author Issam Said
/// \file base_test.cc
/// \version $Id$
/// \brief Defines a unit test for the hicl_base.
///
#include "hiCL/flags.h"
#include "hiCL/base.h"
#include "hiCL/dev.h"
#include "__api/plt-inl.h"
#include "__api/dev-inl.h"
#include "gtest/gtest.h"

extern agt cl;

namespace {

    class BaseTest : public ::testing::Test {
    protected:
        virtual void SetUp() { }
        virtual void TearDown() { }
    };

    TEST_F(BaseTest, init_default) {
        ASSERT_TRUE(cl == NULL);
        hicl_init(DEFAULT);
        ASSERT_TRUE(cl != NULL);
        hicl_release();
        ASSERT_TRUE(cl == NULL);
    }

    TEST_F(BaseTest, init_all) {
        unsigned int n;
        ASSERT_TRUE(cl == NULL);
        hicl_init(ALL);
        n = __api_dev_count(cl->platform_id);
        ASSERT_TRUE(cl != NULL);
        hicl_release();
        ASSERT_GE(n, 0);
    }

    TEST_F(BaseTest, info) {
        hicl_init(DEFAULT);
        hicl_info();
        hicl_release();
    }

    TEST_F(BaseTest, load) {
        hicl_init(DEFAULT);
        //hicl_load("data/hicl_test.cl", NULL);
        hicl_release();
    }

}  // namespace
