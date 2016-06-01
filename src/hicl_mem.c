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
/// \file hicl_mem.c
/// \version $Id$
/// \brief Implements an OpenCL memory wrapper.
///
#include "hiCL/mem.h"
#include <stdio.h>
#include "__api/config/mem.h"
#include "__api/mem-inl.h"
#include "__api/rbt-inl.h"
#include "__api/list-inl.h"

GENERATE_RBT_BODY(address, mem);
GENERATE_RBT_BODY(knl, int);

address hicl_mem_allocate(dev d, size_t size, flags_t flags) {
    cl_int cl_ret;
    mem m;
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
            m->id = clCreateBuffer(cl->context,
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
            m->id = clCreateBuffer(cl->context,
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
            m->id = clCreateBuffer(cl->context,
                                   __api_mem_update_flags(m->flags),
                                   m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate device memory");
            m->pinned = clCreateBuffer(cl->context,
                                       CL_MEM_ALLOC_HOST_PTR |
                                       __api_mem_update_flags(m->flags),
                                       m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate pinned memory");
            __api_mem_map(m, __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
        /// regular buffers in the  HWA memory with duplicates in the CPU memory.
        } else {
            m->id = clCreateBuffer(cl->context,
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
    insert_rbn_address_mem(&cl->mems, m->h, m);
    create_rbt_knl_int(&m->knls, __api_knl_cmp, NULL);
    HICL_DEBUG("alloc  @ %p, h: %p, pinned: %p", m->id, m->h, m->pinned);
    return m->h;
}

mem hicl_mem_wrap(dev d, void *h, size_t size, flags_t flags) {
    cl_int cl_ret;
    mem m;
    HICL_EXIT_IF(d    == NULL, "invalid device");
    HICL_EXIT_IF(size ==    0, "invalid memory size");
    HICL_EXIT_IF(h    == NULL, "invalid memory pointer");
    m = (mem) malloc(sizeof(mem_desc));
    __API_MEM_SET_DEFAULTS(flags);
    m->flags     = flags | HOST_ALLOCATED;
    m->pinned    = NULL;
    m->queue     = d->queue;
    m->size      = size;
    m->unit_size = __api_mem_unit_size(m->flags);
    __API_MEM_PRINT_FLAGS(m->flags);
    if (__API_MEM_ZERO_COPY(m->flags)) {
        /// wrap zero-copy buffers in the CPU memory.
        if (__API_MEM_CPU(m->flags)) {
            m->id = clCreateBuffer(cl->context,
                                   CL_MEM_USE_HOST_PTR | 
                                   __api_mem_update_flags(m->flags),
                                   m->size*m->unit_size, h, &cl_ret);
            HICL_CHECK(cl_ret, "failed to wrap device memory object");
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
                m->pinned = clCreateBuffer(cl->context,
                                          CL_MEM_USE_HOST_PTR | 
                                          __api_mem_update_flags(m->flags),
                                          m->size*m->unit_size, h, &cl_ret);
                HICL_CHECK(cl_ret, "failed to wrap pinned memory object");
                __api_mem_map(m, __api_mem_map_flags(m->flags, CL_TRUE), CL_TRUE);
            } else {
                m->h  = h;
            }
            m->id = clCreateBuffer(cl->context,
                                   __api_mem_update_flags(m->flags),
                                   m->size*m->unit_size, NULL, &cl_ret);
            HICL_CHECK(cl_ret, "failed to allocate device memory object");
            __api_mem_htod(m, CL_TRUE);
        } else {
            HICL_EXIT("invalid memory flags combination for wrapping buffers");
        }
    }
    __API_MEM_CLEAR(m->flags);
    insert_rbn_address_mem(&cl->mems, h, m);
    create_rbt_knl_int(&m->knls, __api_knl_cmp, NULL);
    HICL_DEBUG("wrap   @ %p (%p), h: %p, pinned: %p, size: %12.5f MB", 
	         m->id, h, m->h, m->pinned, (double)m->size*m->unit_size/1024./1024.);
    return m;
}

void hicl_mem_release(address h) {
    remove_rbn_address_mem(&cl->mems, h);
}

void hicl_mem_update(address h, flags_t f) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
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

void hicl_mem_pop(address h, int ix, int ex, 
                    int iy, int ey, int iz, int ez, 
                    int xpitch, int ypitch, cl_bool blocking) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
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

void hicl_mem_push(address h, int ix, int ex, 
                     int iy, int ey, int iz, int ez, 
                     int xpitch, int ypitch, cl_bool blocking) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
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

void hicl_mem_dtoh(address h, cl_bool blocking) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
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

void hicl_mem_htod(address h, cl_bool blocking) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
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

void hicl_mem_info(address h) {
    __api_mem_info(find_rbn_address_mem(&cl->mems, h)->value);
}

double hicl_mem_b(address h) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
    return (double)m->size*m->unit_size;
}
double hicl_mem_kb(address h) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
    return (double)(m->size*m->unit_size)/1024.;
}

double hicl_mem_mb(address h) {
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
    return (double)(m->size*m->unit_size)/1024./1024.;
}

double hicl_mem_gb(address h) { 
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
    return (double)(m->size*m->unit_size)/1024./1024./1024.;
}
