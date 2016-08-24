///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
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
/// @file hicl_timer.c
/// @author Issam SAID
/// @brief The implementation of the hiCL timer manipulation routines.
///
#include <hiCL/timer.h>
#include <sys/time.h>
#include <stdint.h>
#include "__api/config/timer.h"

static time_unit_t __api_timer_unit;
static uint64_t __api_timer_elapsed;
static int __api_timer_index = 0;
#ifdef _WIN32
static LARGE_INTEGER __api_timer_push[__API_TIMER_MAX];
static LARGE_INTEGER __api_timer_pop;
#elif defined(__API_TIMER_USE_GETTIMEOFDAY)
static struct timeval __api_timer_push[__API_TIMER_MAX];
static struct timeval __api_timer_pop;
#elif defined(__API_TIMER_USE_CLOCK_GETTIME)
static struct timespec __api_timer_push[__API_TIMER_MAX];
static struct timespec __api_timer_pop;
#else
#error "no wall clock timer is defined."
#endif  // _WIN32

void hicl_timer_uset(time_unit_t unit) {
    __api_timer_unit = unit;
}

char* hicl_timer_uget() {
    switch (__api_timer_unit) {
    case NANO_SECONDS:
        return "ns";
    case MICRO_SECONDS:
        return "us";
    case MILLI_SECONDS:
        return "ms";
    case SECONDS:
        return "s";
    default:
        return "s";
    }
}

void hicl_timer_tick() {
    if (__api_timer_index == __API_TIMER_MAX - 1) __api_timer_index = 0;
#ifdef _WIN32
    QueryPerformanceCounter(&__api_timer_push[__api_timer_index++]);
#elif defined(__API_TIMER_USE_GETTIMEOFDAY)
    gettimeofday(&__api_timer_push[__api_timer_index++], 0);
#elif defined(__API_TIMER_USE_CLOCK_GETTIME)
    clock_gettime(CLOCK_REALTIME, &__api_timer_push[__api_timer_index++]);
#else
#error "no wall clock timer is defined."
#endif  // _WIN32
}

double hicl_timer_read() {
#ifdef _WIN32
    QueryPerformanceCounter(&__api_timer_pop);
    __api_timer_index = __api_timer_index - 1;
    __api_timer_elapsed = (__api_timer_pop.QuadPart
                            - __api_timer_push[__api_timer_index].QuadPart)
        *__api_timer_unit/freq_.QuadPart;
#elif defined(__API_TIMER_USE_GETTIMEOFDAY)
    gettimeofday(&__api_timer_pop, 0);
    __api_timer_index = __api_timer_index - 1;
    __api_timer_elapsed = __api_timer_pop.tv_sec * 1e9
        - __api_timer_push[__api_timer_index].tv_sec * 1e9
        + __api_timer_pop.tv_usec * 1e3
        - __api_timer_push[__api_timer_index].tv_usec * 1e3;
#elif defined(__API_TIMER_USE_CLOCK_GETTIME)
    clock_gettime(CLOCK_REALTIME, &__api_timer_pop);
    __api_timer_index = __api_timer_index - 1;
    __api_timer_elapsed = __api_timer_pop.tv_sec * 1e9
        - __api_timer_push[__api_timer_index].tv_sec * 1e9
        + __api_timer_pop.tv_nsec
        - __api_timer_push[__api_timer_index].tv_nsec;
#else
#error "no wall clock timer is defined."
#endif  // _WIN32
    return (double)__api_timer_elapsed/__api_timer_unit;
}

double hicl_timer_coef() {
    switch (__api_timer_unit) {
    case NANO_SECONDS:
        return 1.e-9;
    case MICRO_SECONDS:
        return 1.e-6;
    case MILLI_SECONDS:
        return 1.e-3;
    case SECONDS:
        return 1.;
    }
    return 1.;
}
