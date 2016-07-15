#ifndef HICL_TYPES_H_
#define HICL_TYPES_H_
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
/// @file hiCL/types.h
/// @author Issam SAID
/// @brief Prototypes of the types and structures used by hiCL to describe
/// the OpenCL resources.
///
/// @details This file contains the definition of the types used by 
/// hiCL to describe the OpenCL resources such as the platform, the devices
/// the kernels and the memory objects.
///
#include <stdio.h>
#include <stdint.h>
#include "__api/config/opencl.h"
#include "__api/config/guard.h"
#include "__api/config/knl.h"
#include "__api/list-inl.h"
#include "__api/rbt-inl.h"

CPPGUARD_BEGIN()

typedef uint64_t flags_t;
typedef void     *address_t;
typedef struct __hidev_t *hidev_t;
typedef struct __himem_t *himem_t;
typedef struct __hiknl_t *hiknl_t;
typedef struct __hienv_t *hienv_t;

GENERATE_LIST_HEADER(hidev_t);
GENERATE_LIST_HEADER(hiknl_t);
GENERATE_RBT_HEADER(address_t, himem_t);
GENERATE_RBT_HEADER(int, himem_t);
GENERATE_RBT_HEADER(hiknl_t, int);

///
/// @brief The hiCL device descriptor.
///
/// This structure is a high level description of an OpenCL device.
/// It contains an OpenCL command queue that is dedicated to run kernels
/// on that device.
///
struct __hidev_t {
    cl_device_id id;        ///< the OpenCL device identifier.
    cl_command_queue queue; ///< a command queue associated to the device.
};

///
/// @brief The hiCL memory object descriptor.
///
/// This structure is a high level description of an OpenCL memory object.
///
struct __himem_t {
    flags_t flags;          ///< the hiCL flags used to allocate the memory.
    size_t size;            ///< the size of the memory in number of elements.
    size_t unit_size;       ///< the size of each element in bytes.
    cl_mem id;              ///< the OpenCL identifier of the memory object.  
    cl_mem pinned;          ///< an memory object used if the memory is pinned.
    cl_command_queue queue; ///< a queue used to manipulate the memory object.
    void* h;                ///< a pointer that corresponds 
    rbt_hiknl_t_int knls;   ///< a red-black tree of the kernels using this memory.
};

///
/// @brief The hiCL kernel descriptor.
///
/// This structure is a high level description of an OpenCL kernel.
///
struct __hiknl_t {
    size_t gws[__API_KNL_MAX_WORK_SIZE]; ///< the global size.
    size_t lws[__API_KNL_MAX_WORK_SIZE]; ///< the local size.
    size_t ofs[__API_KNL_MAX_WORK_SIZE]; ///< the offsets.
    cl_uint wrk;                         ///< the work dimensions.
    cl_uint num_args;                    ///< the number of arguments.
    cl_kernel id;                        ///< the OpenCL identifier.
    rbt_int_himem_t mems;                ///< a red-black tree of the memory 
                                         ///  objects used by the kernel.
};

///
/// @brief The hiCL envirenment descriptor.
///
/// This sturcture represents the hiCL resources manager.
///
struct __hienv_t {
    cl_platform_id platform_id; ///< the OpenCL platform, we only handle one platform.  
    cl_context context;         ///< the OpenCL context. 
    list_hidev_t *devs;         ///< a list of the used devices.
    list_hiknl_t *knls;         ///< a list of the used kernels.
    rbt_address_t_himem_t mems; ///< a red-black tree of the active memory objects.
    FILE* fdout;                ///< a file descriptor for logging.
    FILE* fderr;                ///< a file descriptor for error reporting.
};

CPPGUARD_END()

#endif  // HICL_TYPES_H_
