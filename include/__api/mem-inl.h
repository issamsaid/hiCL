#ifndef __API_MEM_INL_H_
#define __API_MEM_INL_H_
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

CPPGUARD_BEGIN();

#define __API_MEM_GET(id, mem_info, value)                                    \
    HICL_CHECK(clGetMemObjectInfo(id, mem_info, sizeof(value), &value, NULL), \
             "failed to query memory info")

#define __API_MEM_GET_PTR(id, kernel_info, value)                           \
    HICL_CHECK(clGetMemObjectInfo(id, mem_info, sizeof(value), value, NULL),\
             "failed to query memory info")

#define __API_MEM_INFO_LEVEL_0(fmt, ...) \
    fprintf(hicl->fdout, HICL_GREEN"\no OpenCL "fmt":\n"HICL_END, ##__VA_ARGS__)

#define __API_MEM_INFO_LEVEL_1(fmt, ...) \
    fprintf(hicl->fdout, "\to %-20s: "fmt"\n", ##__VA_ARGS__)

#define __API_MEM_INFO_LEVEL_2(fmt, ...) \
    fprintf(hicl->fdout, "\t %-22s "fmt"\n", " ", ##__VA_ARGS__)

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

#define __API_MEM_PRINT_FLAGS(flags)                                     \
    __API_MEM_INFO_LEVEL_1("%s %s | %s | %s", "flags",                   \
    __API_FLAGS_HAVE(flags, CPU)              ? "CPU":                   \
    __API_FLAGS_HAVE(flags, HWA)              ? "HWA": "UNKNOW",         \
    __API_FLAGS_HAVE(flags, PINNED)           ? "| PINNED":              \
    __API_FLAGS_HAVE(flags, ZERO_COPY)        ? "| ZERO_COPY":           \
    __API_FLAGS_HAVE(flags, HOST_ALLOCATED)   ? "| HOST_ALLOCATED" : "", \
    __API_FLAGS_HAVE(flags, READ_WRITE)       ? "READ_WRITE":            \
    __API_FLAGS_HAVE(flags, READ_ONLY)        ? "READ_ONLY":             \
    __API_FLAGS_HAVE(flags, WRITE_ONLY)       ? "WRITE_ONLY": "UNKNOW",  \
    __API_FLAGS_HAVE(flags, CHAR)             ? "CHAR":                  \
    __API_FLAGS_HAVE(flags, INT)              ? "INT":                   \
    __API_FLAGS_HAVE(flags, UNSIGNED_INT)     ? "UNSIGNED INT" :         \
    __API_FLAGS_HAVE(flags, LONG)             ? "LONG" :                 \
    __API_FLAGS_HAVE(flags, UNSIGNED_LONG)    ? "UNSIGNED LONG" :        \
    __API_FLAGS_HAVE(flags, SIZET)            ? "SIZET"  :               \
    __API_FLAGS_HAVE(flags, FLOAT)            ? "FLOAT"  :               \
    __API_FLAGS_HAVE(flags, DOUBLE)           ? "DOUBLE" : "UNKNOWN")

#define __API_MEM_SET_DEFAULTS(flags)         \
    if ((!__API_FLAGS_HAVE(flags, CPU))           &&  \
        (!__API_FLAGS_HAVE(flags, HWA))           &&  \
        (!__API_FLAGS_HAVE(flags, PINNED))        &&  \
        (!__API_FLAGS_HAVE(flags, ZERO_COPY)))        \
        flags |= HWA;                         \
    if ((!__API_FLAGS_HAVE(flags, READ_ONLY))     &&  \
        (!__API_FLAGS_HAVE(flags, WRITE_ONLY))    &&  \
        (!__API_FLAGS_HAVE(flags, READ_WRITE)))       \
        flags |= READ_WRITE;                  \
    if ((!__API_FLAGS_HAVE(flags, CHAR))          &&  \
        (!__API_FLAGS_HAVE(flags, INT))           &&  \
        (!__API_FLAGS_HAVE(flags, UNSIGNED_INT))  &&  \
        (!__API_FLAGS_HAVE(flags, LONG))          &&  \
        (!__API_FLAGS_HAVE(flags, UNSIGNED_LONG)) &&  \
        (!__API_FLAGS_HAVE(flags, SIZET))         &&  \
        (!__API_FLAGS_HAVE(flags, FLOAT))         &&  \
        (!__API_FLAGS_HAVE(flags, DOUBLE)))           \
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
    if (__API_FLAGS_HAVE(flags, CPU)) {
       mem_flags |= 
          (flags & ZERO_COPY)   ? CL_MEM_ALLOC_HOST_PTR : 0; 
    }
    if (__API_FLAGS_HAVE(flags, HWA)) {
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
               __API_FLAGS_HAVE(m->flags, PINNED) ? m->pinned : m->id,
               __API_FLAGS_HAVE(m->flags, PINNED) ? "pinned " : "",
               (double)m->size*m->unit_size/1024./1024.);
    m->h = clEnqueueMapBuffer(m->queue, 
                              __API_FLAGS_HAVE(m->flags, PINNED) ? m->pinned : m->id,
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
                                       __API_FLAGS_HAVE(m->flags, PINNED) ? 
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
    if (__API_FLAGS_HAVE(src->flags, ZERO_COPY)) {
        dst->flags     = src->flags;
        dst->h         = src->h;
        dst->id        = src->id;
        dst->size      = src->size;
        dst->unit_size = src->unit_size;
    } else {
        if ((__API_MEM_HOST_DIRTY(src->flags)) &&
            (! __API_FLAGS_HAVE(src->flags, READ_ONLY))) {
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
    if (!__API_FLAGS_HAVE(m->flags, READ_ONLY)) {
        __API_MEM_TOUCH_DEVICE(m->flags);
        HICL_DEBUG("mem touch  {h=%p, id=%p}", m->h, m->id);
    }
}

PRIVATE void
__api_mem_sync(void *pointer) {
    himem_t m = (himem_t)pointer;
    if (__API_FLAGS_HAVE(m->flags, ZERO_COPY)) {
        if (__API_MEM_HOST_DIRTY(m->flags) &&
            !__API_FLAGS_HAVE(m->flags, WRITE_ONLY)) {
            __api_mem_unmap(m, CL_TRUE);
            __API_MEM_CLEAR_HOST(m->flags);
            if (__API_FLAGS_HAVE(m->flags, READ_ONLY)) 
                __api_mem_map(m,
                              __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
        }
    } else {
        if (__API_MEM_HOST_DIRTY(m->flags) && 
            !__API_FLAGS_HAVE(m->flags, WRITE_ONLY)) {
            __api_mem_htod(m, CL_FALSE);
            __API_MEM_CLEAR_HOST(m->flags);
        }
    }
    __api_mem_touch(m);
}

PRIVATE himem_t
__api_mem_find(address_t h) {
    return (himem_t)urb_tree_find(&hicl->mems, h, __api_address_cmp)->value;
}

PRIVATE 
void __api_mem_release(void *pointer) {
    himem_t m = (himem_t)pointer;
    if ((m != NULL) && (m->refs >= 0)) {   
        HICL_DEBUG("mem refs-- {h=%p, id=%p} (refs %d => %d)", 
                    m->h, m->id, m->refs, m->refs-1);
        m->refs = m->refs-1; 
        if (m->refs == 0) {
            HICL_DEBUG("mem release {h=%p, id=%p} (refs count = %d)", 
                       m->h, m->id, m->refs);
            if (__API_FLAGS_HAVE(m->flags, CPU)) {
                if (__API_FLAGS_HAVE(m->flags, ZERO_COPY)) {
                    HICL_ABORT(clEnqueueUnmapMemObject(m->queue,
                                                __API_FLAGS_HAVE(m->flags, PINNED) ? 
                                                m->pinned : m->id, 
                                                m->h, 0, NULL, NULL), 
                           "failed to unmap host zero-copy memory");
                    HICL_ABORT(clReleaseMemObject(m->id),
                           "failed to release OpenCL memory");
                } else { free(m->h); }
            } else if (__API_FLAGS_HAVE(m->flags, HWA)) {
                if (__API_FLAGS_HAVE(m->flags, PINNED)) {
                    HICL_ABORT(clReleaseMemObject(m->pinned),
                           "failed to release pinned memory object");
                } else if (__API_FLAGS_HAVE(m->flags, ZERO_COPY)) {
                    __api_mem_unmap(m, CL_TRUE);
                } else {
                    if (!__API_FLAGS_HAVE(m->flags, HOST_ALLOCATED)) {
                        free(m->h);
                    }
                }
                HICL_ABORT(clReleaseMemObject(m->id), 
                       "failed to release OpenCL memory");
            }
            free(m); m = NULL;
        }
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
