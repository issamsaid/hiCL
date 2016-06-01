// Copyright (c) 2016, TOTAL
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. All advertising materials mentioning features or use of this software
//    must display the following acknowledgement:
//    This product includes software developed by the <organization>.
// 4. Neither the name of the <organization> nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "hiCL/base.h"
#include "hiCL/dev.h"
#include "hiCL/mem.h"
#include "hiCL/knl.h"
#include "hiCL/timer.h"

#include "__api/config/dev.h"
#include "__api/config/knl.h"
#include "__api/config/timer.h"

#include "__api/plt-inl.h"
#include "__api/dev-inl.h"
#include "__api/mem-inl.h"
#include "__api/knl-inl.h"
#include<time.h>
#ifdef __API_MPI
#include<mpi.h>
#endif // __API_MPI

agt cl = NULL;

void hicl_init(flags_t flags) {
    cl_int  cl_ret;
    unsigned int nb_platforms, nb_devices;
    unsigned int i, f;
    char tmp_0[__API_STR_SIZE];
    char tmp_1[__API_STR_SIZE];
    char stime[__API_STR_SIZE];
    cl_platform_id *plt_ids;
    cl_device_id   *dev_ids;
    time_t now          = time(NULL);
    struct tm time_desc = *(localtime(&now));

    strftime(stime, __API_STR_SIZE, "%m%d%y%H%M%S", &time_desc);

#if defined(__API_MPI) && ! defined(__API_LOG_STD)
    int mpi_on, mpi_rank;
#endif // __API_MPI
    
    if (cl == NULL) cl = (agt) malloc(sizeof(agt_desc));
    
    // prepare OpenCL logging outputs.
#ifdef __API_LOG_STD 
    cl->fdout = stdout;
    cl->fderr = stderr;
#else
#ifdef  __API_MPI
    MPI_Initialized(&mpi_on);
    if (mpi_on) {
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
        sprintf(tmp_0, "hicl_log_%s_%d.out", stime, mpi_rank);
        sprintf(tmp_1, "hicl_log_%s_%d.err", stime, mpi_rank);
    } else {
        sprintf(tmp_0, "hicl_log_%s.out", stime);
        sprintf(tmp_1, "hicl_log_%s.err", stime);
    }
#else
    sprintf(tmp_0, "hicl_log.out");
    sprintf(tmp_1, "hicl_log.err");
#endif // __API_MPI
    cl->fdout = fopen(tmp_0, "w");
    if (cl->fdout == NULL) {
        fprintf(stderr, "FATAL ERROR: failed to open hiCL output log file");
        exit(EXIT_FAILURE);
    }
    cl->fderr = fopen(tmp_1, "w");
    if (cl->fderr == NULL) {
        fprintf(stderr, "FATAL ERROR: failed to open hiCL error log file");
        exit(EXIT_FAILURE);
    }
#endif // __API_LOG_STD

    HICL_DEBUG("hicl_init: flags = %#016lx", flags);

    // prepare OpenCL resources lists.
    create_list_dev(&cl->devs, __api_dev_release);
    create_list_knl(&cl->knls, __api_knl_release);
    create_rbt_address_mem(&cl->mems, 
                           __api_address_cmp, __api_mem_stdalone_release);

    // get the OpenCL desired platform otherwise exit.
    nb_platforms = __api_plt_count();
    plt_ids      = (cl_platform_id*)malloc(nb_platforms*sizeof(cl_platform_id));
    cl->platform_id = __api_plt_select(plt_ids, nb_platforms, flags);
    __API_PLT_GET_PTR(cl->platform_id, CL_PLATFORM_NAME, tmp_0);
    HICL_PRINT("1 OpenCL platform is loaded: %s (%s)", 
                tmp_0, __API_PLT_TYPE_STR(flags));

    // try to load the OpenCL devices, otherwise fail to default.
    nb_devices = __api_dev_count(cl->platform_id);
    dev_ids    = (cl_device_id*)malloc(nb_devices*sizeof(cl_device_id));
    nb_devices = __api_dev_select(cl->platform_id, dev_ids, nb_devices, flags);
    HICL_PRINT("%u OpenCL device%s %s loaded",
               nb_devices,
               nb_devices == 1 ? "" : "s",
               nb_devices == 1 ? "is" : "are");
    for (i=0; i<nb_devices; ++i) {
        __API_DEV_GET_PTR(dev_ids[i], CL_DEVICE_NAME, tmp_1);
        HICL_PRINT("\t- %s", tmp_1);
    }
    
    // setup an OpenCL context.
    cl->context = clCreateContext(NULL, nb_devices, 
                                  dev_ids, NULL, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create an OpenCL context");
    
    // setup devices.
    for (i=0; i<nb_devices; ++i) {
        insert_node_dev(&cl->devs, hicl_dev_init(dev_ids[i]));
    }
    // setup timer.
    hicl_timer_uset(__API_TIMER_UNIT);
    free(plt_ids);
    free(dev_ids);
}

void hicl_release() {
    HICL_PRINT("OpenCL resources are released");
    if (cl != NULL) {
        delete_rbt_address_mem(&cl->mems);
        delete_list_knl(&cl->knls);
        delete_list_dev(&cl->devs);
        if(cl->context) {
            HICL_DEBUG("releasing OpenCL context @ %p", cl->context);
            if (clReleaseContext(cl->context) != CL_SUCCESS)
                HICL_WARN("failed to release OpenCL context");
        }
    }
#ifndef __API_LOG_STD
    if (fclose(cl->fdout)) {
        fprintf(stderr, "FATAL ERROR: failed to close hicl output log file");
        free(cl); cl = NULL;
        exit(EXIT_FAILURE);
    }
    if (fclose(cl->fderr)) {
        fprintf(stderr, "FATAL ERROR: failed to close hicl error log file");
        free(cl); cl = NULL;
        exit(EXIT_FAILURE);
    }
#endif
    free(cl); cl = NULL;
}

void hicl_load(const char *filename, const char *options) {
    cl_program program;
    cl_int cl_ret;
    size_t idx, nb_kernels, code_size = __api_tell_file(filename);
    char code[code_size];
    cl_kernel *knl_ids;
    __api_read_from_file(code, code_size, filename);
    const char *const_code = code;
    program = clCreateProgramWithSource(cl->context, 1,
                                        &const_code, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL program");
    HICL_DEBUG("loading filename '%s' with options '%s' (program @ %p)",
               filename, options, program);
    knl_ids = __api_knl_create_from_program(program, options, &nb_kernels);
    for(idx=0; idx<nb_kernels; ++idx)
        insert_node_knl(&cl->knls, hicl_knl_init(knl_ids[idx]));
    free(knl_ids);
    HICL_CHECK(clReleaseProgram(program), "failed to release OpenCL program");
}

void hicl_info() {
    node_dev *nd;
    node_knl *nk;
    __api_plt_info(cl->platform_id);
    for (nd=cl->devs.head; nd != NULL; nd=nd->next)
        hicl_dev_info(nd->data);
    walk_value_rbt_address_mem(&cl->mems, __api_mem_info);
    for (nk=cl->knls.head; nk != NULL; nk=nk->next)
        hicl_knl_info(nk->data);
}