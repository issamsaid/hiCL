#ifndef __HICL_TIMER_H_
#define __HICL_TIMER_H_
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
/// @file hiCL/timer.h
/// @author Issam SAID
/// @brief Function prototypes of timing utilities.
///
/// @details This file contains a set of timing routines used by hiCL to time
/// the execution of OpenCL kernels.
///
#include "__api/config/guard.h"

CPPGUARD_BEGIN();

///
/// @brief An enumeration to describe the time units.
///
/// This enumeration defines the different timing units: nano seconds,
/// micro seconds, milli seconds and seconds. 
///
typedef enum __time_unit_t {
    NANO_SECONDS  = 1,
    MICRO_SECONDS = 1000,
    MILLI_SECONDS = 1000000,
    SECONDS       = 1000000000
} time_unit_t;

///
/// @brief Set the time unit of hiCL.
///
/// This routine helps choose the desired time unit to be used to measure
/// the execution time of OpenCL kernels.
/// @return Nothing.
///
void   hicl_timer_uset(time_unit_t unit);

///
/// @brief Return the used time unit by the timer.
///
/// This routine returns a string describing the time unit used by hiCL.
/// @return A string describing the time unit.
///
char*  hicl_timer_uget();

///
/// @brief Start counting the time.
///
/// This routine is used to launch one instance of the timer.
/// @return Nothing.
///
void   hicl_timer_tick();

///
/// @brief Stop and read the timer.
///
/// This routine stops the timer and reads the elapsed time since the user
/// has last called the hicl_timer_tick routine.
/// @return the elapsed time since the nearest tick.
///
double hicl_timer_read();

///
/// @brief Return a multiplier to convert to seconds.
///
/// This routine returns a multiplier to be used if the user wants to 
/// convert the time to seconds with respect to the timing unit chosen
/// when setting the timer up.
/// @return the multiplier.
///
double hicl_timer_coef();

CPPGUARD_END();

#endif  // __HICL_TIMER_H_
