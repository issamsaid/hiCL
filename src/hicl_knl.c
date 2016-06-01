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
/// \file hicl_knl.c
/// \version $Id$
/// \brief Implementation of the OpenCL kernel.
///
#include "hiCL/knl.h"
#include <stdio.h>
#include <string.h>
#include "hiCL/timer.h"
#include "hiCL/mem.h"

#include "__api/config/util.h"
#include "__api/mem-inl.h"
#include "__api/knl-inl.h"

GENERATE_LIST_BODY(knl);
GENERATE_RBT_BODY(int, mem);

knl hicl_knl_init(cl_kernel id) {
    size_t idx;
    knl k = (knl)malloc(sizeof(knl_desc));
    k->id  = id;
    k->wrk = 0;
    for(idx=0; idx<__API_KNL_MAX_WORK_SIZE; ++idx) {
        k->gws[idx]=1;
        k->lws[idx]=1;
	k->ofs[idx]=0;
    }
    create_rbt_int_mem(&k->mems, __api_int_cmp, __api_mem_knl_release);
    __API_KNL_GET(id, CL_KERNEL_NUM_ARGS, k->num_args);
    return k;
}

void hicl_knl_release(knl k) {
    remove_node_knl(&cl->knls, k);
}

knl hicl_knl_find(const char *name) {
    node_knl *nk;
    char tmp[__API_STR_SIZE];
    HICL_EXIT_IF((name == NULL) || (strlen(name) == 0),
               "OpenCL kernel name not valid");
    for (nk=cl->knls.head; nk != NULL; nk=nk->next) {
        __API_KNL_GET(nk->data->id, CL_KERNEL_FUNCTION_NAME, tmp); 
        if (!strcmp(name, tmp)) break;
    }
    HICL_EXIT_IF(nk == NULL, "OpenCL kernel '%s' not found", name);
    return nk->data;
}

void hicl_knl_build(knl k, const char *options) {
    cl_int cl_ret;
    char tmp[__API_STR_SIZE], names[__API_BUFFER_SIZE];
    cl_program program;
    size_t idx, num_kernels, code_size;
    cl_kernel *ids;
    __API_KNL_GET(k->id, CL_KERNEL_PROGRAM, program);
    HICL_CHECK(clGetProgramInfo(program, CL_PROGRAM_SOURCE, 0, NULL, &code_size),
             "failed to query OpenCL program source size");
    char code[code_size];
    __API_PROGRAM_GET_PTR(program, CL_PROGRAM_SOURCE, code);
    const char *const_code = code;
#ifdef CL_VERSION_1_2
    __API_PROGRAM_GET_PTR(program, CL_PROGRAM_KERNEL_NAMES, names);
    while(!__api_strstep(tmp, names, ";"))
        remove_node_knl(&cl->knls, hicl_knl_find(tmp));
    program = clCreateProgramWithSource(cl->context, 1,
                                        &const_code, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL program");
    ids = __api_knl_create_from_program(program, options, &num_kernels);
    for(idx=0; idx<num_kernels; ++idx)
        insert_node_knl(&cl->knls, hicl_knl_init(ids[idx]));
#else
    program = clCreateProgramWithSource(cl->context, 1,
                                        &const_code, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL program");
    ids = __api_knl_create_from_program(program, options, &num_kernels);
    for (idx=0; idx<num_kernels; ++idx) {
        __API_KNL_GET(ids[idx], CL_KERNEL_FUNCTION_NAME, tmp);
        remove_node_knl(&cl->knls, hicl_knl_find(tmp)); 
        insert_node_knl(&cl->knls, hicl_knl_init(ids[idx]));
    }
#endif
    free(ids);
    HICL_CHECK(clReleaseProgram(program), "failed to release OpenCL program");
}

void hicl_knl_set_int32(knl k, int index, int32_t i32) {
    __api_knl_set_arg_int32(k->id, index, &i32);
}

void hicl_knl_set_int64(knl k, int index, int64_t i64) {
    __api_knl_set_arg_int64(k->id, index, &i64);
}

void hicl_knl_set_float(knl k, int index, float f) {
    __api_knl_set_arg_float(k->id, index, &f);
}

void hicl_knl_set_double(knl k, int index, double d) {
    __api_knl_set_arg_double(k->id, index, &d);
}

void hicl_knl_set_mem(knl k, int index, address h) {
    rbn_int_mem *n;
    mem m = find_rbn_address_mem(&cl->mems, h)->value;
    if ((n = find_rbn_int_mem(&k->mems, index)) == k->mems.nil) {
        HICL_DEBUG("insert @ %p for kernel %s", m->id, __api_knl_name(k->id));
        insert_rbn_int_mem(&k->mems, index, m);
        insert_rbn_knl_int(&m->knls, k, index);
        __api_knl_set_arg_cl_mem(k->id, index, &m->id);
    } else {
        if (m != n->value) {
            HICL_DEBUG("modify @ %p for kernel %s", m->id, __api_knl_name(k->id));
            n->value = m; 
            __api_knl_set_arg_cl_mem(k->id, index, &m->id);
        }
    }
}

void hicl_knl_set_args(knl k, ...) {
    va_list list;
    va_start(list, k);
    __api_knl_set_args_valist(k, list);
    va_end(list);
}

void hicl_knl_set_wrk(knl k, cl_uint wrk, size_t *gws, size_t *lws) {
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
    HICL_DEBUG("call hicl_knl_set_wrk: global[%lux%lux%lu], local[%lux%lux%lu]",
	     k->gws[0], k->gws[1], k->gws[2], k->lws[0], k->lws[1], k->lws[2]);
}

void hicl_knl_set_ofs(knl k, size_t *ofs) {
    cl_uint idx;
    for(idx=0; idx<3; ++idx) k->ofs[idx] = ofs[idx];
    HICL_DEBUG("call hicl_knl_set_ofs: ofs[%lux%lux%lu]", 
	     k->ofs[0], k->ofs[1], k->ofs[2]);
}

///
/// WARNING: 
///   - run  : asynchronous run
///   - srun : synchronous run
///   - trun : timed srun
///
void hicl_knl_set_and_run(knl k, dev d, ...) {
    HICL_DEBUG("call hicl_knl_set_and_run");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_knl_run(k, d);
}

void hicl_knl_set_and_srun(knl k, dev d, ...) {
    HICL_DEBUG("call hicl_knl_set_and_srun");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_knl_srun(k, d);
}

double hicl_knl_set_and_trun(knl k, dev d, ...) {
    HICL_DEBUG("call hicl_knl_set_and_trun");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    return hicl_knl_trun(k, d);
}

double hicl_knl_flat_trun(knl k, size_t *gws, 
                            size_t *lws, size_t *ofs, dev d, ...) {
    HICL_DEBUG("call hicl_knl_flat_trun");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, gws, lws, ofs);
    return hicl_timer_read();
}

void hicl_knl_run(knl k, dev d) {
    HICL_DEBUG("run (async) kernel : %s", __api_knl_name(k->id));
    walk_value_rbt_int_mem(&k->mems, __api_mem_sync);
    __api_knl_async_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

void hicl_knl_srun(knl k, dev d) {
    HICL_DEBUG("run (sync) kernel  : %s", __api_knl_name(k->id));
    walk_value_rbt_int_mem(&k->mems, __api_mem_sync);
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

double hicl_knl_trun(knl k, dev d) {
    HICL_DEBUG("run (timed) kernel : %s", __api_knl_name(k->id));
    walk_value_rbt_int_mem(&k->mems, __api_mem_sync);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
    return hicl_timer_read();
}

void hicl_knl_info(knl k) {
    __api_knl_info(k->id, k->num_args);
    __API_KNL_INFO_LEVEL_0("kernel %s has %lu memory objects", 
                           __api_knl_name(k->id), k->mems.size);
    walk_value_rbt_int_mem(&k->mems, __api_mem_info);
}

///
/// same functions, nicer names
///
void hicl_build(const char *name, const char *options) {
    hicl_knl_build(hicl_knl_find(name), options);
}

void hicl_set_int32(const char *name, int index, int32_t i32) {
    hicl_knl_set_int32(hicl_knl_find(name), index, i32);
}

void hicl_set_int64(const char *name, int index, int64_t i64) {
    hicl_knl_set_int64(hicl_knl_find(name), index, i64);
}

void hicl_set_float(const char *name, int index, float f) {
    hicl_knl_set_float(hicl_knl_find(name), index, f);
}

void hicl_set_double(const char *name, int index, double d) {
    hicl_knl_set_double(hicl_knl_find(name), index, d);
}

void hicl_set_mem(const char *name, int index, address h) {
    hicl_knl_set_mem(hicl_knl_find(name), index, h);
}

void hicl_set_args(const char *name, ...) {
    va_list list;
    va_start(list, name);
    __api_knl_set_args_valist(hicl_knl_find(name), list);
    va_end(list);
}

void hicl_set_wrk(const char *name, cl_uint wrk, size_t *gws, size_t *lws) {
    hicl_knl_set_wrk(hicl_knl_find(name), wrk, gws, lws);
}

void hicl_set_ofs(const char *name, size_t *ofs) {
    hicl_knl_set_ofs(hicl_knl_find(name), ofs);
}

void hicl_set_and_run(const char* name, dev d, ...) {
    HICL_DEBUG("hicl_set_and_run");
    va_list list;
    knl k = hicl_knl_find(name);
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_knl_run(k, d);
}

void hicl_set_and_srun(const char* name, dev d, ...) {
    HICL_DEBUG("hicl_set_and_run");
    va_list list;
    knl k = hicl_knl_find(name);
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_knl_srun(k, d);
}

double hicl_set_and_trun(const char* name, dev d, ...) {
    HICL_DEBUG("call hicl_set_and_trun");
    va_list list;
    knl k = hicl_knl_find(name);
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    return hicl_knl_trun(k, d);
}

void hicl_run(const char* name, dev d) {
    hicl_knl_run(hicl_knl_find(name), d);
}

void hicl_srun(const char* name, dev d) {
    hicl_knl_srun(hicl_knl_find(name), d);
}

double hicl_trun(const char* name, dev d) {
    return hicl_knl_trun(hicl_knl_find(name), d);
}
