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
/// @file hicl_mem.c
/// @author Issam SAID
/// @brief The implementation of the hiCL memory objects manipulation routines.
///
#include <hiCL/mem.h>
#include <stdio.h>
#include "__api/config/mem.h"
#include "__api/mem-inl.h"
#include "__api/rbt-inl.h"

GENERATE_RBT_BODY(address_t, himem_t);
GENERATE_RBT_BODY(hiknl_t, int);
/*
address_t hicl_mem_allocate(hidev_t d, size_t size, flags_t flags) {
    cl_int cl_ret;
    himem_t m;
    HICL_EXIT_IF(d    == NULL, "invalid device");
    HICL_EXIT_IF(size ==    0, "invalid memory size");
    m = (mem) malloc(sizeof(mem_desc));
    __API_MEM_SET_DEFAULTS(flags);
    m->flags     = flags;
    m->pinned    = NULL;
    m->h         = NULL;
    m->queue     = d->queue;
    m->size      = size;
    m->unit_size = __api_mem_unit_size(m->flags);

    /// allocate the memory on the CPU and ignore the device.
    if (__API_MEM_CPU(m->flags)) {
        if (__API_MEM_HWA(m->flags)            ||
            __API_MEM_PINNED(m->flags)         ||
            __API_MEM_HOST_ALLOCATED(m->flags)) {
            HICL_EXIT("invalid memory flags combination for CPU buffers");
        }
        if (__API_MEM_ZERO_COPY(m->flags)) {
            m->id = clCreateBuffer(hicl->context,
                                   CL_MEM_ALLOC_HOST_PTR | 
                                   __api_mem_update_flags(m->flags),
                                   m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate host zero copy memory");
            __api_mem_map(m, __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
        } else {
            cl_ret = posix_memalign((void**)(&m->h),
                                    __API_MEM_ALIGN_SIZE,
                                    m->size*m->unit_size);
            HICL_EXIT_IF(cl_ret, "failed to allocate host memory");
        }
    /// allocate memory in the HWA memory
    } else if (__API_MEM_HWA(m->flags)) {
        if (__API_MEM_CPU(m->flags) || 
            __API_MEM_HOST_ALLOCATED(m->flags)) {
            HICL_EXIT("invalid memory flags combination for HWA buffers");
        }
        /// zero-copy buffers in the device memory.
        if (__API_MEM_ZERO_COPY(m->flags)) {
            m->id = clCreateBuffer(hicl->context,
                                   CL_MEM_USE_PERSISTENT_MEM_AMD |
                                   __api_mem_update_flags(m->flags),
                                   m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate device zero copy memory");
            __api_mem_map(m, __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
            memset(m->h, 0, m->size*m->unit_size);
        /// pinned buffers for more efficient CPU-HWA transfers.
        } else if (__API_MEM_PINNED(m->flags)) {
            if (__API_MEM_ZERO_COPY(m->flags)) {
                HICL_EXIT("invalid memory flags combination for PINNED buffers");
            }
            m->id = clCreateBuffer(hicl->context,
                                   __api_mem_update_flags(m->flags),
                                   m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate device memory");
            m->pinned = clCreateBuffer(hicl->context,
                                       CL_MEM_ALLOC_HOST_PTR |
                                       __api_mem_update_flags(m->flags),
                                       m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate pinned memory");
            __api_mem_map(m, __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
        /// regular buffers in the  HWA memory with duplicates in the CPU memory.
        } else {
            m->id = clCreateBuffer(hicl->context,
                                   __api_mem_update_flags(m->flags),
                                   m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate device memory");    
            cl_ret = posix_memalign((void**)(&m->h),
                                    __API_MEM_ALIGN_SIZE,
                                    m->size*m->unit_size);
            HICL_EXIT_IF(cl_ret, "failed to allocate host memory");
            memset(m->h, 0, m->size*m->unit_size);
            __api_mem_htod(m, CL_TRUE);
        }
    } else {
        HICL_EXIT("failed to allocate memory objects, flags should include CPU or HWA");
    }
    __API_MEM_CLEAR(m->flags);
    insert_rbn_address_t_mem(&hicl->mems, m->h, m);
    create_rbt_knl_int(&m->knls, __api_knl_cmp, NULL);
    HICL_DEBUG("alloc  @ %p, h: %p, pinned: %p", m->id, m->h, m->pinned);
    return m->h;
}
*/
himem_t hicl_mem_wrap(hidev_t d, void *h, size_t size, flags_t flags) {
    HICL_EXIT_IF(d    == NULL, "invalid device");
    HICL_EXIT_IF(size ==    0, "invalid memory size");
    HICL_EXIT_IF(h    == NULL, "invalid memory pointer");
    rbn_address_t_himem_t *n;
    if ((n=find_rbn_address_t_himem_t(&hicl->mems, h)) != hicl->mems.nil) {
        return n->value;
    } else {
        cl_int cl_ret;
        himem_t m = (himem_t) malloc(sizeof(struct __himem_t));
        __API_MEM_SET_DEFAULTS(flags);
        m->flags     = flags | HOST_ALLOCATED;
        m->pinned    = NULL;
        m->queue     = d->queue;
        m->size      = size;
        m->unit_size = __api_mem_unit_size(m->flags);
        //__API_MEM_PRINT_FLAGS(m->flags);
        if (__API_MEM_ZERO_COPY(m->flags)) {
            /// wrap zero-copy buffers in the CPU memory.
            if (__API_MEM_CPU(m->flags)) {
                m->id = clCreateBuffer(hicl->context,
                                       CL_MEM_USE_HOST_PTR | 
                                       __api_mem_update_flags(m->flags),
                                       m->size*m->unit_size, h, &cl_ret);
                HICL_CHECK(cl_ret, "failed to wrap device memory object");
                // needed by NVIDIA
                __api_mem_map(m, __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
            /// wrap zero-copy buffers in the HWA memory.
            } else if (__API_MEM_HWA(m->flags)) {
                HICL_EXIT("device zero copy objects are not wrappable yet");
            } else {
                HICL_EXIT("invalid memory flags combination for wrapping buffers");
            }
            m->h = h;
        } else {
            /// regular CPU buffers.
            if (__API_MEM_CPU(m->flags)) {
                m->h  = h;
            /// regular HWA buffers.
            } else if (__API_MEM_HWA(m->flags)) {
                if (__API_MEM_PINNED(m->flags)) {
                    m->pinned = clCreateBuffer(hicl->context,
                                              CL_MEM_USE_HOST_PTR | 
                                              __api_mem_update_flags(m->flags),
                                              m->size*m->unit_size, h, &cl_ret);
                    HICL_CHECK(cl_ret, "failed to wrap pinned memory object");
                    __api_mem_map(m, __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
                } else {
                    m->h  = h;
                }
                m->id = clCreateBuffer(hicl->context,
                                       __api_mem_update_flags(m->flags),
                                       m->size*m->unit_size, NULL, &cl_ret);
                HICL_CHECK(cl_ret, "failed to allocate device memory object");
                __api_mem_htod(m, CL_TRUE);
            } else {
                HICL_EXIT("invalid memory flags combination for wrapping buffers");
            }
        }
        __API_MEM_CLEAR(m->flags);
        insert_rbn_address_t_himem_t(&hicl->mems, h, m);
        create_rbt_hiknl_t_int(&m->knls, __api_knl_cmp, NULL);
        HICL_DEBUG("mem wrap   {h=%p, id=%p} (size= %12.5f MB)", 
                   m->h, m->id, (double)m->size*m->unit_size/1024./1024.);
        return m;
    }
}

void hicl_mem_release(address_t h) {
    remove_rbn_address_t_himem_t(&hicl->mems, h);
}

void hicl_mem_update(address_t h, flags_t f) {
    himem_t m = find_rbn_address_t_himem_t(&hicl->mems, h)->value;
    if (__API_MEM_ZERO_COPY(m->flags)) {
        if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_map(m, __api_mem_map_flags(f, CL_FALSE), CL_TRUE);
            __API_MEM_CLEAR_DEVICE(m->flags);
        }
    } else {
        if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_dtoh(m, CL_TRUE);
            __API_MEM_CLEAR_DEVICE(m->flags);
        }
    }
    if (__API_MEM_READ_WRITE_OR_WRITE_ONLY(f)) __API_MEM_TOUCH_HOST(m->flags);
}

void hicl_mem_pop(address_t h, int ix, int ex, 
                    int iy, int ey, int iz, int ez, 
                    int xpitch, int ypitch, cl_bool blocking) {
    himem_t m = find_rbn_address_t_himem_t(&hicl->mems, h)->value;
    if (__API_MEM_ZERO_COPY(m->flags)) {
        if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_map(m, CL_MAP_READ, CL_TRUE);
            __API_MEM_CLEAR_DEVICE(m->flags);
        }
    } else {
        //if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_dtoh_rect3d(m, ix, ex, iy, ey, iz, ez, 
                                  xpitch, ypitch, blocking);
        //    __API_MEM_CLEAR_DEVICE(m->flags);
        //}
    }
    //if (__API_MEM_READ_WRITE_OR_WRITE_ONLY(f)) __API_MEM_TOUCH_HOST(m->flags);
}

void hicl_mem_push(address_t h, int ix, int ex, 
                     int iy, int ey, int iz, int ez, 
                     int xpitch, int ypitch, cl_bool blocking) {
    himem_t m = find_rbn_address_t_himem_t(&hicl->mems, h)->value;
    if (__API_MEM_ZERO_COPY(m->flags)) {
        if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_map(m, CL_MAP_READ, CL_TRUE);
            __API_MEM_CLEAR_DEVICE(m->flags);
        }
    } else {
        //if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_htod_rect3d(m, ix, ex, iy, ey, iz, ez, 
                                  xpitch, ypitch, blocking);
        //    __API_MEM_CLEAR_DEVICE(m->flags);
        //}
    }
    //if (__API_MEM_READ_WRITE_OR_WRITE_ONLY(f)) __API_MEM_TOUCH_HOST(m->flags);
}

void hicl_mem_dtoh(address_t h, cl_bool blocking) {
    himem_t m = find_rbn_address_t_himem_t(&hicl->mems, h)->value;
    if (__API_MEM_ZERO_COPY(m->flags)) {
        if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_map(m, CL_MAP_READ, CL_TRUE);
            __API_MEM_CLEAR_DEVICE(m->flags);
        }
    } else {
        if (__API_MEM_DEVICE_DIRTY(m->flags)) {
            __api_mem_dtoh(m, blocking);
            __API_MEM_CLEAR_DEVICE(m->flags);
        }
    }
}

void hicl_mem_htod(address_t h, cl_bool blocking) {
    himem_t m = find_rbn_address_t_himem_t(&hicl->mems, h)->value;
    if (__API_MEM_ZERO_COPY(m->flags)) {
        //if (__API_MEM_HOST_DIRTY(m->flags) &&
        //    !__API_MEM_WRITE_ONLY(m->flags)) {
            __api_mem_unmap(m, CL_TRUE);
            __API_MEM_CLEAR_HOST(m->flags);
            if (__API_MEM_READ_ONLY(m->flags))
                __api_mem_map(m,
                              __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
        //}
    } else {
        //if (__API_MEM_HOST_DIRTY(m->flags) &&
        //    !__API_MEM_WRITE_ONLY(m->flags)) {
            __api_mem_htod(m, blocking);
            __API_MEM_CLEAR_HOST(m->flags);
        //}
    }
    //__api_mem_touch(m);
}
