#ifndef __HICL_UTIL_H_
#define __HICL_UTIL_H_
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
/// @file hiCL/util.h
/// @author Issam SAID
/// @brief Function prototypes of set of utilities used by hiCL.
/// 
/// @details this file contains a list of helpers and util functions provided 
/// by the hiCL library.
///
#include <stdbool.h>
#include <hiCL/flags.h>
#include <hiCL/types.h>
#include "__api/config/guard.h"

CPPGUARD_BEGIN();

///
/// @brief Convert a string to hiCL resources flags.
///
/// This routine takes as an input a string and converts it to hiCL flags that 
/// are used to pick and allocate OpenCL resourses. This routine is useful when
/// parsing command line argument of a program using hiCL.
/// @param str is the input string provided by the user.
/// @return the corresponding hiCL flags.
///
flags_t hicl_str2flag(const char *str);

///
/// @brief Check whether the hiCL envirenment contains a given OpenCL resource.
///
/// This routine checks whether an OpenCL device, OpenCL vendor or platform is 
/// selected by the user when initializing hiCL. The given OpenCL resource is 
/// described by a set of hiCL flags. 
/// @see hiCL/flags.h
/// @param flags are the hiCL flags to describe the desired OpenCL resource.
/// @return A boolean to confirm or not that the resource is active.
///
bool hicl_has(flags_t flags);

///
/// @brief Return the count of a class of OpenCL resources that is activated
/// by hiCL.
///
/// This routine returns mainly the number of OpenCL devices that are activated
/// by hiCL and correspond to a set of hiCL flags provided by the user.
/// @param flags are the hiCL flags to describe the desired OpenCL devices.
/// @return The number of the corresponding activated devices.
///
unsigned int hicl_count(flags_t flags);

///
/// @brief Return the number of OpenCL kernel active by hiCL.
///
/// This routine returns the number of OpenCL kernel that are allocated and in 
/// use by hiCL.
/// @return The number of OpenCL kernels.
///
unsigned int hicl_knl_count();

///
/// @brief Return the number of memory objects that are allocated and used 
/// by hiCL.
///
/// This routine returns the number of memory objects that are allocated and 
/// being used by the hiCL library.
/// @return The number of OpenCL memory objects allocated by hiCL.
///
unsigned int hicl_mem_count();

CPPGUARD_END();

#endif  // __HICL_UTIL_H_
