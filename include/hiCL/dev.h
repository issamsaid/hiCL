#ifndef HICL_DEV_H_
#define HICL_DEV_H_
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
/// @file hiCL/dev.h
/// @author Issam SAID
/// @brief Function prototypes of OpenCL devices manipulation routines.
///
/// @details This file describes the library functions used by hiCL to 
/// initialize, release and manipulate an OpenCL device descriptor.
///
#include <stdbool.h>
#include <hiCL/types.h>

CPPGUARD_BEGIN()

///
/// @brief Select a device descriptor from those initialized by hiCL.
///
/// This routine selects a hiCL device, based on bitwise flags, from a list of 
/// the OpenCL devices initially created when launching the hiCL library.
/// @param flags the bitwise flags provided by the user.
/// @return The device descriptor corresponding to the user request.
///
hidev_t  hicl_dev_find(flags_t flags);

///
/// @brief Wait until a given device finishes all its operations.
///
/// This routine is used to synchronize with the command queue related to a 
/// hiCL device descriptor.
/// @param d the hiCL device descriptor.
/// @return Nothing.
///
void hicl_dev_wait(hidev_t d);

///
/// @brief Check whether a given device supports an extension.
///
/// This routine query a given OpenCL device extensions string and checks if 
/// it contains a user provided extension.
/// @param d the hiCL device descriptor.
/// @param extension is the string describing the OpenCL extension.
/// @return A boolean to inform if the extension is supported.
///
bool hicl_dev_support(hidev_t d, char* extension);

CPPGUARD_END()

#endif  // HICL_DEV_H_
