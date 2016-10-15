#ifndef __API_PLT_INL_H_
#define __API_PLT_INL_H_
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
/// @file __api/plt-inl.h
/// @author Issam SAID
/// @brief OpenCL platform manipulation routines.
///
#include <stdio.h>
#include <string.h>
#include <hiCL/flags.h>
#include <hiCL/types.h>
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/config/util.h"
#include "__api/util-inl.h"

CPPGUARD_BEGIN();

#define __API_PLT_VENDOR_MASK DEFAULT | AMD | APPLE | INTEL | NVIDIA

#define __API_PLT_CHECK_FLAGS(flags)       \
   ((! __API_FLAGS_HAVE(flags, DEFAULT) && \
     ! __API_FLAGS_HAVE(flags, AMD)     && \
     ! __API_FLAGS_HAVE(flags, APPLE)   && \
     ! __API_FLAGS_HAVE(flags, INTEL)   && \
     ! __API_FLAGS_HAVE(flags, NVIDIA)) || \
     ( __API_FLAGS_HAVE(flags, DEFAULT) && \
     ! __API_FLAGS_HAVE(flags, AMD)     && \
     ! __API_FLAGS_HAVE(flags, APPLE)   && \
     ! __API_FLAGS_HAVE(flags, INTEL)   && \
     ! __API_FLAGS_HAVE(flags, NVIDIA)) || \
     ( __API_FLAGS_HAVE(flags, AMD)     && \
     ! __API_FLAGS_HAVE(flags, DEFAULT) && \
     ! __API_FLAGS_HAVE(flags, APPLE)   && \
     ! __API_FLAGS_HAVE(flags, INTEL)   && \
     ! __API_FLAGS_HAVE(flags, NVIDIA)) || \
     ( __API_FLAGS_HAVE(flags, APPLE)   && \
     ! __API_FLAGS_HAVE(flags, AMD)     && \
     ! __API_FLAGS_HAVE(flags, DEFAULT) && \
     ! __API_FLAGS_HAVE(flags, INTEL)   && \
     ! __API_FLAGS_HAVE(flags, NVIDIA)) || \
     ( __API_FLAGS_HAVE(flags, INTEL)   && \
     ! __API_FLAGS_HAVE(flags, AMD)     && \
     ! __API_FLAGS_HAVE(flags, APPLE)   && \
     ! __API_FLAGS_HAVE(flags, DEFAULT) && \
     ! __API_FLAGS_HAVE(flags, NVIDIA)) || \
     ( __API_FLAGS_HAVE(flags, NVIDIA)  && \
     ! __API_FLAGS_HAVE(flags, AMD)     && \
     ! __API_FLAGS_HAVE(flags, APPLE)   && \
     ! __API_FLAGS_HAVE(flags, INTEL)   && \
     ! __API_FLAGS_HAVE(flags, DEFAULT)))

#define __API_PLT_GET(id, platform_info, value)                \
    HICL_ABORT(clGetPlatformInfo(id, platform_info,            \
                                 sizeof(value), &value, NULL), \
               "failed to query platform info")

#define __API_PLT_GET_PTR(id, platform_info, value)           \
    HICL_ABORT(clGetPlatformInfo(id, platform_info,           \
                                 sizeof(value), value, NULL), \
               "failed to query platform info")

#define __API_PLT_INFO_LEVEL_0(fdout, fmt, ...) fprintf(fdout, \
HICL_GREEN"\no OpenCL "fmt"\n"HICL_END, ##__VA_ARGS__)

#define __API_PLT_INFO_LEVEL_1(fdout, fmt, ...) fprintf(fdout, \
"\to %-20s: "fmt"\n", ##__VA_ARGS__)

#define __API_PLT_INFO_LEVEL_2(fdout, fmt, ...) fprintf(fdout, \
"\t %-22s "fmt"\n", " ", ##__VA_ARGS__)

#define __API_PLT_TYPE_STR(flags)                                  \
    (__API_FLAGS_HAVE(flags, AMD)      ? "ADVANCED MICRO DEVICES" :\
     __API_FLAGS_HAVE(flags, APPLE)    ? "APPLE"                  :\
     __API_FLAGS_HAVE(flags, INTEL)    ? "INTEL"                  :\
     __API_FLAGS_HAVE(flags, NVIDIA)   ? "NVIDIA"                 :\
     "DEFAULT")

PRIVATE cl_uint
__api_plt_count() {
    cl_uint nb_platforms;
    HICL_ABORT(clGetPlatformIDs(0, NULL, &nb_platforms),
               "failed to query the number of OpenCL platforms");
    return nb_platforms;
}

PRIVATE void
__api_plt_query(cl_platform_id *plt_ids, cl_uint nb_platforms) {
    HICL_ABORT(clGetPlatformIDs(nb_platforms, plt_ids, NULL),
               "failed to query OpenCL platforms");
}

PRIVATE cl_platform_id
__api_plt_select(cl_platform_id *plt_ids, cl_uint nb_platforms, flags_t flags) {
    flags_t plt_vendors[] = {AMD, APPLE, INTEL, NVIDIA};
    flags_t plt_flags     = flags & (__API_PLT_VENDOR_MASK);
    unsigned int i, f;
    char vendor[__API_STR_SIZE];
    HICL_FAIL_IF(!__API_PLT_CHECK_FLAGS(plt_flags), 
                "invalid hiCL platform flags");
    if ((__API_FLAGS_HAVE(plt_flags, DEFAULT)) || (plt_flags == 0)) 
        return plt_ids[0];
    for (f = 0; f < sizeof(plt_vendors)/sizeof(*plt_vendors); ++f) {
        if (__API_FLAGS_HAVE(plt_flags, plt_vendors[f])) {
            for (i = 0; i < nb_platforms; ++i) {
                __API_PLT_GET_PTR(plt_ids[i], CL_PLATFORM_VENDOR, vendor);
                __api_strupper(vendor);
                if (strstr(vendor, __API_PLT_TYPE_STR(plt_vendors[f]))) {
                    return plt_ids[i]; 
                }
            }
        }
    }
    return NULL;
}

PRIVATE void
__api_plt_info(cl_platform_id id, FILE* fdout) {
    char name[__API_STR_SIZE];        
    char vendor[__API_STR_SIZE];      
    char version[__API_STR_SIZE];     
    char profile[__API_STR_SIZE];     
    char extensions[__API_BUFFER_SIZE];
    char *marker;
    __API_PLT_GET_PTR(id, CL_PLATFORM_NAME, name);
    __API_PLT_GET_PTR(id, CL_PLATFORM_VENDOR, vendor);
    __API_PLT_GET_PTR(id, CL_PLATFORM_VERSION, version);
    __API_PLT_GET_PTR(id, CL_PLATFORM_PROFILE, profile);
    __API_PLT_GET_PTR(id, CL_PLATFORM_EXTENSIONS, extensions);
    
    __API_PLT_INFO_LEVEL_0(fdout, "Platform @ %p", id);
    __API_PLT_INFO_LEVEL_1(fdout, "%s", "name", name);
    __API_PLT_INFO_LEVEL_1(fdout, "%s", "vendor", vendor);
    __API_PLT_INFO_LEVEL_1(fdout, "%s", "profile", profile);
    if (strlen(extensions) != 0) {
        char ext[__API_STR_SIZE];
        marker = __api_strstep(ext, extensions, " ");
        __API_PLT_INFO_LEVEL_1(fdout, "%s", "extensions", ext);
        do {
            marker = __api_strstep(ext, marker, " ");
            __API_PLT_INFO_LEVEL_2(fdout, "%s", ext);
        } while(marker != NULL);
    }
}

CPPGUARD_END();

#endif  // __API_PLT_INL_H_