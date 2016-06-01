#ifndef __API_PLT_INL_H_
#define __API_PLT_INL_H_
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
/// \file plt-inl.h
/// \version $Id: plt-inl.h 2383 2014-05-05 01:09:12Z issam $
/// \brief Private routines for hicl_platform.
///
#include <stdio.h>
#include <string.h>
#include "hiCL/flags.h"
#include "hiCL/types.h"
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/config/util.h"
#include "__api/util-inl.h"

CPPGUARD_BEGIN()

#define __API_PLT_VENDOR_MASK AMD | APPLE | INTEL | NVIDIA

#define __API_PLT_GET(id, platform_info, value)                \
    HICL_CHECK(clGetPlatformInfo(id, platform_info,            \
                                 sizeof(value), &value, NULL), \
               "failed to query platform info")

#define __API_PLT_GET_PTR(id, platform_info, value)           \
    HICL_CHECK(clGetPlatformInfo(id, platform_info,           \
                                 sizeof(value), value, NULL), \
               "failed to query platform info")

#define __API_PLT_INFO_LEVEL_0(fmt, ...) fprintf(cl->fdout, \
C_GREEN"\no OpenCL "fmt"\n"C_END, ##__VA_ARGS__)

#define __API_PLT_INFO_LEVEL_1(fmt, ...) fprintf(cl->fdout, \
"\to %-20s: "fmt"\n", ##__VA_ARGS__)

#define __API_PLT_INFO_LEVEL_2(fmt, ...) fprintf(cl->fdout, \
"\t %-22s "fmt"\n", " ", ##__VA_ARGS__)

#define __API_PLT_TYPE_STR(flags)                                  \
    (__API_FLAGS_HAVE(flags, DEFAULT)  ? "DEFAULT"                :\
     __API_FLAGS_HAVE(flags, AMD)      ? "ADVANCED MICRO DEVICES" :\
     __API_FLAGS_HAVE(flags, APPLE)    ? "APPLE"                  :\
     __API_FLAGS_HAVE(flags, INTEL)    ? "INTEL"                  :\
     __API_FLAGS_HAVE(flags, NVIDIA)   ? "NVIDIA"                 :\
     "UNKOWN PLATFORM TYPE")

#define __API_PLT_CHECK_FLAGS(flags) \
    ((__API_FLAGS_HAVE(flags, DEFAULT) && \
        ! __API_FLAGS_HAVE(flags, AMD     | APPLE   | INTEL   | NVIDIA)) ||\
     (__API_FLAGS_HAVE(flags, AMD)     && \
        ! __API_FLAGS_HAVE(flags, DEFAULT | APPLE   | INTEL   | NVIDIA)) ||\
     (__API_FLAGS_HAVE(flags, APPLE)   && \
        ! __API_FLAGS_HAVE(flags, AMD     | DEFAULT | INTEL   | NVIDIA)) ||\
     (__API_FLAGS_HAVE(flags, INTEL)   && \
        ! __API_FLAGS_HAVE(flags, AMD     | APPLE   | DEFAULT | NVIDIA)) ||\
     (__API_FLAGS_HAVE(flags, NVIDIA)  && \
        ! __API_FLAGS_HAVE(flags, AMD     | APPLE   | INTEL   | DEFAULT)))

PRIVATE cl_uint
__api_plt_count() {
    cl_uint nb_platforms;
    HICL_CHECK(clGetPlatformIDs(0, NULL, &nb_platforms),
               "failed to query the number of OpenCL platforms");
    HICL_EXIT_IF(nb_platforms == 0, "no OpenCL platform found");
    return nb_platforms;
}

PRIVATE void
__api_plt_query(cl_platform_id *plt_ids, cl_uint nb_platforms) {
    HICL_CHECK(clGetPlatformIDs(nb_platforms, plt_ids, NULL),
               "failed to query OpenCL platforms");
}

PRIVATE cl_platform_id
__api_plt_select(cl_platform_id *plt_ids, cl_uint nb_platforms, flags_t flags) {
    flags_t plt_vendors[] = {AMD, APPLE, INTEL, NVIDIA};
    flags_t plt_flags     = flags & (__API_PLT_VENDOR_MASK);
    unsigned int i, f;
    char vendor[__API_STR_SIZE];
    HICL_EXIT_IF(!__API_PLT_CHECK_FLAGS(plt_flags),
                 "invalid hiCL flags");
    if (__API_FLAGS_HAVE(plt_flags, DEFAULT)) return plt_ids[0];
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
    HICL_EXIT("unable to find the desired OpenCL platform (%s)", 
               __API_PLT_TYPE_STR(plt_flags));
}

PRIVATE void
__api_plt_info(cl_platform_id id) {
    char name[__API_STR_SIZE];        
    char vendor[__API_STR_SIZE];      
    char version[__API_STR_SIZE];     
    char profile[__API_STR_SIZE];     
    char extensions[__API_BUFFER_SIZE];

    __API_PLT_GET_PTR(id, CL_PLATFORM_NAME, name);
    __API_PLT_GET_PTR(id, CL_PLATFORM_VENDOR, vendor);
    __API_PLT_GET_PTR(id, CL_PLATFORM_VERSION, version);
    __API_PLT_GET_PTR(id, CL_PLATFORM_PROFILE, profile);
    __API_PLT_GET_PTR(id, CL_PLATFORM_EXTENSIONS, extensions);
    
    __API_PLT_INFO_LEVEL_0("Platform @ %p", id);
    __API_PLT_INFO_LEVEL_1("%s", "name", name);
    __API_PLT_INFO_LEVEL_1("%s", "vendor", vendor);
    __API_PLT_INFO_LEVEL_1("%s", "profile", profile);
    if (strlen(extensions) != 0) {
        char ext[__API_STR_SIZE];
        __api_strstep(ext, extensions, " ");
        __API_PLT_INFO_LEVEL_1("%s", "extensions", ext);
        while(!__api_strstep(ext, extensions, " ")) {
            __API_PLT_INFO_LEVEL_2("%s", ext);
        }
    }
}

CPPGUARD_END()

#endif  // __API_PLT_INL_H_