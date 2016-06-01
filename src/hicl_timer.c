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
/// \file easyhicl_timer.c
/// \version $Id$
/// \brief Implements timing utilities.
///
#include "hiCL/timer.h"
#include <sys/time.h>
#include <stdint.h>
#include "__api/config/timer.h"

static time_unit __api_timer_unit;
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

void hicl_timer_uset(time_unit unit) {
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
