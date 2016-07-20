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
/// @file hicl_knl_performance.h
/// @author Issam SAID
/// @brief Extra routines used to evaluate the performance of the hiCL kernels
/// manipulation routines.
///
#include "hiCL/knl.h"
#include <stdio.h>
#include <string.h>
#include "hiCL/timer.h"
#include "hiCL/mem.h"

#include "__api/config/util.h"
#include "__api/mem-inl.h"
#include "__api/knl-inl.h"

inline void hicl_fknl_set_int32(hiknl_t k, int index, int32_t i32) {
    __api_knl_set_arg_int32(k->id, index, &i32);
}

inline void hicl_fknl_set_int64(hiknl_t k, int index, int64_t i64) {
    __api_knl_set_arg_int64(k->id, index, &i64);
}

inline void hicl_fknl_set_float(hiknl_t k, int index, float f) {
    __api_knl_set_arg_float(k->id, index, &f);
}

inline void hicl_fknl_set_double(hiknl_t k, int index, double d) {
    __api_knl_set_arg_double(k->id, index, &d);
}

inline void hicl_fknl_set_mem(hiknl_t k, int index, address_t h) {
    rbn_int_himem_t *n;
    himem_t m = find_rbn_address_t_himem_t(&hicl->mems, h)->value;
    if ((n = find_rbn_int_himem_t(&k->mems, index)) == k->mems.nil) {
        HICL_DEBUG("insert @ %p for kernel %s", m->id, __api_knl_name(k->id));
        insert_rbn_int_himem_t(&k->mems, index, m);
        insert_rbn_hiknl_t_int(&m->knls, k, index);
        __api_knl_set_arg_cl_mem(k->id, index, &m->id);
    } else {
        if (m != n->value) {
            HICL_DEBUG("modify @ %p for kernel %s", m->id, __api_knl_name(k->id));
            n->value = m; 
            __api_knl_set_arg_cl_mem(k->id, index, &m->id);
        }
    }
}

inline void hicl_fknl_set_args(hiknl_t k, ...) {
    va_list list;
    va_start(list, k);
    __api_knl_set_args_valist(k, list);
    va_end(list);
}

inline void hicl_fknl_set_wrk(hiknl_t k, cl_uint wrk, size_t *gws, size_t *lws) {
    cl_uint idx;
    k->wrk = wrk;
    for(idx=0; idx<3; ++idx) {
        if (idx < wrk) {
            k->gws[idx] = gws[idx];
            k->lws[idx] = lws[idx];
        } else {
            k->gws[idx] = 1;
            k->lws[idx] = 1;
        }
    }
    HICL_DEBUG("call hicl_fknl_set_wrk: global[%lux%lux%lu], local[%lux%lux%lu]",
	     k->gws[0], k->gws[1], k->gws[2], k->lws[0], k->lws[1], k->lws[2]);
}

inline void hicl_fknl_set_ofs(hiknl_t k, size_t *ofs) {
    cl_uint idx;
    for(idx=0; idx<3; ++idx) k->ofs[idx] = ofs[idx];
    HICL_DEBUG("call hicl_fknl_set_ofs: ofs[%lux%lux%lu]", 
	     k->ofs[0], k->ofs[1], k->ofs[2]);
}

inline void hicl_fknl_exec(hiknl_t k, hidev_t d) {
    HICL_DEBUG("run (async) kernel : %s", __api_knl_name(k->id));
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_async_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline void hicl_fknl_sync_exec(hiknl_t k, hidev_t d) {
    HICL_DEBUG("run (sync) kernel  : %s", __api_knl_name(k->id));
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline double hicl_fknl_timed_exec(hiknl_t k, hidev_t d) {
    HICL_DEBUG("run (timed) kernel : %s", __api_knl_name(k->id));
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
    return hicl_timer_read();
}

inline void hicl_fknl_run(hiknl_t k, hidev_t d, ...) {
    HICL_DEBUG("call hicl_fknl_set_and_run");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_fknl_exec(k, d);
}

inline void hicl_fknl_sync_run(hiknl_t k, hidev_t d, ...) {
    HICL_DEBUG("call hicl_fknl_sync_run");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_fknl_sync_exec(k, d);
}

inline double hicl_fknl_timed_run(hiknl_t k, hidev_t d, ...) {
    HICL_DEBUG("call hicl_fknl_set_and_trun");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    return hicl_fknl_timed_exec(k, d);
}