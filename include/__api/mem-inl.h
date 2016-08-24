#ifndef __API_MEM_INL_H_
#define __API_MEM_INL_H_
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
/// @file __api/mem-inl.h
/// @author Issam SAID
/// @brief Private functions used to implement the hiCL memory descriptor.
///
#include <stdio.h>
#include <hiCL/flags.h>
#include <hiCL/types.h>
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/util-inl.h"
#include "__api/knl-inl.h"

CPPGUARD_BEGIN();

#define CL_MEMORY_GET(id, mem_info, value)                                  \
    HICL_CHECK(clGetMemObjectInfo(id, mem_info, sizeof(value), &value, NULL), \
             "failed to query memory info")

#define CL_MEMORY_GET_PTR(id, kernel_info, value)                         \
    HICL_CHECK(clGetMemObjectInfo(id, mem_info, sizeof(value), value, NULL),\
             "failed to query memory info")

#define __API_MEM_INFO_LEVEL_0(fmt, ...) \
    fprintf(hicl->fdout, C_GREEN"\no OpenCL "fmt":\n"C_END, ##__VA_ARGS__)

#define __API_MEM_INFO_LEVEL_1(fmt, ...) \
    fprintf(hicl->fdout, "\to %-20s: "fmt"\n", ##__VA_ARGS__)

#define __API_MEM_INFO_LEVEL_2(fmt, ...) \
    fprintf(hicl->fdout, "\t %-22s "fmt"\n", " ", ##__VA_ARGS__)

#define __API_MEM_READ_ONLY(flags)        (flags & READ_ONLY)
#define __API_MEM_WRITE_ONLY(flags)       (flags & WRITE_ONLY)
#define __API_MEM_READ_WRITE(flags)       (flags & READ_WRITE)
#define __API_MEM_READ_WRITE_OR_WRITE_ONLY(flags) \
    (flags & (READ_WRITE | WRITE_ONLY))

#define __API_MEM_CPU(flags)              (flags & CPU)
#define __API_MEM_HWA(flags)              (flags & HWA)
#define __API_MEM_PINNED(flags)           (flags & PINNED)
#define __API_MEM_ZERO_COPY(flags)        (flags & ZERO_COPY)
#define __API_MEM_HOST_ALLOCATED(flags)   (flags & HOST_ALLOCATED)

#define __API_MEM_CHAR(flags)             (flags & CHAR)
#define __API_MEM_INT(flags)              (flags & INT)
#define __API_MEM_UNSIGNED_INT(flags)     (flags & UNSIGNED_INT)
#define __API_MEM_LONG(flags)             (flags & LONG)
#define __API_MEM_UNSIGNED_LONG(flags)    (flags & UNSIGNED_LONG)
#define __API_MEM_SIZET(flags)            (flags & SIZET)
#define __API_MEM_FLOAT(flags)            (flags & FLOAT)
#define __API_MEM_DOUBLE(flags)           (flags & DOUBLE)

#define __API_MEM_HOST_DIRTY(flags)       (flags & HOST_DIRTY)
#define __API_MEM_DEVICE_DIRTY(flags)     (flags & DEVICE_DIRTY)

#define __API_MEM_PRINT_FLAGS(flags)                             \
    __API_MEM_INFO_LEVEL_1("%s %s | %s | %s", "flags",           \
    __API_MEM_CPU(flags)              ? "CPU":                   \
    __API_MEM_HWA(flags)              ? "HWA": "UNKNOW",         \
    __API_MEM_PINNED(flags)           ? "| PINNED":              \
    __API_MEM_ZERO_COPY(flags)        ? "| ZERO_COPY":           \
    __API_MEM_HOST_ALLOCATED(flags)   ? "| HOST_ALLOCATED" : "", \
    __API_MEM_READ_WRITE(flags)       ? "READ_WRITE":            \
    __API_MEM_READ_ONLY(flags)        ? "READ_ONLY":             \
    __API_MEM_WRITE_ONLY(flags)       ? "WRITE_ONLY": "UNKNOW",  \
    __API_MEM_CHAR(flags)             ? "CHAR":                  \
    __API_MEM_INT(flags)              ? "INT":                   \
    __API_MEM_UNSIGNED_INT(flags)     ? "UNSIGNED INT" :         \
    __API_MEM_LONG(flags)             ? "LONG" :                 \
    __API_MEM_UNSIGNED_LONG(flags)    ? "UNSIGNED LONG" :        \
    __API_MEM_SIZET(flags)            ? "SIZET"  :               \
    __API_MEM_FLOAT(flags)            ? "FLOAT"  :               \
    __API_MEM_DOUBLE(flags)           ? "DOUBLE" : "UNKNOWN")

#define __API_MEM_SET_DEFAULTS(flags)         \
    if ((!__API_MEM_CPU(flags))           &&  \
        (!__API_MEM_HWA(flags))           &&  \
        (!__API_MEM_PINNED(flags))        &&  \
        (!__API_MEM_ZERO_COPY(flags)))        \
        flags |= HWA;                         \
    if ((!__API_MEM_READ_ONLY(flags))     &&  \
        (!__API_MEM_WRITE_ONLY(flags))    &&  \
        (!__API_MEM_READ_WRITE(flags)))       \
        flags |= READ_WRITE;                  \
    if ((!__API_MEM_CHAR(flags))          &&  \
        (!__API_MEM_INT(flags))           &&  \
        (!__API_MEM_UNSIGNED_INT(flags))  &&  \
        (!__API_MEM_LONG(flags))          &&  \
        (!__API_MEM_UNSIGNED_LONG(flags)) &&  \
        (!__API_MEM_SIZET(flags))         &&  \
        (!__API_MEM_FLOAT(flags))         &&  \
        (!__API_MEM_DOUBLE(flags)))           \
        flags |= FLOAT

#define CLEAR_HOST                      0xFFFFFFFFFEFFFFFF
#define CLEAR_DEVICE                    0xFFFFFFFFFDFFFFFF
#define CLEAR                           0xFFFFFFFFFCFFFFFF

#define __API_MEM_CLEAR_HOST(flags)     (flags &= CLEAR_HOST)
#define __API_MEM_CLEAR_DEVICE(flags)   (flags &= CLEAR_DEVICE)
#define __API_MEM_CLEAR(flags)          (flags &= CLEAR)

#define __API_MEM_TOUCH_HOST(flags)     (flags |= HOST_DIRTY)
#define __API_MEM_TOUCH_DEVICE(flags)   (flags |= DEVICE_DIRTY)

PRIVATE size_t
__api_mem_unit_size(flags_t flags) {
    if (flags & FLOAT)         return sizeof(float);
    if (flags & DOUBLE)        return sizeof(double);
    if (flags & INT)           return sizeof(int);
    if (flags & UNSIGNED_INT)  return sizeof(unsigned int);
    if (flags & LONG)          return sizeof(long);
    if (flags & UNSIGNED_LONG) return sizeof(unsigned long);
    if (flags & SIZET)         return sizeof(size_t);
    if (flags & CHAR)          return sizeof(char);
    HICL_EXIT("failed to get memory unitary size");
}

PRIVATE cl_mem_flags
__api_mem_alloc_flags(flags_t flags) {
    cl_mem_flags mem_flags = 0;
    if (__API_MEM_CPU(flags)) {
       mem_flags |= 
          (flags & ZERO_COPY)   ? CL_MEM_ALLOC_HOST_PTR : 0; 
    }
    if (__API_MEM_HWA(flags)) {
      mem_flags |= 
        (flags & PINNED)    ? CL_MEM_ALLOC_HOST_PTR         :
        (flags & ZERO_COPY) ? CL_MEM_USE_PERSISTENT_MEM_AMD : 0; /// FIXME AMD ONLY
    }
    return mem_flags;
}

PRIVATE cl_mem_flags
__api_mem_update_flags(flags_t flags) {
    cl_mem_flags mem_flags = 0;
    mem_flags |= 
        (flags & READ_WRITE) ? CL_MEM_READ_WRITE :
        (flags & READ_ONLY)  ? CL_MEM_READ_ONLY  :
        (flags & WRITE_ONLY) ? CL_MEM_WRITE_ONLY : 0;
    return mem_flags;
}

PRIVATE cl_mem_flags
__api_mem_map_flags(flags_t flags, cl_bool reverse) {
    cl_mem_flags mem_flags = 0;
    mem_flags |= 
        (flags & WRITE_ONLY) ? (reverse ? CL_MAP_READ  : CL_MAP_WRITE) :
        (flags & READ_ONLY)  ? (reverse ? CL_MAP_WRITE :  CL_MAP_READ) :
        (flags & READ_WRITE) ? CL_MAP_READ | CL_MAP_WRITE : 0;
    return mem_flags;
}

PRIVATE void
__api_mem_map(himem_t m, cl_map_flags flags, cl_bool blocking) {
    cl_int cl_ret;
    HICL_DEBUG("mem map    {h=%p, id=%p} %s(size=%12.5f MB)",
               m->h,
               __API_MEM_PINNED(m->flags) ? m->pinned : m->id,
               __API_MEM_PINNED(m->flags) ? "pinned " : "",
               (double)m->size*m->unit_size/1024./1024.);
    m->h = clEnqueueMapBuffer(m->queue, 
                              __API_MEM_PINNED(m->flags) ? m->pinned : m->id,
                              blocking,
                              flags, 0, m->size*m->unit_size, 0,
                              NULL, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to map memory region");
}

PRIVATE void
__api_mem_unmap(himem_t m, cl_bool blocking) {
    HICL_DEBUG("mem unmap  {h=%p, id=%p} (size=%12.5f MB)", 
               m->h, m->id, (double)m->size*m->unit_size/1024./1024.);
    HICL_CHECK(clEnqueueUnmapMemObject(m->queue,
                                       __API_MEM_PINNED(m->flags) ? 
                                       m->pinned : m->id, 
                                       m->h, 0, NULL, NULL),
             "failed to unmap memory region");
    if (blocking) clFinish(m->queue);
}

PRIVATE void
__api_mem_htod(himem_t m, cl_bool blocking) {
    HICL_CHECK(clEnqueueWriteBuffer(m->queue, m->id, blocking, 0,
                                  m->size*m->unit_size, m->h, 0, NULL, NULL),
             "failed to write into OpenCL memory");
    HICL_DEBUG("mem htod   {h=%p, id=%p} (size=%12.5f MB)", 
               m->h, m->id, (double)m->size*m->unit_size/1024./1024.);
}

PRIVATE void 
__api_mem_dtoh(himem_t m, cl_bool blocking) { 
    HICL_CHECK(clEnqueueReadBuffer(m->queue, m->id, blocking, 0,
                                 m->size*m->unit_size, m->h, 0, NULL, NULL),
             "failed to read from OpenCL memory");
    HICL_DEBUG("mem dtoh   {h=%p, id=%p} (size=%12.5f MB)", 
               m->h, m->id, (double)m->size*m->unit_size/1024./1024.);
}

/// @todo This is bugy on AMD discrete GPU 
PRIVATE void 
__api_mem_dtoh_rect3d(himem_t m, 
                      int ix, int ex,
                      int iy, int ey,
                      int iz, int ez,
                      int xpitch, int ypitch, 
                      cl_bool blocking) {
    size_t origin[3] = {ix*m->unit_size, (size_t)iy, (size_t)iz};  
    size_t region[3] = {(ex-ix+1)*m->unit_size, 
                        (size_t)(ey-iy+1), (size_t)(ez-iz+1)};
    HICL_CHECK(clEnqueueReadBufferRect(m->queue, m->id, blocking,
                                     origin,
                                     origin,
                                     region, 
                                     xpitch*m->unit_size, 
                                     xpitch*ypitch*m->unit_size, 
                                     xpitch*m->unit_size, 
                                     xpitch*ypitch*m->unit_size, 
                                     m->h, 0, NULL, NULL),
             "failed to read rectangle from OpenCL memory");
    HICL_DEBUG("mem dtoh_rect3d {h=%p, id=%p} (size=%12.5f MB)", 
               m->h, m->id, (double)region[0]*region[1]*region[2]/1024./1024.);
}

/// @todo This is bugy on AMD discrete GPU 
PRIVATE void 
__api_mem_htod_rect3d(himem_t m, 
                      int ix, int ex,
                      int iy, int ey,
                      int iz, int ez,
                      int xpitch, int ypitch, 
                      cl_bool blocking) {
    size_t origin[3] = {ix*m->unit_size, (size_t)iy, (size_t)iz};  
    size_t region[3] = {(ex-ix+1)*m->unit_size,
                        (size_t)(ey-iy+1), (size_t)(ez-iz+1)};
    HICL_CHECK(clEnqueueWriteBufferRect(m->queue, m->id, blocking,
                                      origin,
                                      origin,
                                      region, 
                                      xpitch*m->unit_size, 
                                      xpitch*ypitch*m->unit_size, 
                                      xpitch*m->unit_size, 
                                      xpitch*ypitch*m->unit_size, 
                                      m->h, 0, NULL, NULL),
             "failed to write rectangle from OpenCL memory");
    HICL_DEBUG("mem htod_rect3d {h=%p, id=%p} (size=%12.5f MB)", 
               m->h, m->id, (double)region[0]*region[1]*region[2]/1024./1024.);
}

PRIVATE void
__api_mem_copy(himem_t src, himem_t dst, cl_bool blocking) {
    if (__API_MEM_ZERO_COPY(src->flags)) {
        dst->flags     = src->flags;
        dst->h         = src->h;
        dst->id        = src->id;
        dst->size      = src->size;
        dst->unit_size = src->unit_size;
    } else {
        if ((__API_MEM_HOST_DIRTY(src->flags)) &&
            (! __API_MEM_READ_ONLY(src->flags))) {
            __api_mem_htod(src, blocking);
        }
        HICL_CHECK(clEnqueueCopyBuffer(dst->queue, src->id, dst->id, 0, 0,
                                     src->size*src->unit_size, 0, NULL, NULL),
                 "failed to copy OpenCL memory");
        __API_MEM_TOUCH_DEVICE(dst->flags);
        if (blocking) clFinish(dst->queue);
    }
    HICL_DEBUG("mem copy from {h=%p, id=%p} to {h=%p, id=%p} (size=%12.5f MB)", 
               src->h, src->id, dst->h, dst->id, 
               (double)src->size*src->unit_size/1024./1024.);
}

PRIVATE void
__api_mem_touch(himem_t m) {
    if (!__API_MEM_READ_ONLY(m->flags)) {
        __API_MEM_TOUCH_DEVICE(m->flags);
        HICL_DEBUG("mem touch  {h=%p, id=%p}", m->h, m->id);
    }
}

PRIVATE void
__api_mem_sync(himem_t m) {
    if (__API_MEM_ZERO_COPY(m->flags)) {
        if (__API_MEM_HOST_DIRTY(m->flags) &&
            !__API_MEM_WRITE_ONLY(m->flags)) {
            __api_mem_unmap(m, CL_TRUE);
            __API_MEM_CLEAR_HOST(m->flags);
            if (__API_MEM_READ_ONLY(m->flags)) 
                __api_mem_map(m,
                              __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
        }
    } else {
        if (__API_MEM_HOST_DIRTY(m->flags) && 
            !__API_MEM_WRITE_ONLY(m->flags)) {
            __api_mem_htod(m, CL_FALSE);
            __API_MEM_CLEAR_HOST(m->flags);
        }
    }
    __api_mem_touch(m);
}

PRIVATE himem_t
__api_mem_find(address_t h) {
    return find_rbn_address_t_himem_t(&hicl->mems, h)->value;
}

PRIVATE void
__api_mem_release(himem_t m) {
    if (m != NULL) {
        HICL_DEBUG("mem release {h=%p, id=%p} (knl count = %lu)", 
                    m->h, m->id, m->knls.size);
        if (__API_MEM_CPU(m->flags)) {
            if (__API_MEM_ZERO_COPY(m->flags)) {
                HICL_ABORT(clEnqueueUnmapMemObject(m->queue,
                                                     __API_MEM_PINNED(m->flags) ? 
                                                     m->pinned : m->id, 
                                                     m->h, 0, NULL, NULL), 
                           "failed to unmap host zero-copy memory");
                HICL_ABORT(clReleaseMemObject(m->id),
                           "failed to release OpenCL memory");
            } else {
                free(m->h);
            }
        } else if (__API_MEM_HWA(m->flags)) {
            if (__API_MEM_PINNED(m->flags)) {
                HICL_ABORT(clReleaseMemObject(m->pinned),
                           "failed to release pinned memory object");
            } else if (__API_MEM_ZERO_COPY(m->flags)) {
                __api_mem_unmap(m, CL_TRUE);
            } else {
                if (!__API_MEM_HOST_ALLOCATED(m->flags)) {
                    free(m->h);
                }
            }
            HICL_ABORT(clReleaseMemObject(m->id), 
                       "failed to release OpenCL memory");
        }
        free(m); m = NULL;
    }
}

PRIVATE void
__api_mem_skip_from_knl(hiknl_t k, int index) {
    HICL_DEBUG("skip arg %d of kernel %s", index, __api_knl_name(k->id));
    skip_rbn_int_himem_t(&k->mems, index);
}

PRIVATE void
__api_mem_stdalone_release(himem_t m) {
    if (m != NULL) {
        walk_key_value_rbt_hiknl_t_int(&m->knls, __api_mem_skip_from_knl);
        delete_rbt_hiknl_t_int(&m->knls);
        __api_mem_release(m);
    }
}

PRIVATE void
__api_mem_knl_release(himem_t m) {
    if (m != NULL) {
        walk_key_value_rbt_hiknl_t_int(&m->knls, __api_mem_skip_from_knl);
        delete_rbt_hiknl_t_int(&m->knls);
        skip_rbn_address_t_himem_t(&hicl->mems, m->h);
        __api_mem_release(m);
    }
}

PRIVATE void
__api_mem_info(void *pointer) {
    himem_t m = (himem_t)pointer;
    __API_MEM_INFO_LEVEL_0("memory object {h=%p, id=%p} (flags=%#lx)", 
                            m->h, m->id, m->flags);
    __API_MEM_INFO_LEVEL_1("%-12.5f %s", "size", 
                           (double)m->size*m->unit_size/1024./1024., "MB");
    __API_MEM_PRINT_FLAGS(m->flags);
}

CPPGUARD_END();

#endif  // __API_MEM_INL_H_