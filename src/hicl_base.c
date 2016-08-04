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
/// @file hicl_base.c
/// @author Issam SAID
/// @brief The implementation of the main routines to setup hiCL.
///
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

hienv_t hicl = NULL;

void hicl_init(flags_t flags) {
    cl_int  cl_ret;
    unsigned int nb_platforms, nb_devices;
    unsigned int i;
    char tmp_0[__API_STR_SIZE];
    char tmp_1[__API_STR_SIZE];
    cl_platform_id *plt_ids;
    cl_device_id   *dev_ids;
#if defined(__API_MPI) && ! defined(__API_LOG_STD)
    char stime[__API_STR_SIZE];
    time_t now          = time(NULL);
    struct tm time_desc = *(localtime(&now));
    int mpi_on, mpi_rank;
#endif // __API_MPI
    
    ///
    /// if hicl == NULL means that the library is not initialized yet.
    /// This test ensures that hiCL is initialized only once.
    ///
    if (hicl == NULL) {
        hicl              = (hienv_t) malloc(sizeof(struct __hienv_t));
        hicl->platform_id = NULL;
        hicl->context     = NULL;
        hicl->devs        = NULL;
        hicl->knls        = NULL;
        //hicl->mems        = NULL;
#ifdef __API_LOG_STD 
        hicl->fdout = stdout;
        hicl->fderr = stderr;
#else
#ifdef  __API_MPI
        strftime(stime, __API_STR_SIZE, "%m%d%y%H%M%S", &time_desc);

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
        hicl->fdout = fopen(tmp_0, "w");
        if (hicl->fdout == NULL) {
            fprintf(stderr, "FATAL ERROR: failed to open hiCL output log file");
            exit(EXIT_FAILURE);
        }
        hicl->fderr = fopen(tmp_1, "w");
        if (hicl->fderr == NULL) {
            fprintf(stderr, "FATAL ERROR: failed to open hiCL error log file");
            exit(EXIT_FAILURE);
        }
#endif // __API_LOG_STD
    
        HICL_DEBUG("hicl_init: flags = %#016lx", flags);

        // prepare OpenCL resources lists.
        create_rbt_address_t_himem_t(&hicl->mems, 
                                    __api_address_cmp, 
                                    __api_mem_stdalone_release);
        // get the OpenCL desired platform otherwise exit.
        nb_platforms = __api_plt_count();
        HICL_EXIT_IF(nb_platforms == 0, "no OpenCL platform found");
        plt_ids      = 
            (cl_platform_id*)malloc(nb_platforms*sizeof(cl_platform_id));
        __api_plt_query(plt_ids, nb_platforms);
        hicl->platform_id = __api_plt_select(plt_ids, nb_platforms, flags);
        HICL_EXIT_IF(hicl->platform_id == NULL, 
                     "no '%s' OpenCL platform found", 
                     __API_PLT_TYPE_STR(flags & (__API_PLT_VENDOR_MASK)));
        __API_PLT_GET_PTR(hicl->platform_id, CL_PLATFORM_NAME, tmp_0);
        HICL_PRINT("1 OpenCL platform is loaded: %s (%s)", 
                    tmp_0, __API_PLT_TYPE_STR(flags));

        // try to load the OpenCL devices, otherwise fail to default.
        nb_devices = __api_dev_count(hicl->platform_id);
        HICL_EXIT_IF(nb_devices == 0, 
                     "no OpenCL devices found in platform %s", tmp_0);
        dev_ids    = (cl_device_id*)malloc(nb_devices*sizeof(cl_device_id));
        __api_dev_query(hicl->platform_id, dev_ids, nb_devices);
        nb_devices = __api_dev_select(dev_ids, nb_devices, flags);
        HICL_EXIT_IF(nb_devices == 0, 
                     "the desired '%s' OpenCL device cannot be found", 
                     __API_DEV_TYPE_STR(
                            __api_dev_flags_to_type(flags & 
                                    (__API_DEV_TYPE_MASK))));
        HICL_PRINT("%u OpenCL device%s %s loaded",
                   nb_devices,
                   nb_devices == 1 ? "" : "s",
                   nb_devices == 1 ? "is" : "are");
        for (i=0; i<nb_devices; ++i) {
            __API_DEV_GET_PTR(dev_ids[i], CL_DEVICE_NAME, tmp_1);
            HICL_PRINT("\t- %s", tmp_1);
        }
        
        // setup an OpenCL context.
        hicl->context = clCreateContext(NULL, nb_devices, 
                                      dev_ids, NULL, NULL, &cl_ret);
        HICL_CHECK(cl_ret, "failed to create an OpenCL context");
        
        // setup devices.
        for (i=0; i<nb_devices; ++i) {
            hicl_dev_init(dev_ids[i]);
        }
        // setup timer.
        hicl_timer_uset(__API_TIMER_UNIT);
        free(plt_ids);
        free(dev_ids);
    }
}

void hicl_release() {
    ///
    /// if hicl != NULL means that the library is not released yet.
    /// This test ensures that hiCL is released only once.
    ///
    if (hicl != NULL) {
        list_hiknl_t *tmp_knl, *i_knl = hicl->knls;
        list_hidev_t *tmp_dev, *i_dev = hicl->devs;
        delete_rbt_address_t_himem_t(&hicl->mems);
        while(i_knl != NULL) {
            tmp_knl = i_knl;
            HICL_DEBUG("releasing OpenCL kernel @ %p (%s)", 
                        i_knl->data->id, 
                        __api_knl_name(i_knl->data->id));
            /// delete_rbt_int_mem(&i_knl->data->mems);
            __api_knl_release(i_knl->data->id);
            free(i_knl->data); i_knl->data = NULL;
            i_knl = i_knl->next;
            list_delete_hiknl_t(&tmp_knl);
        }
        while(i_dev != NULL) {
            tmp_dev = i_dev;
            HICL_DEBUG("releasing OpenCL device @ %p", i_dev->data->id);
            __api_dev_release_queues(i_dev->data->queue);
            free(i_dev->data); i_dev->data = NULL;
            i_dev = i_dev->next;
            list_delete_hidev_t(&tmp_dev);
        }
        if(hicl->context) {
            HICL_DEBUG("releasing OpenCL context @ %p", hicl->context);
            if (clReleaseContext(hicl->context) != CL_SUCCESS)
                HICL_WARN("failed to release OpenCL context");
        }
        HICL_PRINT("OpenCL resources are released");
    
    #ifndef __API_LOG_STD
        if (fclose(hicl->fdout)) {
            fprintf(stderr, 
                    "FATAL ERROR: failed to close the hiCL output log file");
            free(hicl); hicl = NULL;
            exit(EXIT_FAILURE);
        }
        if (fclose(hicl->fderr)) {
            fprintf(stderr, 
                    "FATAL ERROR: failed to close the hiCL error log file");
            free(hicl); hicl = NULL;
            exit(EXIT_FAILURE);
        }
    #endif
        free(hicl); hicl = NULL;
    }
}

void hicl_load(const char *filename, const char *options_format, ...) {
    cl_program program;
    cl_int cl_ret;
    size_t idx, nb_kernels, code_size;
    char *code;
    cl_kernel *knl_ids;
    char options[__API_STR_SIZE];
    va_list args;
    va_start(args, options_format);
    vsprintf(options, options_format, args);
    va_end(args);
    code_size = __api_tell_file(filename);
    code      = (char*)malloc(code_size+1);
    __api_read_from_file(code, code_size+1, filename);
    program = clCreateProgramWithSource(hicl->context, 1,
                                        (const char**)&code, NULL, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL program");
    HICL_DEBUG("loading filename '%s' with options '%s' (program @ %p)",
               filename, options, program);
    knl_ids = __api_knl_create_from_program(program, options, &nb_kernels);
    for(idx=0; idx<nb_kernels; ++idx) hicl_knl_init(knl_ids[idx]);
    free(knl_ids);
    free(code);
    HICL_CHECK(clReleaseProgram(program), "failed to release OpenCL program");
}

void hicl_info() {
    list_hidev_t *i_dev;
    list_hiknl_t *i_knl;
    __api_plt_info(hicl->platform_id, hicl->fdout);
    for (i_dev=hicl->devs; i_dev != NULL; i_dev=i_dev->next)
        hicl_dev_info(i_dev->data);
    walk_value_rbt_address_t_himem_t(&hicl->mems, __api_mem_info);
    for (i_knl=hicl->knls; i_knl != NULL; i_knl=i_knl->next)
        __api_knl_info(i_knl->data->id, i_knl->data->num_args);
}
