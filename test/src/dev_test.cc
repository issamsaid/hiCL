///
/// \copyright Copyright 2012-2013 TOTAL S.A. All rights reserved.
/// This file is part of \b hicl.
///
/// \b hicl is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// \b hicl is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with \b hicl.  If not, see <http://www.gnu.org/licenses/>.
///
/// \author Issam Said
/// \file device_test.cc
/// \version $Id: device_test.cc 2013 2013-03-07 23:48:34Z isaid $
/// \brief Defines a unit test for the hicl_dev.
///
#include "hiCL/base.h"
#include "hiCL/dev.h"
#include "gtest/gtest.h"

namespace {

    class DevTest : public ::testing::Test {
    protected:
        virtual void SetUp() { hicl_init(ALL); }
        virtual void TearDown() { hicl_release(); }
    };

    TEST_F(DevTest, all) {
        dev d0   = hicl_dev_find(ALL);
        dev d1   = hicl_dev_find(ALL | FIRST);
        dev d2   = hicl_dev_find(FIRST);
        ASSERT_EQ(d0, d1);
        ASSERT_EQ(d1, d2);
        ASSERT_EQ(d0, d2);
        ASSERT_GE(hicl_count(DEVICE), (unsigned int)1);
    }

    TEST_F(DevTest, default) {
        dev d0   = hicl_dev_find(DEFAULT);
        dev d1   = hicl_dev_find(DEFAULT | FIRST);
        dev d2   = hicl_dev_find(ALL | FIRST);
        ASSERT_EQ(d0, d1);
        ASSERT_EQ(d0, d2);
        ASSERT_EQ(d2, d1);
        ASSERT_GE(hicl_count(DEVICE), (unsigned int)1);
    }

    TEST_F(DevTest, cpu) {
        dev cpu1 = hicl_dev_find(CPU);
        dev cpu2 = hicl_dev_find(CPU | FIRST);
        ASSERT_EQ(cpu1, cpu2);
        ASSERT_GE(hicl_count(DEVICE), (unsigned int)1);
    }

    TEST_F(DevTest, gpu) {
        if (hicl_has(GPU)) {
            dev gpu1 = hicl_dev_find(GPU);
            dev gpu2 = hicl_dev_find(GPU | FIRST);
            ASSERT_EQ(gpu1, gpu2);
            ASSERT_GE(hicl_count(DEVICE), (unsigned int)2);
        }
    }

    TEST_F(DevTest, accelerator) {
        if (hicl_has(ACCELERATOR)) {
            dev acc1 = hicl_dev_find(ACCELERATOR);
            dev acc2 = hicl_dev_find(ACCELERATOR | FIRST);
            ASSERT_EQ(acc1, acc2);
            ASSERT_GE(hicl_count(DEVICE), (unsigned int)2);
        }
    }

    TEST_F(DevTest, two_by_index) {
        if (hicl_count(DEVICE) > 2) {
            dev first  = hicl_dev_find(FIRST);
            dev second = hicl_dev_find(SECOND);
            ASSERT_NE(first, second);
        }
    }

    TEST_F(DevTest, extension_supported) {
        dev first  = hicl_dev_find(FIRST);
        ASSERT_FALSE(hicl_dev_extension_supported(first, "fake"));
        ASSERT_TRUE(hicl_dev_extension_supported(first,
                    "cl_khr_global_int32_base_atomics"));
    }

}  // namespace
