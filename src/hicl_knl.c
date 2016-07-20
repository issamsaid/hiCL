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
/// @file hicl_knl.c
/// @author Issam SAID
/// @brief The implementation of the hiCL kernels manipulation routines.
///
#include "hiCL/knl.h"
#include <stdio.h>
#include <string.h>
#include "hiCL/timer.h"
#include "hiCL/mem.h"

#include "__api/config/util.h"
#include "__api/mem-inl.h"
#include "__api/knl-inl.h"

GENERATE_LIST_BODY(hiknl_t);
GENERATE_RBT_BODY(int, himem_t);

hiknl_t hicl_knl_init(cl_kernel id) {
    size_t idx;
    hiknl_t k = (hiknl_t)malloc(sizeof(struct __hiknl_t));
    k->id  = id;
    k->wrk = 0;
    for(idx=0; idx<__API_KNL_MAX_WORK_SIZE; ++idx) {
        k->gws[idx]=1;
        k->lws[idx]=1;
	    k->ofs[idx]=0;
    }
    create_rbt_int_himem_t(&k->mems, __api_int_cmp, __api_mem_knl_release);
    __API_KNL_GET(id, CL_KERNEL_NUM_ARGS, k->num_args);
    return k;
}

inline void hicl_knl_release(const char *name) {
    list_hiknl_t *tmp_knl;
    unsigned int nb_refs;
    hiknl_t k = hicl_knl_find(name);
    __API_KNL_GET(k->id, CL_KERNEL_REFERENCE_COUNT, nb_refs);
    HICL_DEBUG("releasing OpenCL kernel @ %p (%s, himem_t size = %lu)", 
                k->id, __api_knl_name(k->id), k->mems.size);
    delete_rbt_int_himem_t(&k->mems);
    __api_knl_release(k->id);
    tmp_knl = list_remove_hiknl_t(&hicl->knls, k);
    list_delete_hiknl_t(&tmp_knl);
    free(k); k = NULL;
}

inline hiknl_t hicl_knl_find(const char *name) {
    list_hiknl_t *i_knl;
    char tmp[__API_STR_SIZE];
    HICL_EXIT_IF((name == NULL) || (strlen(name) == 0),
                 "OpenCL kernel name not valid");
    for (i_knl=hicl->knls; i_knl != NULL; i_knl=i_knl->next) {
        __API_KNL_GET(i_knl->data->id, CL_KERNEL_FUNCTION_NAME, tmp); 
        if (!strcmp(name, tmp)) break;
    }
    HICL_EXIT_IF(i_knl == NULL, "OpenCL kernel '%s' not found", name);
    return i_knl->data;
}

inline void hicl_knl_build(const char *name, const char *options) {
    cl_int cl_ret;
    char tmp[__API_STR_SIZE], names[__API_BUFFER_SIZE];
    cl_program program;
    size_t idx, num_kernels, code_size;
    cl_kernel *ids;
    hiknl_t k = hicl_knl_find(name);
    __API_KNL_GET(k->id, CL_KERNEL_PROGRAM, program);
    HICL_CHECK(clGetProgramInfo(program, CL_PROGRAM_SOURCE, 0, NULL, &code_size),
               "failed to query OpenCL program source size");
    char code[code_size], *marker;
    __API_PROGRAM_GET_PTR(program, CL_PROGRAM_SOURCE, code);
    const char *const_code = code;
#ifdef CL_VERSION_1_2
    __API_PROGRAM_GET_PTR(program, CL_PROGRAM_KERNEL_NAMES, names);
    marker = names;
    do {
        marker  = __api_strstep(tmp, marker, ";");
        hicl_knl_release(tmp);
    } while(marker != NULL);
    program = clCreateProgramWithSource(hicl->context, 1,
                                        &const_code, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL program");
    ids = __api_knl_create_from_program(program, options, &num_kernels);
    for(idx=0; idx<num_kernels; ++idx)
        list_insert_hiknl_t(&hicl->knls, 
                            list_create_hiknl_t(hicl_knl_init(ids[idx])));
#else
    program = clCreateProgramWithSource(hicl->context, 1,
                                        &const_code, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL program");
    ids = __api_knl_create_from_program(program, options, &num_kernels);
    for (idx=0; idx<num_kernels; ++idx) {
        __API_KNL_GET(ids[idx], CL_KERNEL_FUNCTION_NAME, tmp);
        hicl_knl_release(tmp);
        list_insert_hiknl_t(&hicl->knls, 
                            list_create_hiknl_t(hicl_knl_init(ids[idx])));
    }
#endif
    free(ids);
    HICL_CHECK(clReleaseProgram(program), "failed to release OpenCL program");
}

inline void hicl_knl_set_int32(const char *name, int index, int32_t i32) {
    hiknl_t k = hicl_knl_find(name);
    __api_knl_set_arg_int32(k->id, index, &i32);
}

inline void hicl_knl_set_int64(const char *name, int index, int64_t i64) {
    hiknl_t k = hicl_knl_find(name);
    __api_knl_set_arg_int64(k->id, index, &i64);
}

inline void hicl_knl_set_float(const char *name, int index, float f) {
    hiknl_t k = hicl_knl_find(name);
    __api_knl_set_arg_float(k->id, index, &f);
}

inline void hicl_knl_set_double(const char *name, int index, double d) {
    hiknl_t k = hicl_knl_find(name);
    __api_knl_set_arg_double(k->id, index, &d);
}

inline void hicl_knl_set_mem(const char *name, int index, address_t h) {
    hiknl_t k = hicl_knl_find(name);
    rbn_int_himem_t *n;
    rbn_address_t_himem_t *i;
    himem_t m;
    HICL_EXIT_IF((i=find_rbn_address_t_himem_t(&hicl->mems, 
                                               h)) == hicl->mems.nil,
                 "memory object not found, check if it is already wrapped");
    m = i->value;
    if ((n = find_rbn_int_himem_t(&k->mems, index)) == k->mems.nil) {
        HICL_DEBUG("insert @ %p for kernel %s", m->id, name);
        insert_rbn_int_himem_t(&k->mems, index, m);
        insert_rbn_hiknl_t_int(&m->knls, k, index);
        __api_knl_set_arg_cl_mem(k->id, index, &m->id);
    } else {
        if (m != n->value) {
            HICL_DEBUG("modify @ %p for kernel %s", m->id, name);
            n->value = m; 
            __api_knl_set_arg_cl_mem(k->id, index, &m->id);
        }
    }
}

inline void hicl_knl_set_args(const char *name, ...) {
    va_list list;
    hiknl_t k = hicl_knl_find(name);
    va_start(list, name);
    __api_knl_set_args_valist(k, list);
    va_end(list);
}

inline void hicl_knl_set_wrk(const char *name, 
                             cl_uint wrk, size_t *gws, size_t *lws) {
    cl_uint idx;
    hiknl_t k = hicl_knl_find(name);
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
    HICL_DEBUG("call hicl_knl_set_wrk: global[%lux%lux%lu], local[%lux%lux%lu]",
	     k->gws[0], k->gws[1], k->gws[2], k->lws[0], k->lws[1], k->lws[2]);
}

inline void hicl_knl_set_ofs(const char *name, size_t *ofs) {
    cl_uint idx;
    hiknl_t k = hicl_knl_find(name);
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
    hiknl_t k = hicl_knl_find(name);
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    HICL_DEBUG("run (async) kernel : %s", name);
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_async_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline void hicl_knl_sync_run(const char *name, hidev_t d, ...) {
    hiknl_t k = hicl_knl_find(name);
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    HICL_DEBUG("exec (sync) kernel  : %s", name);
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline double hicl_knl_timed_run(const char *name, hidev_t d, ...) {
    hiknl_t k = hicl_knl_find(name);
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    HICL_DEBUG("run (timed) kernel : %s", name);
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
    return hicl_timer_read();
}

inline void hicl_knl_exec(const char *name, hidev_t d) {
    hiknl_t k = hicl_knl_find(name);
    HICL_DEBUG("exec (async) kernel : %s", name);
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_async_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline void hicl_knl_sync_exec(const char *name, hidev_t d) {
    hiknl_t k = hicl_knl_find(name);
    HICL_DEBUG("exec (sync) kernel  : %s", name);
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline double hicl_knl_timed_exec(const char *name, hidev_t d) {
    hiknl_t k = hicl_knl_find(name);
    HICL_DEBUG("exec (timed) kernel : %s", name);
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
    return hicl_timer_read();
}

void hicl_knl_info(const char *name) {
    hiknl_t k = hicl_knl_find(name);
    __api_knl_info(k->id, k->num_args);
    __API_KNL_INFO_LEVEL_0("kernel %s has %lu memory objects", 
                           __api_knl_name(k->id), k->mems.size);
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_info);
}