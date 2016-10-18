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
/// @file hicl_knl.c
/// @author Issam SAID
/// @brief The implementation of the hiCL kernels manipulation routines.
///
#include <hiCL/knl.h>
#include <stdio.h>
#include <string.h>
#include <hiCL/timer.h>
#include <hiCL/mem.h>
#include "__api/config/util.h"
#include "__api/mem-inl.h"
#include "__api/knl-inl.h"

inline void hicl_knl_build(const char *name, const char *options) {
    cl_int cl_ret;
    char tmp[__API_BUFFER_SIZE];
    const char *const_code;
    cl_program program;
    size_t num_kernels;
    cl_kernel *ids;
    hiknl_t k = __api_knl_find(name);
    HICL_EXIT_IF(__api_knl_extract_code(k->id, name, tmp), 
                 "failed to extract kernel code in order to re-build it");
    const_code = tmp;
    program = clCreateProgramWithSource(hicl->context, 1,
                                        &const_code, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL program");
    ids   = __api_knl_create_from_program(program, options, &num_kernels);
    if (clReleaseKernel(k->id) != CL_SUCCESS)
        HICL_FAIL("failed to release Openx`CL kernel");
    k->id = ids[0];
    free(ids);
    HICL_CHECK(clReleaseProgram(program), "failed to release OpenCL program");
}

inline void hicl_knl_set_int32(const char *name, int index, int32_t i32) {
    hiknl_t k = __api_knl_find(name);
    __api_knl_set_arg_int32(k->id, index, &i32);
}

inline void hicl_knl_set_int64(const char *name, int index, int64_t i64) {
    hiknl_t k = __api_knl_find(name);
    __api_knl_set_arg_int64(k->id, index, &i64);
}

inline void hicl_knl_set_float(const char *name, int index, float f) {
    hiknl_t k = __api_knl_find(name);
    __api_knl_set_arg_float(k->id, index, &f);
}

inline void hicl_knl_set_double(const char *name, int index, double d) {
    hiknl_t k = __api_knl_find(name);
    __api_knl_set_arg_double(k->id, index, &d);
}

inline void hicl_knl_set_mem(const char *name, int index, address_t h) {
    hiknl_t k = __api_knl_find(name);
    urb_t *n, *i;
    int *ikey;
    himem_t m;
    HICL_EXIT_IF((i = urb_tree_find(&hicl->mems, h, __api_address_cmp)) == 
                  &urb_sentinel,
                 "memory object not found, check if it is already wrapped");
    m = (himem_t)i->value;
    if ((n = urb_tree_find(&k->mems, &index, __api_int_cmp)) == 
         &urb_sentinel) {
        ikey  = (int*)malloc(sizeof(int));
        *ikey = index;
        urb_tree_put(&k->mems, urb_tree_create(ikey, m), __api_int_cmp);
        m->refs++;
        __api_knl_set_arg_cl_mem(k->id, index, &m->id);
        HICL_DEBUG("mem insert {h=%p, id=%p, refs = %d} for kernel %s", 
                    m->h, m->id, m->refs, name);
    } else {
        if (m != (himem_t)n->value) {
            ((himem_t)n->value)->refs = ((himem_t)n->value)->refs-1;
            m->refs++;
            HICL_DEBUG("mem modify {h=%p, id=%p, refs = %d} to"
                       " {h=%p, id=%p, refs = %d} for kernel %s", 
                        ((himem_t)n->value)->h, 
                        ((himem_t)n->value)->id, 
                        ((himem_t)n->value)->refs, 
                        m->h, m->id, m->refs, name);
            n->value = m; 
            __api_knl_set_arg_cl_mem(k->id, index, &m->id);
        }
    }
}

inline void hicl_knl_set_args(const char *name, ...) {
    va_list list;
    hiknl_t k = __api_knl_find(name);
    va_start(list, name);
    __api_knl_set_args_valist(k, list);
    va_end(list);
}

inline void hicl_knl_set_wrk(const char *name, 
                             cl_uint wrk, size_t *gws, size_t *lws) {
    cl_uint idx;
    hiknl_t k = __api_knl_find(name);
    k->wrk = wrk;
    for(idx=0; idx<3; ++idx) {
        if (idx < wrk) {
#ifdef __API_ROUNDUP_WRK
            k->gws[idx] = __api_nmult64(gws[idx], lws[idx]);
#else
            k->gws[idx] = gws[idx];
#endif // __API_ROUNDUP_WRK
            k->lws[idx] = lws[idx];
        } else {
            k->gws[idx] = 1;
            k->lws[idx] = 1;
        }
    }
    HICL_DEBUG("call hicl_knl_set_wrk: global[%lux%lux%lu], local[%lux%lux%lu]",
	     k->gws[0], k->gws[1], k->gws[2], k->lws[0], k->lws[1], k->lws[2]);
}

inline void hicl_knl_set_ofs(const char *name, size_t *ofs) {
    cl_uint idx;
    hiknl_t k = __api_knl_find(name);
    for(idx=0; idx<3; ++idx) k->ofs[idx] = ofs[idx];
    HICL_DEBUG("call hicl_knl_set_ofs: ofs[%lux%lux%lu]", 
	     k->ofs[0], k->ofs[1], k->ofs[2]);
}

//
// WARNING: 
//   - run (default) : asynchronous run
//   - sync run      : synchronous run
//   - timed run     : timed synchronous execution
//
inline void hicl_knl_run(const char *name, hidev_t d, ...) {
    hiknl_t k = __api_knl_find(name);
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    HICL_DEBUG("run (async) kernel : %s", name);
    urb_tree_walk(&k->mems, NULL, __api_mem_sync);
    __api_knl_async_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline void hicl_knl_sync_run(const char *name, hidev_t d, ...) {
    hiknl_t k = __api_knl_find(name);
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    HICL_DEBUG("exec (sync) kernel  : %s", name);
    urb_tree_walk(&k->mems, NULL, __api_mem_sync);
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline double hicl_knl_timed_run(const char *name, hidev_t d, ...) {
    hiknl_t k = __api_knl_find(name);
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    HICL_DEBUG("run (timed) kernel : %s", name);
    urb_tree_walk(&k->mems, NULL, __api_mem_sync);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
    return hicl_timer_read();
}

inline void hicl_knl_exec(const char *name, hidev_t d) {
    hiknl_t k = __api_knl_find(name);
    HICL_DEBUG("exec (async) kernel : %s", name);
    urb_tree_walk(&k->mems, NULL, __api_mem_sync);
    __api_knl_async_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline void hicl_knl_sync_exec(const char *name, hidev_t d) {
    hiknl_t k = __api_knl_find(name);
    HICL_DEBUG("exec (sync) kernel  : %s", name);
    urb_tree_walk(&k->mems, NULL, __api_mem_sync);
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline double hicl_knl_timed_exec(const char *name, hidev_t d) {
    hiknl_t k = __api_knl_find(name);
    HICL_DEBUG("exec (timed) kernel : %s", name);
    urb_tree_walk(&k->mems, NULL, __api_mem_sync);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
    return hicl_timer_read();
}
