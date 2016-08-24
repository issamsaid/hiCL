#ifndef HICL_KNL_H_
#define HICL_KNL_H_
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
/// @file hiCL/knl.h
/// @author Issam SAID
/// @brief Function prototypes of OpenCL devices manipulation routines.
///
/// @details This file describes the library functions used by hiCL to 
/// initialize, release and manipulate an OpenCL kernel descriptor.
///
#include <hiCL/types.h>
#include <stdint.h>

CPPGUARD_BEGIN();

///
/// @brief Return a kernel descriptor provided a string.
///
/// This routine performs a lookup in the list of hiCL kernel descriptors based 
/// on a string provided by the user
/// @param name is the string that identifies the kernel descriptor.
/// @return
///
hiknl_t hicl_knl_find(const char *name);

///
/// @brief Build the OpenCL kernel related to a hiCL descriptor.
///
/// This routine invoques the OpenCL compiler in order to build an OpenCL
/// kernel identified by a string.
/// @param name is the string that identifies the kernel descriptor.
/// @param options is a string that contains compilation options to pass
///        to the OpenCL compiler.
/// @return Nothing.
///
void    hicl_knl_build(const char *name, const char *options);

///
/// @brief Set an integer (32 bits) value for a specific kernel argument.
///
/// This routine sets the value of a kernel argument to an integer (32 bits)
/// of a given OpenCL kernel (identified by a hiCL kernel descriptor).
/// @param name is the string that identifies the kernel descriptor.
/// @param index is the position of the argument in the list 
/// @return Nothing.
///
void    hicl_knl_set_int32(const char *name, int index, int32_t i32);

///
/// @brief Set an integer (64 bits) value for a specific kernel argument.
///
/// This routine sets the value of a kernel argument to an integer (64 bits)
/// of a given OpenCL kernel (identified by a hiCL kernel descriptor).
/// @param name is the string that identifies the kernel descriptor.
/// @param index is the position of the argument in the list 
/// @return Nothing.
///
void    hicl_knl_set_int64(const char *name, int index, int64_t i64);

///
/// @brief Set a float value for a specific kernel argument.
///
/// This routine sets the value of a kernel argument to a float
/// of a given OpenCL kernel (identified by a hiCL kernel descriptor).
/// @param name is the string that identifies the kernel descriptor.
/// @param index is the position of the argument in the list 
/// @return Nothing.
///
void    hicl_knl_set_float(const char *name, int index, float f);

///
/// @brief Set a double value for a specific kernel argument.
///
/// This routine sets the value of a kernel argument to a double
/// of a given OpenCL kernel (identified by a hiCL kernel descriptor).
/// @param name is the string that identifies the kernel descriptor.
/// @param index is the position of the argument in the list 
/// @return Nothing.
///
void    hicl_knl_set_double(const char *name, int index, double d);

///
/// @brief Set a memory address for a specific kernel argument.
///
/// This routine sets the value of a kernel argument to a memory address
/// of a given OpenCL kernel (identified by a hiCL kernel descriptor). 
/// @param name is the string that identifies the kernel descriptor.
/// @param index is the position of the argument in the list 
/// @return Nothing.
///
void    hicl_knl_set_mem(const char *name, int index, address_t h);

///
/// @brief Set a list of arguments for a given OpenCL kernel.
///
/// This routine uses the C variadic functions to set a collection of
/// arguments for a given OpenCL kernel all at once.
/// @param name is the string that identifies the kernel descriptor.
/// @param ... are the kernel arguments.
/// @return Nothing.
///
void    hicl_knl_set_args(const char *name, ...);

///
/// @brief Set the work sizes and dimension for a given OpenCL kernel.
///
/// This routine sets the work size (the global size and the local size) and 
/// the thread blocks dimensions for a given OpenCL kernel.
/// @param name is the string that identifies the kernel descriptor.
/// @param wrk is the work dimension of the kernel.
/// @param gws is and array of dimension 'wrk' that contains the global sizes.
/// @param lws is and array of dimension 'wrk' that contains the local sizes.
/// @return Nothing.
///
void    hicl_knl_set_wrk(const char *name, 
		        		 cl_uint wrk, size_t *gws, size_t *lws);

///
/// @brief Set the offset for a given OpenCL kernel.
///
/// This routine sets the thread blocks offset for a given OpenCL kernel.
/// @param name is the string that identifies the kernel descriptor.
/// @param ofs is and array that contains the offset values of each work
///        dimension.
/// @return
///
void    hicl_knl_set_ofs(const char *name, size_t *ofs);

///
/// @brief Run (asynchronously) a given OpenCL kernel after synchronizing its 
///        related memory objects between the host and a given device.
///
/// This routine ensures the integrity, between the host and a 
/// given OpenCL device, of the memory objects being manipulated by 
/// given OpenCL kernel, and runs the latter on that same device. The
/// execution is asynchronous which is the default behavior in hiCL.
/// @param name is the string that identifies the kernel descriptor.
/// @param d is the device descriptor of the OpenCL device to run the kernel
///        on.
/// @param ... is the list of arguments to pass to the kernel.
/// @return Nothing.
///
void    hicl_knl_run(const char *name, hidev_t d, ...);

///
/// @brief Run (synchronously) a given OpenCL kernel after synchronizing its 
///        related memory objects between the host and a given device.
///
/// This routine ensures the integrity, between the host and a 
/// given OpenCL device, of the memory objects being manipulated by 
/// given OpenCL kernel, and runs the latter on that same device. Finally
/// it flushes the command queue relqted to the device.
/// @param name is the string that identifies the kernel descriptor.
/// @param d is the device descriptor of the OpenCL device to run the kernel
///        on.
/// @param ... is the list of arguments to pass to the kernel.
/// @return Nothing.
///
void    hicl_knl_sync_run(const char *name, hidev_t d, ...);

///
/// @brief Run (and time the execution) a given OpenCL kernel after 
///        synchronizing its related memory objects between the 
///        host and a given device.
///
/// This routine ensures the integrity, between the host and a 
/// given OpenCL device, of the memory objects being manipulated by 
/// given OpenCL kernel, and runs the latter on that same device while timing
/// the execution.
/// @param name is the string that identifies the kernel descriptor.
/// @param d is the device descriptor of the OpenCL device to run the kernel
///        on.
/// @param ... is the list of arguments to pass to the kernel.
/// @return Nothing.
///
double  hicl_knl_timed_run(const char *name, hidev_t d, ...);

///
/// @brief Run (synchronously) only a given kernel on a given device.
///
/// This routine simply runs (synchronously) a given OpenCL kernel on a given
/// OpenCL device.
/// @param name is the string that identifies the kernel descriptor.
/// @return Nothing.
///
void    hicl_knl_exec(const char *name, hidev_t d);

///
/// @brief Run (asynchronously) only a given kernel on a given device.
///
/// This routine simply runs (asynchronously) a given OpenCL kernel on a given
/// OpenCL device.
/// @param name is the string that identifies the kernel descriptor.
/// @return Nothing.
///
void    hicl_knl_sync_exec(const char *name, hidev_t d);

///
/// @brief
///
///
/// @param name is the string that identifies the kernel descriptor.
/// @return
///

///
/// @brief Run (and time the execution) only a given kernel on a given device.
///
/// This routine simply runs and times the execution of a given OpenCL kernel 
/// on a given OpenCL device.
/// @param name is the string that identifies the kernel descriptor.
/// @return Nothing.
///
double  hicl_knl_timed_exec(const char *name, hidev_t d);

CPPGUARD_END();

#endif  // HICL_KNL_H_