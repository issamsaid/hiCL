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
/// \file  timer_test.cc
/// \version $Id: timer_test.cc 2082 2013-03-27 06:33:52Z isaid $
/// \brief Unit test for the hicl_timer.
///
#include "hiCL/timer.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <stdlib.h>
#endif  // _WIN32
#include <gtest/gtest.h>

#ifdef _WIN32
#define SLEEP(s) Sleep(s*1000)
#else
#define SLEEP(s) sleep(s)
#endif

namespace {

    class TimerTest: public ::testing::Test {
    protected:
        double t;
        virtual void SetUp() {}

        virtual void TearDown() {}
    };

    TEST_F(TimerTest, read_timer_in_seconds) {
        hicl_timer_uset(SECONDS);
        hicl_timer_tick();
        SLEEP(1);
        t = hicl_timer_read();
        ASSERT_NEAR(t, 1., 1.e-2);
    }

    TEST_F(TimerTest, read_timer_in_milli_seconds) {
        hicl_timer_uset(MILLI_SECONDS);
        hicl_timer_tick();
        SLEEP(1);
        t = hicl_timer_read();
        ASSERT_NEAR(t, 1.e3, 1.e1);
    }

    TEST_F(TimerTest, read_timer_in_micro_seconds) {
        hicl_timer_uset(MICRO_SECONDS);
        hicl_timer_tick();
        SLEEP(1);
        t = hicl_timer_read();
        ASSERT_NEAR(t, 1.e6, 1.e4);
    }

    TEST_F(TimerTest, read_timer_in_nano_seconds) {
        hicl_timer_uset(NANO_SECONDS);
        hicl_timer_tick();
        SLEEP(1);
        t = hicl_timer_read();
        ASSERT_NEAR(t, 1.e9, 1.e8);
    }

    TEST_F(TimerTest, nested_timing) {
        double t1, t2, t3, t4;
        hicl_timer_uset(NANO_SECONDS);
        hicl_timer_tick();
        SLEEP(1);
        hicl_timer_tick();
        SLEEP(1);
        hicl_timer_tick();
        SLEEP(1);
        hicl_timer_tick();
        SLEEP(1);
        t4 = hicl_timer_read();
        t3 = hicl_timer_read();
        t2 = hicl_timer_read();
        t1 = hicl_timer_read();
        ASSERT_NEAR(t1, 4.e9, 1.e8);
        ASSERT_NEAR(t2, 3.e9, 1.e8);
        ASSERT_NEAR(t3, 2.e9, 1.e8);
        ASSERT_NEAR(t4, 1.e9, 1.e8);
    }

    TEST_F(TimerTest, loop_timing) {
        unsigned int i, n = 5;
        double all, t[n];
        hicl_timer_uset(SECONDS);
        hicl_timer_tick();
        for (i = 0; i < n; ++i) {
            hicl_timer_tick();
            SLEEP(1);
            t[i] = hicl_timer_read();
        }
        all = hicl_timer_read();
        for (i = 0; i < n; ++i) ASSERT_NEAR(t[i], 1., 1.e-1);
        ASSERT_NEAR(all, n*1., 1.e-1);
    }

}  // namespace
