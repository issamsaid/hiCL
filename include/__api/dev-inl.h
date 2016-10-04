#ifndef __API_DEV_INL_H_
#define __API_DEV_INL_H_
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
/// @file __api/dev-inl.h
/// @author Issam SAID
/// @brief Private functions used to implement the hiCL device descriptor.
///
#include <stdio.h>
#include <string.h>
#include <hiCL/flags.h>
#include <hiCL/types.h>
#include "__api/config/dev.h"
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/config/util.h"
#include "__api/util-inl.h"     

CPPGUARD_BEGIN();

extern hienv_t hicl;

#define __API_DEV_TYPE_MASK DEFAULT | ALL | CPU | GPU | ACCELERATOR

#define __API_DEV_INDEX_MASK DEFAULT | FIRST | SECOND | THIRD   |\
                             FOURTH  | FIFTH | SIXTH  | SEVENTH | EIGHTH

#define __API_DEV_CHECK_TYPE_FLAGS(flags)       \
   ((! __API_FLAGS_HAVE(flags, DEFAULT)      && \
     ! __API_FLAGS_HAVE(flags, ALL)          && \
     ! __API_FLAGS_HAVE(flags, CPU)          && \
     ! __API_FLAGS_HAVE(flags, GPU)          && \
     ! __API_FLAGS_HAVE(flags, ACCELERATOR)) || \
     ( __API_FLAGS_HAVE(flags, DEFAULT)      && \
     ! __API_FLAGS_HAVE(flags, ALL)          && \
     ! __API_FLAGS_HAVE(flags, CPU)          && \
     ! __API_FLAGS_HAVE(flags, GPU)          && \
     ! __API_FLAGS_HAVE(flags, ACCELERATOR)) || \
     ( __API_FLAGS_HAVE(flags, ALL)          && \
     ! __API_FLAGS_HAVE(flags, DEFAULT)      && \
     ! __API_FLAGS_HAVE(flags, CPU)          && \
     ! __API_FLAGS_HAVE(flags, GPU)          && \
     ! __API_FLAGS_HAVE(flags, ACCELERATOR)) || \
     ( __API_FLAGS_HAVE(flags, CPU)          && \
     ! __API_FLAGS_HAVE(flags, ALL)          && \
     ! __API_FLAGS_HAVE(flags, DEFAULT)      && \
     ! __API_FLAGS_HAVE(flags, GPU)          && \
     ! __API_FLAGS_HAVE(flags, ACCELERATOR)) || \
     ( __API_FLAGS_HAVE(flags, GPU)          && \
     ! __API_FLAGS_HAVE(flags, ALL)          && \
     ! __API_FLAGS_HAVE(flags, CPU)          && \
     ! __API_FLAGS_HAVE(flags, DEFAULT)      && \
     ! __API_FLAGS_HAVE(flags, ACCELERATOR)) || \
     ( __API_FLAGS_HAVE(flags, ACCELERATOR)  && \
     ! __API_FLAGS_HAVE(flags, ALL)          && \
     ! __API_FLAGS_HAVE(flags, CPU)          && \
     ! __API_FLAGS_HAVE(flags, GPU)          && \
     ! __API_FLAGS_HAVE(flags, DEFAULT)))

#define __API_DEV_CHECK_INDEX_FLAGS(flags)      \
   ((! __API_FLAGS_HAVE(flags, DEFAULT)      && \
     ! __API_FLAGS_HAVE(flags, FIRST)        && \
     ! __API_FLAGS_HAVE(flags, SECOND)       && \
     ! __API_FLAGS_HAVE(flags, THIRD)        && \
     ! __API_FLAGS_HAVE(flags, FOURTH)       && \
     ! __API_FLAGS_HAVE(flags, FIFTH)        && \
     ! __API_FLAGS_HAVE(flags, SIXTH)        && \
     ! __API_FLAGS_HAVE(flags, SEVENTH)      && \
     ! __API_FLAGS_HAVE(flags, EIGHTH))      || \
     ( __API_FLAGS_HAVE(flags, DEFAULT)      && \
     ! __API_FLAGS_HAVE(flags, FIRST)        && \
     ! __API_FLAGS_HAVE(flags, SECOND)       && \
     ! __API_FLAGS_HAVE(flags, THIRD)        && \
     ! __API_FLAGS_HAVE(flags, FOURTH)       && \
     ! __API_FLAGS_HAVE(flags, FIFTH)        && \
     ! __API_FLAGS_HAVE(flags, SIXTH)        && \
     ! __API_FLAGS_HAVE(flags, SEVENTH)      && \
     ! __API_FLAGS_HAVE(flags, EIGHTH))      || \
   ((! __API_FLAGS_HAVE(flags, DEFAULT))     && \
    ( __API_FLAGS_HAVE(flags, FIRST)         || \
      __API_FLAGS_HAVE(flags, SECOND)        || \
      __API_FLAGS_HAVE(flags, THIRD)         || \
      __API_FLAGS_HAVE(flags, FOURTH)        || \
      __API_FLAGS_HAVE(flags, FIFTH)         || \
      __API_FLAGS_HAVE(flags, SIXTH)         || \
      __API_FLAGS_HAVE(flags, SEVENTH)       || \
      __API_FLAGS_HAVE(flags, EIGHTH))))

#define __API_DEV_GET(id, device_info, value)                                 \
    HICL_ABORT(clGetDeviceInfo(id, device_info, sizeof(value), &value, NULL), \
               "failed to query device info")

#define __API_DEV_GET_PTR(id, device_info, value)                            \
    HICL_ABORT(clGetDeviceInfo(id, device_info, sizeof(value), value, NULL), \
               "failed to query device info")

#define __API_DEV_TYPE_STR(t)                                        \
    (t == CL_DEVICE_TYPE_ALL         ? "CL_DEVICE_TYPE_ALL"         :\
     t == CL_DEVICE_TYPE_CPU         ? "CL_DEVICE_TYPE_CPU"         :\
     t == CL_DEVICE_TYPE_GPU         ? "CL_DEVICE_TYPE_GPU"         :\
     t == CL_DEVICE_TYPE_ACCELERATOR ? "CL_DEVICE_TYPE_ACCELERATOR" :\
     t == CL_DEVICE_TYPE_DEFAULT     ? "CL_DEVICE_TYPE_DEFAULT"     :\
     "BAD DEVICE TYPE")

#define __API_DEV_INFO_LEVEL_0(fdout, fmt, ...) fprintf(fdout, \
HICL_GREEN"\n\to OpenCL "fmt"\n"HICL_END, ##__VA_ARGS__)

#ifdef __API_DEV_LONG_INFO_ENABLED
#define __API_DEV_INFO_LEVEL_1(fdout, fmt, ...) fprintf(fdout, \
"\t\to %-30s: "fmt"\n", ##__VA_ARGS__)

#define __API_DEV_INFO_LEVEL_2(fdout, fmt, ...) fprintf(fdout, \
"\t\t %32s "fmt"\n", " ", ##__VA_ARGS__)

/// \brief Convert OpenCl device global himem_t cache type to a string constant
#define __API_DEV_GLOBAL_MEM_CACHE_TYPE_STR(c)               \
    (c == CL_NONE             ? "CL_NONE"             :      \
     c == CL_READ_ONLY_CACHE  ? "CL_READ_ONLY_CACHE"  :      \
     c == CL_READ_WRITE_CACHE ? "CL_READ_WRITE_CACHE" :      \
      "BAD DEVICE GLOBAL CACHE TYPE")

/// \brief Convert OpenCl device local himem_t type to a string constant
#define __API_DEV_LOCAL_MEM_TYPE_STR(l) \
    (l == CL_NONE   ? "CL_NONE"     :   \
     l == CL_GLOBAL ? "CL_GLOBAL"   :   \
     l == CL_LOCAL  ? "CL_LOCAL"    :   \
     "BAD DEVICE LOCAL MEM TYPE")

#else
#define __API_DEV_INFO_LEVEL_1(fdout, fmt, ...) fprintf(fdout, \
"\t\to %-20s: "fmt"\n", ##__VA_ARGS__)

#define __API_DEV_INFO_LEVEL_2(fdout, fmt, ...) fprintf(fdout, \
"\t\t %22s "fmt"\n", " ", ##__VA_ARGS__)
#endif  // __API_DEV_LONG_INFO_ENABLED

PRIVATE cl_device_type
__api_dev_flags_to_type(flags_t flags) {
    if (__API_FLAGS_HAVE(flags, DEFAULT))     return CL_DEVICE_TYPE_DEFAULT;
    if (__API_FLAGS_HAVE(flags, CPU))         return CL_DEVICE_TYPE_CPU;
    if (__API_FLAGS_HAVE(flags, GPU))         return CL_DEVICE_TYPE_GPU;
    if (__API_FLAGS_HAVE(flags, ACCELERATOR)) return CL_DEVICE_TYPE_ACCELERATOR;
    if (__API_FLAGS_HAVE(flags, ALL))         return CL_DEVICE_TYPE_ALL;
    return CL_DEVICE_TYPE_DEFAULT;
}

PRIVATE cl_uint
__api_dev_flags_to_index(flags_t flags) {
    unsigned int log_value = -1, n = flags;
    if (__API_FLAGS_HAVE(flags, DEFAULT)) return 0;
    while (n) {
        log_value++;
        n >>= 1;
    }
    return log_value-1;
}

PRIVATE cl_uint
__api_dev_count(cl_platform_id plt) {
    cl_uint nb_devices;
    HICL_ABORT(clGetDeviceIDs(plt, CL_DEVICE_TYPE_ALL, 0, NULL, &nb_devices),
               "failed to query the number of OpenCL devices");
    return nb_devices;
}

PRIVATE void
__api_dev_query(cl_platform_id plt, cl_device_id *dev_ids, cl_uint nb_devices) {
    HICL_ABORT(clGetDeviceIDs(plt, 
                              CL_DEVICE_TYPE_ALL, nb_devices, dev_ids, NULL),
               "failed to query OpenCL devices");
}

PRIVATE cl_uint
__api_dev_select_by_type(cl_device_id *ids, cl_uint in_devices, flags_t flags) {
    cl_uint out_devices = 0;
    cl_device_id tmp_id;
    cl_device_type ftype, type;
    unsigned int f, i;
    flags_t hidev_types[]     = {ALL, CPU, GPU, ACCELERATOR};
    flags_t hidev_type_flags  = flags & (__API_DEV_TYPE_MASK);
    HICL_FAIL_IF(!__API_DEV_CHECK_TYPE_FLAGS(hidev_type_flags), 
                 "invalid hiCL device type flags");
    if(hidev_type_flags == 0) return in_devices;
    for (f = 0; f < sizeof(hidev_types)/sizeof(*hidev_types); ++f) {
        if (__API_FLAGS_HAVE(hidev_type_flags, hidev_types[f])) {
            ftype = __api_dev_flags_to_type(hidev_types[f]);
            for (i = out_devices; i < in_devices; ++i) {
                __API_DEV_GET(ids[i], CL_DEVICE_TYPE, type);
                if (type & ftype) {
                    tmp_id             = ids[out_devices];
                    ids[out_devices++] = ids[i];
                    ids[i]             = tmp_id;
                }
            }
        }
    }
    return out_devices;
}

PRIVATE cl_uint
__api_dev_select_by_index(cl_device_id *ids, 
                          cl_uint in_devices, flags_t flags) {
    cl_uint f, i, findex, out_devices = 0;
    cl_device_id tmp_ids[in_devices];
    flags_t dev_indices[]   = {FIRST, SECOND,  THIRD, FOURTH,
                               FIFTH, SIXTH, SEVENTH, EIGHTH};
    flags_t dev_index_flags = flags & (__API_DEV_INDEX_MASK);
    HICL_FAIL_IF(!__API_DEV_CHECK_INDEX_FLAGS(dev_index_flags), 
                 "invalid hiCL device index flags");
    for (f = 0; f < sizeof(dev_indices)/sizeof(*dev_indices); ++f) {
        if (__API_FLAGS_HAVE(dev_index_flags, dev_indices[f])) {
            findex = __api_dev_flags_to_index(dev_indices[f]);
            HICL_FAIL_IF(findex > in_devices,
                         "OpenCL device index (%u) out of bound (%u)",
                         findex, in_devices);
            tmp_ids[out_devices++] = ids[findex];
        }
    }
    for (i = 0; i < out_devices; ++i) ids[i] = tmp_ids[i];
    return out_devices ? out_devices : in_devices;
}

PRIVATE cl_uint
__api_dev_select(cl_device_id *ids, cl_uint nb_devices, flags_t flags) {
    cl_uint n = 0;
    flags_t hidev_type_flags  = flags & (__API_DEV_TYPE_MASK);
    flags_t dev_index_flags = flags & (__API_DEV_INDEX_MASK);
    if ((__API_FLAGS_HAVE(flags, DEFAULT)) ||
        ((hidev_type_flags == 0) && (dev_index_flags == 0))) return 1;
    if ((n = __api_dev_select_by_type(ids, nb_devices, hidev_type_flags)) == 0) {
        return 0;
    } else {
        return __api_dev_select_by_index(ids, n, dev_index_flags);
    }
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
__api_dev_single_fp_config_str(cl_device_fp_config fc, FILE* fdout) {
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "execution capabilities",
                           fc & CL_FP_ROUND_TO_NEAREST ?
                           "CL_FP_ROUND_TO_NEAREST" : " ");
    if (fc & CL_FP_DENORM)
        __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_FP_DENORM");
    if (fc & CL_FP_INF_NAN)
        __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_FP_INF_NAN");
    if (fc & CL_FP_ROUND_TO_ZERO)
        __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_FP_ROUND_TO_ZERO");
    if (fc & CL_FP_ROUND_TO_INF)
        __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_FP_ROUND_TO_INF");
    if (fc & CL_FP_FMA)
        __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_FP_FMA");
    if (fc & CL_FP_SOFT_FLOAT)
        __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_FP_SOFT_FLOAT");
#ifdef CL_VERSION_1_2
    if (fc & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT)
        __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT");
#endif  // CL_VERSION_1_2
}

/// \brief Convert OpenCl device single fp config value to a string constant
PRIVATE void 
__api_dev_execution_capabilities_str(cl_device_exec_capabilities ec, 
                                     FILE* fdout) {
    if (ec & CL_EXEC_KERNEL) {
        __API_DEV_INFO_LEVEL_1(fdout, 
                               "%s", "exec capabilities", "CL_EXEC_KERNEL");
        if (ec & CL_EXEC_NATIVE_KERNEL)
            __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_EXEC_NATIVE_KERNEL");
    } else {
        if (ec & CL_EXEC_NATIVE_KERNEL)
            __API_DEV_INFO_LEVEL_1(fdout, "%s", "exec capabilities",
                                   "CL_EXEC_NATIVE_KERNEL");
    }
}

/// \brief Convert OpenCl device single fp config value to a string constant
PRIVATE void 
__api_dev_queue_properties_str(cl_command_queue_properties qp, FILE* fdout) {
    if (qp & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) {
        __API_DEV_INFO_LEVEL_1(fdout, "%s", "queue properties",
                               "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE");
        if (qp & CL_QUEUE_PROFILING_ENABLE)
            __API_DEV_INFO_LEVEL_2(fdout, "%s", "CL_QUEUE_PROFILING_ENABLE");

    } else {
        if (qp & CL_QUEUE_PROFILING_ENABLE)
            __API_DEV_INFO_LEVEL_1(fdout, "%s", "queue properties",
                                   "CL_QUEUE_PROFILING_ENABLE");
    }
}
#endif  // __API_DEV_LONG_INFO_ENABLED

PRIVATE void 
__api_dev_info(void *pointer) {
    hidev_t     d = (hidev_t)pointer;
    FILE* fdout   = (hicl == NULL) ? stdout : hicl->fdout;
    cl_device_type device_type;
    char name[__API_STR_SIZE];
    char vendor[__API_STR_SIZE];
    char driver_version[__API_STR_SIZE];
    char profile[__API_STR_SIZE];
    char extensions[__API_BUFFER_SIZE];
    char *marker;
    __API_DEV_GET(d->id, CL_DEVICE_TYPE, device_type);
    __API_DEV_GET_PTR(d->id, CL_DEVICE_NAME, name);
    __API_DEV_GET_PTR(d->id, CL_DEVICE_VENDOR, vendor);
    __API_DEV_GET_PTR(d->id, CL_DRIVER_VERSION, driver_version);
    __API_DEV_GET_PTR(d->id, CL_DEVICE_PROFILE, profile);
    __API_DEV_GET_PTR(d->id, CL_DEVICE_EXTENSIONS, extensions);
    __API_DEV_INFO_LEVEL_0(fdout, "Device @ %p", d->id);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "type",
                              __API_DEV_TYPE_STR(device_type));
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "name", name);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "vendor", vendor);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "driver version", driver_version);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "profile", profile);
    if (strlen(extensions) != 0) {
        char ext[__API_STR_SIZE];
        marker = __api_strstep(ext, extensions, " ");
        __API_DEV_INFO_LEVEL_1(fdout, "%s", "extensions", ext);
        do {
            marker = __api_strstep(ext, marker, " ");
            __API_DEV_INFO_LEVEL_2(fdout, "%s", ext);
        } while(marker != NULL);
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
    cl_uint address_t_bits;
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

    __API_DEV_GET_PTR(d->id, CL_DEVICE_VERSION,
                         device_version);
    __API_DEV_GET(d->id, CL_DEVICE_VENDOR_ID,
                     vendor_id);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_COMPUTE_UNITS,
                     max_compute_units);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                     max_work_item_dimensions);
    __API_DEV_GET_PTR(d->id, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                         max_work_item_sizes);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_WORK_GROUP_SIZE,
                     max_work_group_size);
    __API_DEV_GET(d->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
                     preferred_vector_width_char);
    __API_DEV_GET(d->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
                     preferred_vector_width_short);
    __API_DEV_GET(d->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
                     preferred_vector_width_int);
    __API_DEV_GET(d->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
                     preferred_vector_width_long);
    __API_DEV_GET(d->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
                     preferred_vector_width_float);
    __API_DEV_GET(d->id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
                     preferred_vector_width_double);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_CLOCK_FREQUENCY,
                     max_clock_frequency);
    __API_DEV_GET(d->id, CL_DEVICE_ADDRESS_BITS,
                     address_t_bits);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                     max_mem_alloc_size);
    __API_DEV_GET(d->id, CL_DEVICE_IMAGE_SUPPORT,
                     image_support);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_READ_IMAGE_ARGS,
                     max_read_image_args);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
                     max_write_image_args);
    __API_DEV_GET(d->id, CL_DEVICE_IMAGE2D_MAX_WIDTH,
                     image2d_max_width);
    __API_DEV_GET(d->id, CL_DEVICE_IMAGE2D_MAX_HEIGHT,
                     image2d_max_height);
    __API_DEV_GET(d->id, CL_DEVICE_IMAGE3D_MAX_WIDTH,
                     image3d_max_width);
    __API_DEV_GET(d->id, CL_DEVICE_IMAGE3D_MAX_HEIGHT,
                     image3d_max_height);
    __API_DEV_GET(d->id, CL_DEVICE_IMAGE3D_MAX_DEPTH,
                     image3d_max_depth);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_SAMPLERS,
                     max_samplers);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_PARAMETER_SIZE,
                     max_parameter_size);
    __API_DEV_GET(d->id, CL_DEVICE_MEM_BASE_ADDR_ALIGN,
                     mem_base_addr_align);
    __API_DEV_GET(d->id, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
                     min_data_type_align_size);
    __API_DEV_GET(d->id, CL_DEVICE_SINGLE_FP_CONFIG,
                     single_fp_config);
    __API_DEV_GET(d->id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                     global_mem_cache_type);
    __API_DEV_GET(d->id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                     global_mem_cache_size);
    __API_DEV_GET(d->id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                     global_mem_cacheline_size);
    __API_DEV_GET(d->id, CL_DEVICE_GLOBAL_MEM_SIZE,
                     global_mem_size);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
                     max_constant_buffer_size);
    __API_DEV_GET(d->id, CL_DEVICE_MAX_CONSTANT_ARGS,
                     max_constant_args);
    __API_DEV_GET(d->id, CL_DEVICE_LOCAL_MEM_TYPE,
                     local_mem_type);
    __API_DEV_GET(d->id, CL_DEVICE_LOCAL_MEM_SIZE,
                     local_mem_size);
    __API_DEV_GET(d->id, CL_DEVICE_ERROR_CORRECTION_SUPPORT,
                     error_correction_support);
    __API_DEV_GET(d->id, CL_DEVICE_PROFILING_TIMER_RESOLUTION,
                     profiling_timer_resolution);
    __API_DEV_GET(d->id, CL_DEVICE_ENDIAN_LITTLE,
                     endian_little);
    __API_DEV_GET(d->id, CL_DEVICE_EXECUTION_CAPABILITIES,
                     execution_capabilities);
    __API_DEV_GET(d->id, CL_DEVICE_QUEUE_PROPERTIES,
                     queue_properties);
    __API_DEV_GET(d->id, CL_DEVICE_PLATFORM,
                     platform_id);

    __API_DEV_INFO_LEVEL_1(fdout, "%s", "device version",
                              device_version);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "vendor id",
                              vendor_id);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "max compute units",
                              max_compute_units);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "max work item dimensions",
                              max_work_item_dimensions);
    __API_DEV_INFO_LEVEL_1(fdout, "dimension[0] = %ld", "max work sizes",
                              max_work_item_sizes[0]);
    for (i = 1; i < sizeof(max_work_item_sizes)/sizeof(size_t); ++i)
        __API_DEV_INFO_LEVEL_2(fdout, "dimension[%d] = %ld",
                                  i, max_work_item_sizes[i]);
    __API_DEV_INFO_LEVEL_1(fdout, "%lu", "max work group size",
                              max_work_group_size);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "preferred vector width char",
                              preferred_vector_width_char);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "preferred vector width short",
                              preferred_vector_width_short);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "preferred vector width int",
                              preferred_vector_width_int);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "preferred vector width long",
                              preferred_vector_width_long);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "preferred vector width float",
                              preferred_vector_width_float);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "preferred vector width double",
                              preferred_vector_width_double);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "max clock frequency",
                              max_clock_frequency);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "address bits",
                              address_t_bits);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "image support",
                              image_support ? "yes" : "no");
    if (image_support) {
        __API_DEV_INFO_LEVEL_1(fdout, "%u", "max read image args",
                                  max_read_image_args);
        __API_DEV_INFO_LEVEL_1(fdout, "%u", "max write image args",
                                  max_write_image_args);
        __API_DEV_INFO_LEVEL_1(fdout, "%lu", "image2d max width",
                                  image2d_max_width);
        __API_DEV_INFO_LEVEL_1(fdout, "%lu", "image2d max height",
                                  image2d_max_height);
        __API_DEV_INFO_LEVEL_1(fdout, "%lu", "image3d max width",
                                  image3d_max_width);
        __API_DEV_INFO_LEVEL_1(fdout, "%lu", "image3d max height",
                                  image3d_max_height);
        __API_DEV_INFO_LEVEL_1(fdout, "%lu", "image3d max depth",
                                  image3d_max_depth);
        __API_DEV_INFO_LEVEL_1(fdout, "%u", "max samplers",
                                  max_samplers);
    }
    __API_DEV_INFO_LEVEL_1(fdout, "%ld", "max paramater size",
                              max_parameter_size);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "mem base addr align",
                              mem_base_addr_align);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "min data type align size",
                              min_data_type_align_size);
    __API_DEV_INFO_LEVEL_1(fdout, "", "single fp cpnfig");
    __api_dev_single_fp_config_str(single_fp_config, fdout);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "global mem cache type",
          __API_DEV_GLOBAL_MEM_CACHE_TYPE_STR(global_mem_cache_type));
    __API_DEV_INFO_LEVEL_1(fdout, "%u Bytes", "global mem cacheline size",
                              global_mem_cacheline_size);
    __API_DEV_INFO_LEVEL_1(fdout, "%lu"" Bytes", "global mem cache size",
                              global_mem_cache_size);
    __API_DEV_INFO_LEVEL_1(fdout, "%lu"" MB", "global mem size",
                              global_mem_size/1024/1024);
    __API_DEV_INFO_LEVEL_1(fdout, "%lu"" MB", "global mem max alloc",
			               max_mem_alloc_size/1024/1024);	
    __API_DEV_INFO_LEVEL_1(fdout, "%lu"" KB", "max constant buffer size",
                              max_constant_buffer_size/1024);
    __API_DEV_INFO_LEVEL_1(fdout, "%u", "max constant args",
                              max_constant_args);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "local mem type",
                              __API_DEV_LOCAL_MEM_TYPE_STR(local_mem_type));
    __API_DEV_INFO_LEVEL_1(fdout, "%lu"" KB", "local mem size",
                              local_mem_size/1024);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "error correction support",
                              error_correction_support ? "yes" : "no");
    __API_DEV_INFO_LEVEL_1(fdout, "%lu", "profiling timer resolution",
                              profiling_timer_resolution);
    __API_DEV_INFO_LEVEL_1(fdout, "%s", "endian little",
                              endian_little ? "yes" : "no");
    __api_dev_execution_capabilities_str(execution_capabilities, fdout);
#endif  // _CL_DEVICE_LONG_INFO_ENABLED
}

///
/// @brief Initialize a device descriptor.
///
/// This routine creates a hiCL device descriptor, provided an OpenCL device id,
/// which contains an OpenCL command queue that is associated to the device id,
/// see types.h.
/// @param id the OpenCL device id of the descriptor to create.
/// @return A hiCL device descriptor.
///
PRIVATE hidev_t  __api_dev_init(cl_device_id id) {
    cl_int cl_ret;
    hidev_t d    = (hidev_t)malloc(sizeof(struct __hidev_t));
    d->id    = id; 
    d->queue = clCreateCommandQueue(hicl->context, id,
                                    __API_USE_EVENTS ?
                                    CL_QUEUE_PROFILING_ENABLE : 0, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL queue");
    return d;
}

PRIVATE void
__api_dev_release(void *pointer) {
    hidev_t     d = (hidev_t)pointer;
    if (d != NULL) {
        HICL_DEBUG("releasing OpenCL device @ %p", d->id);
        if (clReleaseCommandQueue(d->queue) != CL_SUCCESS) 
            HICL_FAIL("failed to release OpenCL command queue");
        free(d); d = NULL;
    }
}

CPPGUARD_END();

#endif  // __API_DEV_INL_H_
