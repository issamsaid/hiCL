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
/// @file timer_test.cc
/// @author Issam SAID
/// @brief Unit testing file for the hiCL timer routines.
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
