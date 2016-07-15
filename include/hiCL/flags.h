#ifndef HICL_FLAGS_H_
#define HICL_FLAGS_H_
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
/// @file hiCL/flags.h
/// @author Issam SAID
/// @brief Bitwise flags used by hiCL to select OpenCL resources.
///
/// @details This file describes the bitwise flags that users can use to 
/// select OpenCL resources (platforms, devices, memory objects) or to 
/// determine the hiCL library behavior.
///
#define FIRST             (uint64_t)1<<1  ///< Select the 1st device in list.
#define SECOND            (uint64_t)1<<2  ///< Select the 2nd device in list.
#define THIRD             (uint64_t)1<<3  ///< Select the 3rd device in list.
#define FOURTH            (uint64_t)1<<4  ///< Select the 4th device in list.
#define FIFTH             (uint64_t)1<<5  ///< Select the 5th device in list.
#define SIXTH             (uint64_t)1<<6  ///< Select the 6th device in list.
#define SEVENTH           (uint64_t)1<<7  ///< Select the 7th device in list.
#define EIGHTH            (uint64_t)1<<8  ///< Select the 8th device in list.
// reserved               (uint64_t)1<<9
// reserved               (uint64_t)1<<10
// reserved               (uint64_t)1<<11
#define CPU               (uint64_t)1<<12 ///< Select the CPU devices in list.
#define GPU               (uint64_t)1<<13 ///< Select the GPU devices in list.
#define ACCELERATOR       (uint64_t)1<<14 ///< Select the Accelerators in list.
#define ALL               (uint64_t)1<<15 ///< Select all devices in list.
#define AMD               (uint64_t)1<<16 ///< Select the AMD platform.
#define APPLE             (uint64_t)1<<17 ///< Select the APPLE platform.
#define INTEL             (uint64_t)1<<18 ///< Select the INTEL platform.
#define NVIDIA            (uint64_t)1<<19 ///< Select the NVIDIA platform.
// reserved               (uint64_t)1<<20 
// reserved               (uint64_t)1<<21
// reserved               (uint64_t)1<<22
// reserved               (uint64_t)1<<23
#define HOST_DIRTY        (uint64_t)1<<24 ///< For host/device synchronization.
#define DEVICE_DIRTY      (uint64_t)1<<25 ///< For host/device synchronization.
// reserved               (uint64_t)1<<26
#define HWA               (uint64_t)1<<27 ///< Store memory on the device.
#define PINNED            (uint64_t)1<<28 ///< Use pinned memory.
#define ZERO_COPY         (uint64_t)1<<29 ///< Make the memory zero-copy.
#define HOST_ALLOCATED    (uint64_t)1<<30 ///< Use pre-allocated host memory.
#define READ_ONLY         (uint64_t)1<<31 ///< Use read-only data.  
#define WRITE_ONLY        (uint64_t)1<<32 ///< Use write-only data.  
#define READ_WRITE        (uint64_t)1<<33 ///< Use read-write data.  
#define CHAR              (uint64_t)1<<34 ///< The data contain chars.  
#define INT               (uint64_t)1<<35 ///< The data contain integers.  
#define UNSIGNED_INT      (uint64_t)1<<36 ///< The data contain unsigned ints.  
#define LONG              (uint64_t)1<<37 ///< The data contain long ints.  
#define UNSIGNED_LONG     (uint64_t)1<<38 ///< The data contain unsigned longs.  
#define SIZET             (uint64_t)1<<39 ///< The data contain size_ts.  
#define FLOAT             (uint64_t)1<<40 ///< The data contain floats.  
#define DOUBLE            (uint64_t)1<<41 ///< The data contain doubles.  
// ...
#define DEFAULT           (uint64_t)1<<53 ///< Choose the hiCL default behavior.  

/// Macro to check whether a set of flags contains a given mask.
#define __API_FLAGS_HAVE(flags, mask) ((flags & (mask)) == (mask))

#endif  // HICL_FLAGS_H_