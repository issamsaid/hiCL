#ifndef __API_KNL_INL_H_
#define __API_KNL_INL_H_
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
/// \file knl-inl.h
/// \version $Id: knl-inl.h 2412 2014-05-15 22:10:52Z issam $
/// \brief Defines the private routines for an OpenCL kernel.
///
#include <stdarg.h>
#include "hiCL/types.h"
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/config/util.h"
#include "__api/config/log.h"
#include "__api/util-inl.h"

#define __API_PROGRAM_GET(program, program_info, value)       \
    HICL_CHECK(clGetProgramInfo(program, program_info,       \
                              sizeof(value), &value, NULL),\
             "failed to query OpenCL program info")

#define __API_PROGRAM_GET_PTR(program, program_info, value)  \
    HICL_CHECK(clGetProgramInfo(program, program_info,      \
                              sizeof(value), value, NULL),\
             "failed to query OpenCL program info")

#define __API_KNL_GET(kernel, kernel_info, value)                              \
    HICL_CHECK(clGetKernelInfo(kernel, kernel_info, sizeof(value), &value, NULL),\
             "failed to query OpenCL kernel info")

#define __API_KNL_GET_PTR(kernel, kernel_info, value)                         \
    HICL_CHECK(clGetKernelInfo(kernel, kernel_info, sizeof(value), value, NULL),\
             "failed to query OpenCL kernel info")

#ifdef CL_VERSION_1_2

#define __API_KNL_ARG_GET(kernel, idx, arg_info, value)      \
    HICL_CHECK(clGetKernelArgInfo(kernel, idx, arg_info,       \
                                sizeof(value), &value, NULL),\
             "failed to query OpenCL kernel arg info")

#define __API_KNL_ARG_GET_PTR(kernel, idx, arg_info, value) \
    HICL_CHECK(clGetKernelArgInfo(kernel, idx, arg_info,      \
                                sizeof(value), value, NULL),\
             "failed to query OpenCL kernel arg info")

#endif  // CL_VERSION_1_2

#define __API_KNL_WGP_GET(kernel, device, info, value)             \
    HICL_CHECK(clGetKernelWorkGroupInfo(kernel, device, info,        \
                                      sizeof(value), value, NULL), \
             "failed to query OpenCL kernel work group info")

#define __API_KNL_WGP_GET_PTR(kernel, device, info, value)         \
    HICL_CHECK(clGetKernelWorkGroupInfo(kernel, device, info,        \
                                      sizeof(value), &value, NULL),\
             "failed to query OpenCL kernel work group info")


#define __API_KNL_INFO_LEVEL_0(fmt, ...) fprintf(cl->fdout,\
C_GREEN"\no OpenCL "fmt":\n"C_END, ##__VA_ARGS__)

#define __API_KNL_INFO_LEVEL_1(fmt, ...) fprintf(cl->fdout,\
"\to %-20s: "fmt"\n", ##__VA_ARGS__)

#define __API_KNL_INFO_LEVEL_2(fmt, ...) fprintf(cl->fdout,\
"\t %-22s "fmt"\n", " ", ##__VA_ARGS__)

CPPGUARD_BEGIN()

PRIVATE char*
__api_knl_name(cl_kernel kernel) {
    static char tmp[__API_STR_SIZE];
    __API_KNL_GET_PTR(kernel, CL_KERNEL_FUNCTION_NAME, tmp);
    return tmp;
}

PRIVATE cl_kernel*
__api_knl_create_from_program(cl_program program,
                                 const char *options,
                                 size_t *num_kernels) {
    char buffer[__API_BUFFER_SIZE];
    cl_kernel *ids;
    cl_int cl_ret = clBuildProgram(program, 0, NULL, options, NULL, NULL);
    cl_context context;
    cl_uint idx, nb_devices;
    if (cl_ret != CL_SUCCESS) {
        __API_PROGRAM_GET(program, CL_PROGRAM_CONTEXT, context);
        __API_CONTEXT_GET(context, CL_CONTEXT_NUM_DEVICES, nb_devices);
        cl_device_id ids[nb_devices];
        __API_CONTEXT_GET_PTR(context, CL_CONTEXT_DEVICES, ids);
        for (idx=0; idx < nb_devices; ++idx) {
            HICL_DEBUG("OpenCL build failed (%s) for '%p', collecting build info",
                     __api_error_msg(cl_ret), ids[idx]);
            HICL_CHECK(clGetProgramBuildInfo(program, ids[idx],
                                           CL_PROGRAM_BUILD_LOG,
                                           __API_BUFFER_SIZE, buffer, NULL),
                     "failed to load program build info");
            HICL_WARN("build info for OpenCL device %p:\n%s", ids[idx], buffer);
        }
        CL_FAIL(cl_ret, "failed to build OpenCL program");
    }
#ifdef CL_VERSION_1_2
    size_t i;
    char tmp[__API_STR_SIZE];
    __API_PROGRAM_GET(program, CL_PROGRAM_NUM_KERNELS, i);
    ids = (cl_kernel*) malloc(sizeof(cl_kernel)*i);
    *num_kernels = i; i = 0;
    __API_PROGRAM_GET_PTR(program, CL_PROGRAM_KERNEL_NAMES, buffer);
    while(!__api_strstep(tmp, buffer, ";")) {
        HICL_DEBUG("creating OpenCL kernel: %s", tmp);
        ids[i++] = clCreateKernel(program, tmp, &cl_ret);
        HICL_CHECK(cl_ret, "failed to create OpenCL kernel object");
    }
#else
    cl_ret = clCreateKernelsInProgram(program, 0, NULL, &idx);
    HICL_CHECK(cl_ret, "failed to query OpenCL kernels");
    ids = (cl_kernel*) malloc(sizeof(cl_kernel)*idx);
    cl_ret = clCreateKernelsInProgram(program, idx, ids, NULL);
    HICL_CHECK(cl_ret, "failed to create OpenCL kernels");
    *num_kernels = idx;
#endif // CL_VERSION_1_2
    return ids;
}

PRIVATE void
__api_knl_set_arg_int32(cl_kernel k, int idx, int32_t *ptr) {
    HICL_CHECK(clSetKernelArg(k, idx, sizeof(int32_t), (void*)ptr),
             "failed to set argument n° %d (int32) for kernel %s",
             idx, __api_knl_name(k));
}

PRIVATE void
__api_knl_set_arg_int64(cl_kernel k, int idx, int64_t *ptr) {
    HICL_CHECK(clSetKernelArg(k, idx, sizeof(int64_t), (void*)ptr),
             "failed to set argument n° %d (int64) for kernel %s",
             idx, __api_knl_name(k));
}

PRIVATE void
__api_knl_set_arg_float(cl_kernel k, int idx, float *ptr) {
    HICL_CHECK(clSetKernelArg(k, idx, sizeof(float), (void*)ptr),
             "failed to set argument n° %d (float) for kernel %s", 
             idx, __api_knl_name(k));
}

PRIVATE void
__api_knl_set_arg_double(cl_kernel k, int idx, double *ptr) {
    HICL_CHECK(clSetKernelArg(k, idx, sizeof(double), (void*)ptr),
             "failed to set argument n° %d (double) for kernel %s", 
             idx, __api_knl_name(k));
}

PRIVATE void
__api_knl_set_arg_cl_mem(cl_kernel k, int idx, cl_mem *ptr) {
    HICL_CHECK(clSetKernelArg(k, idx, sizeof(cl_mem), (void*)ptr),
             "failed to set argument n° %d (pointer) for kernel %s", 
             idx, __api_knl_name(k));
}

#ifndef CL_VERSION_1_2
PRIVATE void
__api_knl_get_arg_types(cl_kernel k, int n, 
                           char name[], char types[][__API_STR_SIZE]) {
    cl_int cl_ret, i;
    cl_program program;
    size_t code_size;
    char tmp[__API_BUFFER_SIZE], *pch, *f, *s, *e;
    __API_KNL_GET(k, CL_KERNEL_PROGRAM, program);
    HICL_CHECK(clGetProgramInfo(program, CL_PROGRAM_SOURCE, 0, NULL, &code_size),
             "failed to query OpenCL program source size");
    char code[code_size];
    __API_PROGRAM_GET_PTR(program, CL_PROGRAM_SOURCE, code);
    f = strstr(code, name);
    s = strstr(f, "(");
    e = strstr(f, ")");
    strncpy(tmp, s+1, strlen(s+1)-strlen(e));
    tmp[strlen(tmp)-2] = '\0';
    pch = strtok(tmp, ",");
    i   = 0;
    while (pch != NULL) {
        sprintf(types[i++], "%s", pch);
        pch = strtok(NULL, ",");
    }
}
#endif // ! CL_VERSION_1_2

PRIVATE void
__api_knl_set_args_valist(knl k, va_list list) {
    cl_uint a;
    char tmp[__API_STR_SIZE];
    cl_kernel kernel = k->id;
#ifdef CL_VERSION_1_2
    for (a = 0; a < k->num_args; ++a) {
        __API_KNL_ARG_GET_PTR(kernel, a, CL_KERNEL_ARG_TYPE_NAME, tmp);
        if (strstr(tmp, "*")) {
            mem m = find_rbn_address_mem(&cl->mems, 
                                         va_arg(list, address))->value;
            rbn_int_mem *n;
            if ((n = find_rbn_int_mem(&k->mems, a)) == k->mems.nil) {
                HICL_DEBUG("insert @ %p for kernel %s", 
                         m->id, __api_knl_name(k->id));
                insert_rbn_int_mem(&k->mems, a, m);
                insert_rbn_knl_int(&m->knls, k, a);
                __api_knl_set_arg_cl_mem(k->id, a, &m->id);
            } else {
                if (m != n->value) {
                    HICL_DEBUG("modify @ %p for kernel %s",
                             m->id, __api_knl_name(k->id));
                    n->value = m; 
                    __api_knl_set_arg_cl_mem(k->id, a, &m->id);
                }
            }
        } else if (strstr(tmp, "float")) {
            float f = va_arg(list, double);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(f),(void*)&f),
                     "failed to set OpenCL kernel argument (float)");
        } else if (strstr(tmp, "double")) {
            double d = va_arg(list, double);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(d),(void*)&d),
                     "failed to set OpenCL kernel argument (double)");
        } else if (strstr(tmp, "int")) {
            int i = va_arg(list, int);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(i),(void*)&i),
                     "failed to set OpenCL kernel argument (int)");
        } else if ((strstr(tmp, "int") && (strstr(tmp, "unsigned"))) ||
                   (strstr(tmp, "uint"))) {
            unsigned int ui = va_arg(list, unsigned int);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(ui),(void*)&ui),
                     "failed to set OpenCL kernel argument (unsigned int)");
        } else if (strstr(tmp, "long")) {
            int64_t l = va_arg(list, int64_t);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(l),(void*)&l),
                     "failed to set OpenCL kernel argument (long)");
        } else if ((strstr(tmp, "long") && (strstr(tmp, "unsigned"))) ||
                   (strstr(tmp, "ulong"))) {
            uint64_t ul = va_arg(list, uint64_t);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(ul),(void*)&ul),
                     "failed to set OpenCL kernel argument (unsigned long)");
        } else if (strstr(tmp, "char") || strstr(tmp, "uchar")) {
            char c = va_arg(list, int);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(c),(void*)&c),
                     "failed to set OpenCL kernel argument (char)");
        } else {
            HICL_EXIT("unknown OpenCL kernel argument type");
        }
    }
#else
    char types[k->num_args][__API_STR_SIZE];
    __API_KNL_GET_PTR(kernel, CL_KERNEL_FUNCTION_NAME, tmp);
    __api_knl_get_arg_types(kernel, k->num_args, tmp, types);
    for (a = 0; a < k->num_args; ++a) {
        if (strstr(types[a], "*")) {
            mem m = find_rbn_address_mem(&cl->mems, 
                                         va_arg(list, address))->value;
            rbn_int_mem *n;
            if ((n = find_rbn_int_mem(&k->mems, index)) == k->mems.nil) {
                HICL_DEBUG("insert @ %p for kernel %s", 
                         m->id, __api_knl_name(k->id));
                insert_rbn_int_mem(&k->mems, a, m);
                insert_rbn_knl_int(&m->knls, k, a);
                __api_knl_set_arg_cl_mem(k->id, a, &m->id);
            } else {
                if (m != n->value) {
                    HICL_DEBUG("modify @ %p for kernel %s",
                             m->id, __api_knl_name(k->id));
                    n->value = m; 
                    __api_knl_set_arg_cl_mem(k->id, a, &m->id);
                }
            }
        } else if (strstr(types[a], "float")) {
            float f = va_arg(list, double);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(f),(void*)&f),
                     "failed to set OpenCL kernel argument (float)");
        } else if (strstr(types[a], "double")) {
            double d = va_arg(list, double);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(d),(void*)&d),
                     "failed to set OpenCL kernel argument (double)");
        } else if (strstr(types[a], "int")) {
            int i = va_arg(list, int);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(i),(void*)&i),
                     "failed to set OpenCL kernel argument (int)");
        } else if ((strstr(types[a], "int")        && 
                   (strstr(types[a], "unsigned"))) ||
                   (strstr(types[a], "uint"))) {
            unsigned int ui = va_arg(list, unsigned int);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(ui),(void*)&ui),
                     "failed to set OpenCL kernel argument (unsigned int)");
        } else if (strstr(types[a], "long")) {
            int64_t l = va_arg(list, int64_t);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(l),(void*)&l),
                     "failed to set OpenCL kernel argument (long)");
        } else if ((strstr(types[a], "long")       && 
                   (strstr(types[a], "unsigned"))) ||
                   (strstr(types[a], "ulong"))) {
            uint64_t ul = va_arg(list, uint64_t);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(ul),(void*)&ul),
                     "failed to set OpenCL kernel argument (unsigned long)");
        } else if (strstr(types[a], "char") || strstr(types[a], "uchar")) {
            char c = va_arg(list, int);
            HICL_CHECK(clSetKernelArg(kernel, a, sizeof(c),(void*)&c),
                     "failed to set OpenCL kernel argument (char)");
        } else {
            HICL_EXIT("unknown OpenCL kernel argument type");
        }
    }
#endif // CL_VERSION_1_2
}

PRIVATE void
__api_knl_sync_run(cl_kernel kernel, cl_command_queue queue,
		   cl_uint wrk, const size_t *gws, const size_t *lws,
		   const size_t *ofs) {
    HICL_CHECK(clEnqueueNDRangeKernel(queue, kernel, wrk,
                                    ofs, gws, lws, 0, NULL, NULL),
                                    "failed to run OpenCL kernel");
    clFinish(queue);
}

PRIVATE void
__api_knl_async_run(cl_kernel kernel, cl_command_queue queue,
		    cl_uint wrk, const size_t *gws, const size_t *lws, 
		    const size_t *ofs) {
    HICL_CHECK(clEnqueueNDRangeKernel(queue, kernel, wrk,
                                    ofs, gws, lws, 0, NULL, NULL),
                                    "failed to run OpenCL kernel");
}

PRIVATE void
__api_knl_sync_run_task(cl_kernel kernel, cl_command_queue queue) {
    HICL_CHECK(clEnqueueTask(queue, kernel, 0, NULL, NULL),
                           "failed to run OpenCL task");
    clFinish(queue);
}

PRIVATE void
__api_knl_async_run_task(cl_kernel kernel, cl_command_queue queue) {
    HICL_CHECK(clEnqueueTask(queue, kernel, 0, NULL, NULL),
                           "failed to run OpenCL task");
}

PRIVATE void
__api_knl_info(cl_kernel kernel, cl_uint num_args) {
    cl_uint i;
    char tmp_0[__API_STR_SIZE], tmp_1[__API_STR_SIZE];
    __API_KNL_GET_PTR(kernel, CL_KERNEL_FUNCTION_NAME, tmp_0);
    __API_KNL_INFO_LEVEL_0("kernel @ %p", kernel);
    __API_KNL_INFO_LEVEL_1("%s", "name", tmp_0);
    __API_KNL_INFO_LEVEL_1("%u", "num args", num_args);
#ifdef CL_VERSION_1_2
    if (num_args > 0) {
        __API_KNL_ARG_GET_PTR(kernel, 0, CL_KERNEL_ARG_NAME, tmp_0);
        __API_KNL_ARG_GET_PTR(kernel, 0, CL_KERNEL_ARG_TYPE_NAME, tmp_1);
        __API_KNL_INFO_LEVEL_1("%-2d - %-10s (%s)", "args:", 0, tmp_0, tmp_1);
        for (i=1; i<num_args; ++i) {
            __API_KNL_ARG_GET_PTR(kernel, i, CL_KERNEL_ARG_NAME, tmp_0);
            __API_KNL_ARG_GET_PTR(kernel, i, CL_KERNEL_ARG_TYPE_NAME, tmp_1);
            __API_KNL_INFO_LEVEL_2("%-2d - %-10s (%s)", i, tmp_0, tmp_1);
        }
    }
#endif // CL_VERSION_1_2
}

PRIVATE void
__api_knl_release(knl k) {
    if (k) {
        HICL_DEBUG("releasing OpenCL kernel @ %p (%s, mem size = %lu)", 
                 k->id, __api_knl_name(k->id), k->mems.size);
        delete_rbt_int_mem(&k->mems);
        if (clReleaseKernel(k->id) != CL_SUCCESS)
            HICL_WARN("failed to release OpenCL kernel");
        free(k); k = NULL;
    }
}

CPPGUARD_END()

#endif  // __API_KNL_INL_H_
