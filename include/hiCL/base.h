#ifndef HICL_BASE_H_
#define HICL_BASE_H_
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
/// @file hiCL/base.h
/// @author Issam SAID
/// @brief Function prototypes of the main routines to setup hiCL.
/// 
/// @details This file contains the main hiCL routines to setup an OpenCL
/// environment which includes the platform, the context, the desired
/// devices and the files (with a '.cl' extention) that contain the 
/// OpenCL kernels source code.
///
#include <hiCL/types.h>
#include "__api/config/guard.h"

CPPGUARD_BEGIN()

/// 
/// @brief Initialize the OpenCL environment. 
///
/// This routine selects the OpenCL platform and the devices indicated
/// by the user, with the help of bitwise flags, and wraps them into an
/// OpenCL context. Flags are used by hiCL to select the platform vendor
/// (AMD, APPLE, INTEL or NVIDIA) or to select the device type (CPU, GPU,
/// ACCELERATOR).
/// @see flags.h for more information.
/// @param flags is the indication used to select the appropriate hardware.
/// @return Nothing.
///
void hicl_init(flags_t flags);

///
/// @brief Release the OpenCL resources and quits the hiCL library.
///
/// This routine releases the OpenCL memory objects, kernels and command
/// queues that are allocated by the user when running hiCL.
/// @return Nothing. 
///
void hicl_release();

///
/// @brief Parse a '.cl' file and to create OpenCL kernels.
///
/// This routine loads a source code from a given file (with a '.cl'
/// extention) in order to create the OpenCL kernels contained by this file.
/// @param filename is the path to the kernels file.
/// @param options_format is the OpenCL options to pass to the OpenCL compiler 
///        when compiling the kernels (can be formatted).
/// @param ... are optional arguments to concatenate to the options string. 
/// @return Nothing.
///
void hicl_load(const char *filename, const char *options_string, ...);

///
/// @brief Shows information about the used OpenCL resources.
///
/// This routine prints information about the OpenCL platform,
/// the OpenCL devices, the OpenCL kernels and the OpenCL memory
/// objects allocated by the user when running hiCL.
/// @return Nothing.
///
void hicl_info();

CPPGUARD_END()

#endif  // HICL_BASE_H_