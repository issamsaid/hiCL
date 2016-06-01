#ifndef __API_DEV_INL_H_
#define __API_DEV_INL_H_
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
/// \file dev-inl.h
/// \version $Id: dev-inl.h 2396 2014-05-10 12:40:26Z issam $
/// \brief Private routines for easyhicl_dev.
///
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hiCL/flags.h"
#include "hiCL/types.h"
#include "__api/config/dev.h"
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/config/util.h"
#include "__api/util-inl.h"     

CPPGUARD_BEGIN()

#define __API_DEV_TYPE_MASK ALL | CPU | GPU | ACCELERATOR

#define __API_DEV_INDEX_MASK FIRST | SECOND | THIRD   | FOURTH |\
                             FIFTH | SIXTH  | SEVENTH | EIGHTH

#define __API_DEV_GET(id, device_info, value)                                 \
    HICL_CHECK(clGetDeviceInfo(id, device_info, sizeof(value), &value, NULL), \
               "failed to query device info")

#define __API_DEV_GET_PTR(id, device_info, value)                            \
    HICL_CHECK(clGetDeviceInfo(id, device_info, sizeof(value), value, NULL), \
               "failed to query device info")

#define __API_DEV_TYPE_STR(t)                                        \
    (t == CL_DEVICE_TYPE_ALL         ? "CL_DEVICE_TYPE_ALL"         :\
     t == CL_DEVICE_TYPE_CPU         ? "CL_DEVICE_TYPE_CPU"         :\
     t == CL_DEVICE_TYPE_GPU         ? "CL_DEVICE_TYPE_GPU"         :\
     t == CL_DEVICE_TYPE_ACCELERATOR ? "CL_DEVICE_TYPE_ACCELERATOR" :\
     t == CL_DEVICE_TYPE_DEFAULT     ? "CL_DEVICE_TYPE_DEFAULT"     :\
     "BAD DEVICE TYPE")

#define __API_DEV_INFO_LEVEL_0(fmt, ...) fprintf(cl->fdout, \
C_GREEN"\n\to OpenCL "fmt"\n"C_END, ##__VA_ARGS__)

#ifdef __API_DEV_LONG_INFO_ENABLED
#define __API_DEV_INFO_LEVEL_1(fmt, ...) fprintf(cl->fdout, \
"\t\to %-30s: "fmt"\n", ##__VA_ARGS__)
#define __API_DEV_INFO_LEVEL_2(fmt, ...) fprintf(cl->fdout, \
"\t\t %32s "fmt"\n", " ", ##__VA_ARGS__)
/// \brief Convert OpenCl device global mem cache type to a string constant
#define __API_DEV_GLOBAL_MEM_CACHE_TYPE_STR(c)               \
    (c == CL_NONE             ? "CL_NONE"             :      \
     c == CL_READ_ONLY_CACHE  ? "CL_READ_ONLY_CACHE"  :      \
     c == CL_READ_WRITE_CACHE ? "CL_READ_WRITE_CACHE" :      \
      "BAD DEVICE GLOBAL CACHE TYPE")
/// \brief Convert OpenCl device local mem type to a string constant
#define __API_DEV_LOCAL_MEM_TYPE_STR(l) \
    (l == CL_NONE   ? "CL_NONE"     :   \
     l == CL_GLOBAL ? "CL_GLOBAL"   :   \
     l == CL_LOCAL  ? "CL_LOCAL"    :   \
     "BAD DEVICE LOCAL MEM TYPE")

#else
#define __API_DEV_INFO_LEVEL_1(fmt, ...) fprintf(cl->fdout, \
"\t\to %-20s: "fmt"\n", ##__VA_ARGS__)
#define __API_DEV_INFO_LEVEL_2(fmt, ...) fprintf(cl->fdout, \
"\t\t %22s "fmt"\n", " ", ##__VA_ARGS__)
#endif  // __API_DEV_LONG_INFO_ENABLED

PRIVATE cl_device_type
__api_dev_flags_to_type(flags_t flags) {
    HICL_DEBUG("flags to type %#016lx", flags);
    if (__API_FLAGS_HAVE(flags, DEFAULT))     return CL_DEVICE_TYPE_DEFAULT;
    if (__API_FLAGS_HAVE(flags, CPU))         return CL_DEVICE_TYPE_CPU;
    if (__API_FLAGS_HAVE(flags, GPU))         return CL_DEVICE_TYPE_GPU;
    if (__API_FLAGS_HAVE(flags, ACCELERATOR)) return CL_DEVICE_TYPE_ACCELERATOR;
    if (__API_FLAGS_HAVE(flags, ALL))         return CL_DEVICE_TYPE_ALL;
    HICL_EXIT("unable to identify type from hiCL flags");
}

PRIVATE cl_uint
__api_dev_flags_to_index(flags_t flags) {
    HICL_DEBUG("flags to index %#016lx", flags);
    if (__API_FLAGS_HAVE(flags, DEFAULT)) return 0;
    else return (cl_uint)(log2(flags));
}

PRIVATE cl_uint
__api_dev_select_by_type(cl_device_id *ids, cl_uint in_devices, flags_t flags) {
    cl_uint out_devices = 0;
    cl_device_id tmp_id;
    cl_device_type ftype, type;
    unsigned int f, i;
    flags_t dev_types[]  = {ALL, CPU, GPU, ACCELERATOR};
    if (__API_FLAGS_HAVE(flags, DEFAULT)) return 1;
    
    for (f = 0; f < sizeof(dev_types)/sizeof(*dev_types); ++f) {
        if ((flags & dev_types[f]) == dev_types[f]) {
            ftype = __api_dev_flags_to_type(dev_types[f]);
            for (i = out_devices; i < in_devices; ++i) {
                __API_DEV_GET(ids[i], CL_DEVICE_TYPE, type);
                if (ftype & type) {
                    tmp_id             = ids[out_devices];
                    ids[out_devices++] = ids[i];
                    ids[i]             = tmp_id;
                }
            }
        }
    }
    return out_devices ? out_devices : in_devices;
}

PRIVATE cl_uint
__api_dev_select_by_index(cl_device_id *ids, 
                          cl_uint in_devices, flags_t flags) {
    cl_device_id tmp_ids[in_devices];
    flags_t dev_indices[] = {FIRST,  SECOND,  THIRD, FOURTH,
                             FIFTH, SIXTH, SEVENTH, EIGHTH};
    cl_uint f, i, findex, out_devices = 0;
    if (__API_FLAGS_HAVE(flags, DEFAULT)) return 1;
   
    for (f = 0; f < sizeof(dev_indices)/sizeof(*dev_indices); ++f) {
        if ((flags & dev_indices[f]) == dev_indices[f]) {
            findex = __api_dev_flags_to_index(dev_indices[f]);
            HICL_EXIT_IF(findex > in_devices,
                         "OpenCL device index (%u) out of bound (%u)",
                         findex, in_devices);
            tmp_ids[out_devices++] = ids[findex];
        }
    }
    for (i = 0; i < out_devices; ++i) ids[i] = tmp_ids[i];
    return out_devices ? out_devices : in_devices;
}

PRIVATE cl_uint
__api_dev_count(cl_platform_id plt) {
    cl_uint nb_devices;
    HICL_CHECK(clGetDeviceIDs(plt, CL_DEVICE_TYPE_ALL, 0, NULL, &nb_devices),
               "failed to query the number of OpenCL devices");
    HICL_EXIT_IF(nb_devices == 0, "no OpenCL devices found");
    return nb_devices;
}

PRIVATE void
__api_dev_query(cl_platform_id plt, cl_device_id *dev_ids, cl_uint nb_devices) {
    HICL_CHECK(clGetDeviceIDs(plt, 
                              CL_DEVICE_TYPE_ALL, nb_devices, dev_ids, NULL),
               "failed to query OpenCL devices");
}

PRIVATE cl_uint
__api_dev_select(cl_platform_id plt, 
                 cl_device_id *ids, cl_uint nb_devices, flags_t flags) {
    cl_uint n;
    n = __api_dev_select_by_type(ids, n, flags & __API_DEV_TYPE_MASK);
    n = __api_dev_select_by_index(ids, n, flags & __API_DEV_INDEX_MASK);
    HICL_EXIT_IF(n == 0, "failed to load the desired OpenCL devices");
    return n;
}

PRIVATE cl_bool 
__api_dev_extension_supported(cl_device_id id, char* extension) {
    char extensions[__API_BUFFER_SIZE];
    __API_DEV_GET_PTR(id, CL_DEVICE_EXTENSIONS, extensions);
    return strstr(extensions, extension) != NULL;
}

PRIVATE cl_bool 
__api_dev_available(cl_device_id id) {
    cl_bool available;
    __API_DEV_GET(id, CL_DEVICE_AVAILABLE, available);
    return available;
}

PRIVATE cl_bool
__api_dev_compiler_available(cl_device_id id) {
    cl_bool available;
    __API_DEV_GET(id, CL_DEVICE_COMPILER_AVAILABLE, available);
    return available;
}

#ifdef __API_DEV_LONG_INFO_ENABLED
/// \brief Convert OpenCl device single fp config value to a string constant
PRIVATE void 
__api_dev_single_fp_config_str(cl_device_fp_config fc) {
    __API_DEV_INFO_LEVEL_1("%s", "execution capabilities",
                               fc & CL_FP_ROUND_TO_NEAREST ?
                               "CL_FP_ROUND_TO_NEAREST" : " ");
    if (fc & CL_FP_DENORM)
        __API_DEV_INFO_LEVEL_2("%s", "CL_FP_DENORM");
    if (fc & CL_FP_INF_NAN)
        __API_DEV_INFO_LEVEL_2("%s", "CL_FP_INF_NAN");
    if (fc & CL_FP_ROUND_TO_ZERO)
        __API_DEV_INFO_LEVEL_2("%s", "CL_FP_ROUND_TO_ZERO");
    if (fc & CL_FP_ROUND_TO_INF)
        __API_DEV_INFO_LEVEL_2("%s", "CL_FP_ROUND_TO_INF");
    if (fc & CL_FP_FMA)
        __API_DEV_INFO_LEVEL_2("%s", "CL_FP_FMA");
    if (fc & CL_FP_SOFT_FLOAT)
        __API_DEV_INFO_LEVEL_2("%s", "CL_FP_SOFT_FLOAT");
#ifdef CL_VERSION_1_2
    if (fc & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT)
        __API_DEV_INFO_LEVEL_2("%s", "CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT");
#endif  // CL_VERSION_1_2
}

/// \brief Convert OpenCl device single fp config value to a string constant
PRIVATE void 
__api_dev_execution_capabilities_str(cl_device_exec_capabilities ec) {
    if (ec & CL_EXEC_KERNEL) {
        __API_DEV_INFO_LEVEL_1("%s", "exec capabilities", "CL_EXEC_KERNEL");
        if (ec & CL_EXEC_NATIVE_KERNEL)
            __API_DEV_INFO_LEVEL_2("%s", "CL_EXEC_NATIVE_KERNEL");
    } else {
        if (ec & CL_EXEC_NATIVE_KERNEL)
            __API_DEV_INFO_LEVEL_1("%s", "exec capabilities",
                                       "CL_EXEC_NATIVE_KERNEL");
    }
}

/// \brief Convert OpenCl device single fp config value to a string constant
PRIVATE void 
__api_dev_queue_properties_str(cl_command_queue_properties qp) {
    if (qp & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) {
        __API_DEV_INFO_LEVEL_1("%s", "queue properties",
                                   "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE");
        if (qp & CL_QUEUE_PROFILING_ENABLE)
            __API_DEV_INFO_LEVEL_2("%s", "CL_QUEUE_PROFILING_ENABLE");

    } else {
        if (qp & CL_QUEUE_PROFILING_ENABLE)
            __API_DEV_INFO_LEVEL_1("%s", "queue properties",
                                       "CL_QUEUE_PROFILING_ENABLE");
    }
}
#endif  // __API_DEV_LONG_INFO_ENABLED

PRIVATE void 
__api_dev_info(cl_device_id id) {
    cl_device_type device_type;
    char name[__API_STR_SIZE];
    char vendor[__API_STR_SIZE];
    char driver_version[__API_STR_SIZE];
    char profile[__API_STR_SIZE];
    char extensions[__API_BUFFER_SIZE];
    __API_DEV_GET(id, CL_DEVICE_TYPE, device_type);
    __API_DEV_GET_PTR(id, CL_DEVICE_NAME, name);
    __API_DEV_GET_PTR(id, CL_DEVICE_VENDOR, vendor);
    __API_DEV_GET_PTR(id, CL_DRIVER_VERSION, driver_version);
    __API_DEV_GET_PTR(id, CL_DEVICE_PROFILE, profile);
    __API_DEV_GET_PTR(id, CL_DEVICE_EXTENSIONS, extensions);
    __API_DEV_INFO_LEVEL_0("Device @ %p", id);
    __API_DEV_INFO_LEVEL_1("%s", "type",
                              __API_DEV_TYPE_STR(device_type));
    __API_DEV_INFO_LEVEL_1("%s", "name", name);
    __API_DEV_INFO_LEVEL_1("%s", "vendor", vendor);
    __API_DEV_INFO_LEVEL_1("%s", "driver version", driver_version);
    __API_DEV_INFO_LEVEL_1("%s", "profile", profile);
    if (strlen(extensions) != 0) {
        char ext[__API_STR_SIZE];
        __api_strstep(ext, extensions, " ");
        __API_DEV_INFO_LEVEL_1("%s", "extensions", ext);
        while(!__api_strstep(ext, extensions, " ")) {
            __API_DEV_INFO_LEVEL_2("%s", ext);
        }
    }
#ifdef __API_DEV_LONG_INFO_ENABLED
    unsigned int i;
    char device_version[__API_STR_SIZE];
    cl_uint vendor_id;
    cl_uint max_compute_units;
    cl_uint max_work_item_dimensions;
    size_t max_work_item_sizes[3];
    size_t max_work_group_size;
    cl_uint preferred_vector_width_char;
    cl_uint preferred_vector_width_short;
    cl_uint preferred_vector_width_int;
    cl_uint preferred_vector_width_long;
    cl_uint preferred_vector_width_float;
    cl_uint preferred_vector_width_double;
    cl_uint max_clock_frequency;
    cl_uint address_bits;
    cl_ulong max_mem_alloc_size;
    cl_bool image_support;
    cl_uint max_read_image_args;
    cl_uint max_write_image_args;
    size_t image2d_max_width;
    size_t image2d_max_height;
    size_t image3d_max_width;
    size_t image3d_max_height;
    size_t image3d_max_depth;
    cl_uint max_samplers;
    size_t max_parameter_size;
    cl_uint mem_base_addr_align;
    cl_uint min_data_type_align_size;
    cl_device_fp_config single_fp_config;
    cl_device_mem_cache_type global_mem_cache_type;
    cl_uint global_mem_cacheline_size;
    cl_ulong global_mem_cache_size;
    cl_ulong global_mem_size;
    cl_ulong max_constant_buffer_size;
    cl_uint max_constant_args;
    cl_device_local_mem_type local_mem_type;
    cl_ulong local_mem_size;
    cl_bool error_correction_support;
    size_t profiling_timer_resolution;
    cl_bool endian_little;
    cl_device_exec_capabilities execution_capabilities;
    cl_command_queue_properties queue_properties;
    cl_platform_id platform_id;

    __API_DEV_GET_PTR(id, CL_DEVICE_VERSION,
                         device_version);
    __API_DEV_GET(id, CL_DEVICE_VENDOR_ID,
                     vendor_id);
    __API_DEV_GET(id, CL_DEVICE_MAX_COMPUTE_UNITS,
                     max_compute_units);
    __API_DEV_GET(id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                     max_work_item_dimensions);
    __API_DEV_GET_PTR(id, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                         max_work_item_sizes);
    __API_DEV_GET(id, CL_DEVICE_MAX_WORK_GROUP_SIZE,
                     max_work_group_size);
    __API_DEV_GET(id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
                     preferred_vector_width_char);
    __API_DEV_GET(id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
                     preferred_vector_width_short);
    __API_DEV_GET(id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
                     preferred_vector_width_int);
    __API_DEV_GET(id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
                     preferred_vector_width_long);
    __API_DEV_GET(id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
                     preferred_vector_width_float);
    __API_DEV_GET(id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
                     preferred_vector_width_double);
    __API_DEV_GET(id, CL_DEVICE_MAX_CLOCK_FREQUENCY,
                     max_clock_frequency);
    __API_DEV_GET(id, CL_DEVICE_ADDRESS_BITS,
                     address_bits);
    __API_DEV_GET(id, CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                     max_mem_alloc_size);
    __API_DEV_GET(id, CL_DEVICE_IMAGE_SUPPORT,
                     image_support);
    __API_DEV_GET(id, CL_DEVICE_MAX_READ_IMAGE_ARGS,
                     max_read_image_args);
    __API_DEV_GET(id, CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
                     max_write_image_args);
    __API_DEV_GET(id, CL_DEVICE_IMAGE2D_MAX_WIDTH,
                     image2d_max_width);
    __API_DEV_GET(id, CL_DEVICE_IMAGE2D_MAX_HEIGHT,
                     image2d_max_height);
    __API_DEV_GET(id, CL_DEVICE_IMAGE3D_MAX_WIDTH,
                     image3d_max_width);
    __API_DEV_GET(id, CL_DEVICE_IMAGE3D_MAX_HEIGHT,
                     image3d_max_height);
    __API_DEV_GET(id, CL_DEVICE_IMAGE3D_MAX_DEPTH,
                     image3d_max_depth);
    __API_DEV_GET(id, CL_DEVICE_MAX_SAMPLERS,
                     max_samplers);
    __API_DEV_GET(id, CL_DEVICE_MAX_PARAMETER_SIZE,
                     max_parameter_size);
    __API_DEV_GET(id, CL_DEVICE_MEM_BASE_ADDR_ALIGN,
                     mem_base_addr_align);
    __API_DEV_GET(id, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
                     min_data_type_align_size);
    __API_DEV_GET(id, CL_DEVICE_SINGLE_FP_CONFIG,
                     single_fp_config);
    __API_DEV_GET(id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                     global_mem_cache_type);
    __API_DEV_GET(id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                     global_mem_cache_size);
    __API_DEV_GET(id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                     global_mem_cacheline_size);
    __API_DEV_GET(id, CL_DEVICE_GLOBAL_MEM_SIZE,
                     global_mem_size);
    __API_DEV_GET(id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
                     max_constant_buffer_size);
    __API_DEV_GET(id, CL_DEVICE_MAX_CONSTANT_ARGS,
                     max_constant_args);
    __API_DEV_GET(id, CL_DEVICE_LOCAL_MEM_TYPE,
                     local_mem_type);
    __API_DEV_GET(id, CL_DEVICE_LOCAL_MEM_SIZE,
                     local_mem_size);
    __API_DEV_GET(id, CL_DEVICE_ERROR_CORRECTION_SUPPORT,
                     error_correction_support);
    __API_DEV_GET(id, CL_DEVICE_PROFILING_TIMER_RESOLUTION,
                     profiling_timer_resolution);
    __API_DEV_GET(id, CL_DEVICE_ENDIAN_LITTLE,
                     endian_little);
    __API_DEV_GET(id, CL_DEVICE_EXECUTION_CAPABILITIES,
                     execution_capabilities);
    __API_DEV_GET(id, CL_DEVICE_QUEUE_PROPERTIES,
                     queue_properties);
    __API_DEV_GET(id, CL_DEVICE_PLATFORM,
                     platform_id);

    __API_DEV_INFO_LEVEL_1("%s", "device version",
                              device_version);
    __API_DEV_INFO_LEVEL_1("%u", "vendor id",
                              vendor_id);
    __API_DEV_INFO_LEVEL_1("%u", "max compute units",
                              max_compute_units);
    __API_DEV_INFO_LEVEL_1("%u", "max work item dimensions",
                              max_work_item_dimensions);
    __API_DEV_INFO_LEVEL_1("dimension[0] = %ld", "max work sizes",
                              max_work_item_sizes[0]);
    for (i = 1; i < sizeof(max_work_item_sizes)/sizeof(size_t); ++i)
        __API_DEV_INFO_LEVEL_2("dimension[%d] = %ld",
                                  i, max_work_item_sizes[i]);
    __API_DEV_INFO_LEVEL_1("%lu", "max work group size",
                              max_work_group_size);
    __API_DEV_INFO_LEVEL_1("%u", "preferred vector width char",
                              preferred_vector_width_char);
    __API_DEV_INFO_LEVEL_1("%u", "preferred vector width short",
                              preferred_vector_width_short);
    __API_DEV_INFO_LEVEL_1("%u", "preferred vector width int",
                              preferred_vector_width_int);
    __API_DEV_INFO_LEVEL_1("%u", "preferred vector width long",
                              preferred_vector_width_long);
    __API_DEV_INFO_LEVEL_1("%u", "preferred vector width float",
                              preferred_vector_width_float);
    __API_DEV_INFO_LEVEL_1("%u", "preferred vector width double",
                              preferred_vector_width_double);
    __API_DEV_INFO_LEVEL_1("%u", "max clock frequency",
                              max_clock_frequency);
    __API_DEV_INFO_LEVEL_1("%u", "address bits",
                              address_bits);
    __API_DEV_INFO_LEVEL_1("%s", "image support",
                              image_support ? "yes" : "no");
    if (image_support) {
        __API_DEV_INFO_LEVEL_1("%u", "max read image args",
                                  max_read_image_args);
        __API_DEV_INFO_LEVEL_1("%u", "max write image args",
                                  max_write_image_args);
        __API_DEV_INFO_LEVEL_1("%lu", "image2d max width",
                                  image2d_max_width);
        __API_DEV_INFO_LEVEL_1("%lu", "image2d max height",
                                  image2d_max_height);
        __API_DEV_INFO_LEVEL_1("%lu", "image3d max width",
                                  image3d_max_width);
        __API_DEV_INFO_LEVEL_1("%lu", "image3d max height",
                                  image3d_max_height);
        __API_DEV_INFO_LEVEL_1("%lu", "image3d max depth",
                                  image3d_max_depth);
        __API_DEV_INFO_LEVEL_1("%u", "max samplers",
                                  max_samplers);
    }
    __API_DEV_INFO_LEVEL_1("%ld", "max paramater size",
                              max_parameter_size);
    __API_DEV_INFO_LEVEL_1("%u", "mem base addr align",
                              mem_base_addr_align);
    __API_DEV_INFO_LEVEL_1("%u", "min data type align size",
                              min_data_type_align_size);
    __API_DEV_INFO_LEVEL_1("", "single fp cpnfig");
    __api_dev_single_fp_config_str(single_fp_config);
    __API_DEV_INFO_LEVEL_1("%s", "global mem cache type",
          __API_DEV_GLOBAL_MEM_CACHE_TYPE_STR(global_mem_cache_type));
    __API_DEV_INFO_LEVEL_1("%u Bytes", "global mem cacheline size",
                              global_mem_cacheline_size);
    __API_DEV_INFO_LEVEL_1("%lu"" Bytes", "global mem cache size",
                              global_mem_cache_size);
    __API_DEV_INFO_LEVEL_1("%lu"" MB", "global mem size",
                              global_mem_size/1024/1024);
    __API_DEV_INFO_LEVEL_1("%lu"" MB", "global mem max alloc",
			      max_mem_alloc_size/1024/1024);	
    __API_DEV_INFO_LEVEL_1("%lu"" KB", "max constant buffer size",
                              max_constant_buffer_size/1024);
    __API_DEV_INFO_LEVEL_1("%u", "max constant args",
                              max_constant_args);
    __API_DEV_INFO_LEVEL_1("%s", "local mem type",
                              __API_DEV_LOCAL_MEM_TYPE_STR(local_mem_type));
    __API_DEV_INFO_LEVEL_1("%lu"" KB", "local mem size",
                              local_mem_size/1024);
    __API_DEV_INFO_LEVEL_1("%s", "error correction support",
                              error_correction_support ? "yes" : "no");
    __API_DEV_INFO_LEVEL_1("%lu", "profiling timer resolution",
                              profiling_timer_resolution);
    __API_DEV_INFO_LEVEL_1("%s", "endian little",
                              endian_little ? "yes" : "no");
    __api_dev_execution_capabilities_str(execution_capabilities);
#endif  // _CL_DEVICE_LONG_INFO_ENABLED
}

PRIVATE void
__api_dev_release(dev d) {
    if(d) {
        HICL_DEBUG("releasing OpenCL device @ %p", d->id);
        if (clReleaseCommandQueue(d->queue) != CL_SUCCESS)
            HICL_WARN("failed to release OpenCL command queue");
        free(d); d = NULL;
    }
}

CPPGUARD_END()

#endif  // __API_DEV_INL_H_