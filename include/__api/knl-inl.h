#ifndef __API_KNL_INL_H_
#define __API_KNL_INL_H_
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
/// @file __api/knl-inl.h
/// @author Issam SAID
/// @brief Private functions used to implement the hiCL kernel descriptor.
///
#include <stdarg.h>
#include <hiCL/types.h>
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/config/util.h"
#include "__api/config/log.h"
#include "__api/util-inl.h"

CPPGUARD_BEGIN();

#define __API_PRG_GET(program, program_info, value)         \
    HICL_CHECK(clGetProgramInfo(program, program_info,      \
                              sizeof(value), &value, NULL), \
               "failed to query OpenCL program info")

#define __API_PRG_GET_PTR(program, program_info, value)     \
    HICL_CHECK(clGetProgramInfo(program, program_info,      \
                              sizeof(value), value, NULL),  \
               "failed to query OpenCL program info")

#define __API_KNL_GET(kernel, kernel_info, value)           \
    HICL_CHECK(clGetKernelInfo(kernel, kernel_info,         \
                               sizeof(value), &value, NULL),\
               "failed to query OpenCL kernel info")

#define __API_KNL_GET_PTR(kernel, kernel_info, value)       \
    HICL_CHECK(clGetKernelInfo(kernel, kernel_info,         \
                               sizeof(value), value, NULL), \
               "failed to query OpenCL kernel info")

#ifdef CL_VERSION_1_2

#define __API_KNL_ARG_GET(kernel, idx, arg_info, value)        \
    HICL_CHECK(clGetKernelArgInfo(kernel, idx, arg_info,       \
                                  sizeof(value), &value, NULL),\
               "failed to query OpenCL kernel arg info")

#define __API_KNL_ARG_GET_PTR(kernel, idx, arg_info, value)   \
    HICL_CHECK(clGetKernelArgInfo(kernel, idx, arg_info,      \
                                  sizeof(value), value, NULL),\
               "failed to query OpenCL kernel arg info")

#endif  // CL_VERSION_1_2

#define __API_KNL_WGP_GET(kernel, device, info, value)             \
    HICL_CHECK(clGetKernelWorkGroupInfo(kernel, device, info,      \
                                      sizeof(value), value, NULL), \
               "failed to query OpenCL kernel work group info")

#define __API_KNL_WGP_GET_PTR(kernel, device, info, value)         \
    HICL_CHECK(clGetKernelWorkGroupInfo(kernel, device, info,      \
                                      sizeof(value), &value, NULL),\
               "failed to query OpenCL kernel work group info")


#define __API_KNL_INFO_LEVEL_0(fmt, ...) fprintf(hicl->fdout,\
HICL_GREEN"\no OpenCL "fmt":\n"HICL_END, ##__VA_ARGS__)

#define __API_KNL_INFO_LEVEL_1(fmt, ...) fprintf(hicl->fdout,\
"\to %-20s: "fmt"\n", ##__VA_ARGS__)

#define __API_KNL_INFO_LEVEL_2(fmt, ...) fprintf(hicl->fdout,\
"\t %-22s "fmt"\n", " ", ##__VA_ARGS__)

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
    cl_context context;
    cl_uint idx, nb_devices;
    cl_int cl_ret;
#ifdef CL_VERSION_1_2
    if((getenv("HICL_BUILD_OPTIONS")) == NULL)
        sprintf(buffer, "-cl-kernel-arg-info %s", options);
    else
        sprintf(buffer, "-cl-kernel-arg-info %s %s", 
                getenv("HICL_BUILD_OPTIONS"), options);
#else
    if((getenv("HICL_BUILD_OPTIONS")) == NULL)
        sprintf(buffer, "%s", options);
    else
        sprintf(buffer, "%s %s", getenv("HICL_BUILD_OPTIONS"), options);
#endif 
    cl_ret = clBuildProgram(program, 0, NULL, buffer, NULL, NULL);
    if (cl_ret != CL_SUCCESS) {
        __API_PRG_GET(program, CL_PROGRAM_CONTEXT, context);
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
        HICL_CHECK(cl_ret, "failed to build OpenCL program");
    }
#ifdef CL_VERSION_1_2
    size_t i;
    char tmp[__API_STR_SIZE], *marker;
    __API_PRG_GET(program, CL_PROGRAM_NUM_KERNELS, i);
    ids = (cl_kernel*) malloc(sizeof(cl_kernel)*i);
    *num_kernels = i; i = 0;
    __API_PRG_GET_PTR(program, CL_PROGRAM_KERNEL_NAMES, buffer);
    marker = buffer;
    do {
        marker = __api_strstep(tmp, marker, ";");
        HICL_DEBUG("building OpenCL kernel: '%s'", tmp);
        ids[i++] = clCreateKernel(program, tmp, &cl_ret);
        HICL_CHECK(cl_ret, "failed to create OpenCL kernel object");
    } while(marker != NULL);    
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

PRIVATE int
__api_knl_extract_code(cl_kernel k, const char* name, char* container) {
    cl_program program;
    size_t code_size, length;
    char *pch = NULL, *s =NULL, *e = NULL;
    __API_KNL_GET(k, CL_KERNEL_PROGRAM, program);
    HICL_CHECK(clGetProgramInfo(program, 
                                CL_PROGRAM_SOURCE, 
                                0, NULL, &code_size),
               "failed to query OpenCL program source size");
    char code[code_size];
    __API_PRG_GET_PTR(program, CL_PROGRAM_SOURCE, code);
    if (strstr(code, name) == NULL) return -1;
    s = code;
    while (pch == NULL) {
        s   = strstr(s, "kernel");
        pch = strstr(s, name);
        s   = s+6;
    }
    s = s-6;
    e = strstr(s, "}");
    length = strlen(s)-strlen(e+1)+1;
    strncpy(container, s, length);
    container[length-1] = '\0';
    return 0;
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
    __API_PRG_GET_PTR(program, CL_PROGRAM_SOURCE, code);
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
__api_knl_set_args_valist(hiknl_t k, va_list list) {
    cl_uint a;
    int *ikey;
    char tmp[__API_STR_SIZE];
    cl_kernel kernel = k->id;
#ifdef CL_VERSION_1_2
    for (a = 0; a < k->num_args; ++a) {
        __API_KNL_ARG_GET_PTR(kernel, a, CL_KERNEL_ARG_TYPE_NAME, tmp);
        if (strstr(tmp, "*")) {
            himem_t m = (himem_t)urb_tree_find(&hicl->mems, 
                                               va_arg(list, address_t), 
                                               __api_address_cmp)->value;
            urb_t *n;
            if ((n = urb_tree_find(&k->mems, 
                                   &a, __api_int_cmp)) == &urb_sentinel) {
                HICL_DEBUG("mem insert {h=%p, id=%p} for kernel %s", 
                           m->h, m->id, __api_knl_name(k->id));
                ikey  = (int*)malloc(sizeof(int));
                *ikey = a;
                urb_tree_put(&k->mems, urb_tree_create(ikey, (void*)m), 
                             __api_int_cmp);
                m->refs++;
                __api_knl_set_arg_cl_mem(k->id, a, &m->id);
            } else {
                if (m != (himem_t)n->value) {
                    ((himem_t)n->value)->refs = ((himem_t)n->value)->refs-1;
                    m->refs++;
                    HICL_DEBUG("mem modify {h=%p, id=%p, refs = %d} to"
                               " {h=%p, id=%p, refs = %d} for kernel %s", 
                                ((himem_t)n->value)->h, 
                                ((himem_t)n->value)->id, 
                                ((himem_t)n->value)->refs, 
                                m->h, m->id, m->refs, __api_knl_name(k->id));
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
            himem_t m = urb_tree_find(&hicl->mems, 
                                      va_arg(list, address_t), 
                                      __api_address_cmp)->value;
            urb_t *n;
            if ((n = urb_tree_find(&k->mems, &a, __api_int_cmp)) == 
                 &urb_sentinel) {
                HICL_DEBUG("insert {h=%p, id=%p} for kernel %s", 
                           m->h, m->id, __api_knl_name(k->id));
                ikey  = (int*)malloc(sizeof(int));
                *ikey = a;
                urb_tree_put(&k->mems, urb_tree_create(ikey, (void*)m), 
                             __api_int_cmp);
                m->refs++;
                __api_knl_set_arg_cl_mem(k->id, a, &m->id);
            } else {
                if (m != (himem_t)n->value) {
                    ((himem_t)n->value)->refs = ((himem_t)n->value)->refs-1;
                    m->refs++;
                    HICL_DEBUG("mem modify {h=%p, id=%p, refs = %d} to"
                               " {h=%p, id=%p, refs = %d} for kernel %s", 
                                ((himem_t)n->value)->h, 
                                ((himem_t)n->value)->id, 
                                ((himem_t)n->value)->refs, 
                                m->h, m->id, m->refs, __api_knl_name(k->id));
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
    cl_int ret;
    if ((ret=clEnqueueNDRangeKernel(queue, kernel, wrk,
                                    ofs, gws, lws, 
                                    0, NULL, NULL)) != CL_SUCCESS) {
        char tmp[__API_STR_SIZE];
        __API_KNL_GET_PTR(kernel, CL_KERNEL_FUNCTION_NAME, tmp);
        HICL_ABORT(ret, "failed to run OpenCL kernel %s", tmp);
    }
    clFinish(queue);
}

PRIVATE void
__api_knl_async_run(cl_kernel kernel, cl_command_queue queue,
		            cl_uint wrk, const size_t *gws, const size_t *lws, 
		            const size_t *ofs) {
    cl_int ret;
    if ((ret=clEnqueueNDRangeKernel(queue, kernel, wrk,
                                    ofs, gws, lws, 
                                    0, NULL, NULL)) != CL_SUCCESS) {
        char tmp[__API_STR_SIZE];
        __API_KNL_GET_PTR(kernel, CL_KERNEL_FUNCTION_NAME, tmp);
        HICL_ABORT(ret, "failed to run OpenCL kernel %s", tmp);
    }
}

///
/// @brief Create a hiCL kernel descriptor.
///
/// This routine creates a hiCL kernel descriptor, provided an OpenCL cl_kernel,
/// which contains information about the work size and also the offset, see 
/// types.h. 
/// @param id is the OpenCL cl_kernel.
/// @return a hiCL kernel descriptor.
///
PRIVATE hiknl_t 
__api_knl_init(cl_kernel id) {
    size_t idx;
    hiknl_t k = (hiknl_t)malloc(sizeof(struct __hiknl_t));
    k->id  = id;
    k->wrk = 0;
    for(idx=0; idx<__API_KNL_MAX_WORK_SIZE; ++idx) {
        k->gws[idx]=1;
        k->lws[idx]=1;
        k->ofs[idx]=0;
    }
    k->mems = &urb_sentinel;
    __API_KNL_GET(id, CL_KERNEL_NUM_ARGS, k->num_args);
    return k;
}

///
/// @brief Return a kernel descriptor provided a string.
///
/// This routine performs a lookup in the list of hiCL kernel descriptors based 
/// on a string provided by the user
/// @param name is the string that identifies the kernel descriptor.
/// @return
///
PRIVATE hiknl_t
__api_knl_find(const char *name) {
    ulist_t *i_knl;
    hiknl_t      k;
    char tmp[__API_STR_SIZE];
    HICL_EXIT_IF((name == NULL) || (strlen(name) == 0),
                 "OpenCL kernel name not valid");
    for (i_knl=hicl->knls; i_knl != NULL; i_knl=i_knl->next) {
        k = (hiknl_t)i_knl->data;
        __API_KNL_GET(k->id, CL_KERNEL_FUNCTION_NAME, tmp);
        if (!strcmp(name, tmp)) break;
    }
    HICL_EXIT_IF(i_knl == NULL, "OpenCL kernel '%s' not found", name);
    return k;
}

PRIVATE void
__api_knl_release(void *pointer) {
    hiknl_t k = (hiknl_t)pointer;
    if (k != NULL) {
#ifdef __API_DEBUG
        unsigned int nb_refs;
        __API_KNL_GET(k->id, CL_KERNEL_REFERENCE_COUNT, nb_refs);
        HICL_DEBUG("releasing OpenCL kernel %s (mem count = %lu)", 
                   __api_knl_name(k->id), urb_tree_size(&k->mems));
#endif // __API_DEBUG
        urb_tree_delete(&k->mems, __api_int_del, __api_mem_release);
        if (clReleaseKernel(k->id) != CL_SUCCESS)
            HICL_FAIL("failed to release OpenCL kernel");
        free(k); k = NULL;
    }
}

PRIVATE void
__api_knl_info(void *pointer) {
    hiknl_t k = (hiknl_t)pointer;
    unsigned int i;
    char tmp_0[__API_STR_SIZE], tmp_1[__API_STR_SIZE];
    __API_KNL_GET_PTR(k->id, CL_KERNEL_FUNCTION_NAME, tmp_0);
    __API_KNL_INFO_LEVEL_0("kernel {id=%p}", k->id);
    __API_KNL_INFO_LEVEL_1("%s", "name", tmp_0);
    __API_KNL_INFO_LEVEL_1("%u", "num args", k->num_args);
#ifdef CL_VERSION_1_2
    if (k->num_args > 0) {
        __API_KNL_ARG_GET_PTR(k->id, 0, CL_KERNEL_ARG_NAME, tmp_0);
        __API_KNL_ARG_GET_PTR(k->id, 0, CL_KERNEL_ARG_TYPE_NAME, tmp_1);
        __API_KNL_INFO_LEVEL_1("%-2d - %-10s (%s)", "args:", 0, tmp_0, tmp_1);
        for (i=1; i<k->num_args; ++i) {
            __API_KNL_ARG_GET_PTR(k->id, i, CL_KERNEL_ARG_NAME, tmp_0);
            __API_KNL_ARG_GET_PTR(k->id, i, CL_KERNEL_ARG_TYPE_NAME, tmp_1);
            __API_KNL_INFO_LEVEL_2("%-2d - %-10s (%s)", i, tmp_0, tmp_1);
        }
    }
#endif // CL_VERSION_1_2
    __API_KNL_INFO_LEVEL_0("kernel %s has %lu memory objects", 
                           __api_knl_name(k->id), urb_tree_size(&k->mems));
}

CPPGUARD_END();

#endif  // __API_KNL_INL_H_
