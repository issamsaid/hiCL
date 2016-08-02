#ifndef __API_UTIL_INL_H_
#define __API_UTIL_INL_H_
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
/// @file __api/util-inl.h
/// @author Issam SAID
/// @brief Private functions used by the hiCL utilities.    
///
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "hiCL/types.h"
#include "__api/config/opencl.h"
#include "__api/config/private.h"
#include "__api/config/guard.h"
#include "__api/config/log.h"

#ifdef  __API_DEBUG
#define HICL_DEBUG(fmt,...)                                              \
    fprintf(hicl->fdout, C_PURPLE"[HICL DBG]: "fmt".\n"C_END, ##__VA_ARGS__)
#else 
#define HICL_DEBUG(fmt,...) 
#endif  // __API_DEBUG

#ifdef __API_VERBOSE                             
#define HICL_PRINT(fmt,...)                                             \
    fprintf(hicl->fdout,                                                  \
            C_GREEN"[HICL MSG]: "fmt".\n"C_END, ##__VA_ARGS__)
#define HICL_WARN(fmt,...)                                              \
    fprintf(hicl->fdout,                                                  \
            C_YELLOW"[HICL WRN]: "fmt".\n"C_END, ##__VA_ARGS__)
#define HICL_WARN_IF(predicate, fmt,...)                                \
    if (predicate) {                                                    \
        fprintf(hicl->fdout,                                              \
                C_YELLOW"[HICL WRN]: "fmt".\n"C_END, ##__VA_ARGS__);    \
    }
#else 
#define HICL_PRINT(fmt,...) 
#define HICL_WARN(fmt,...) 
#define HICL_WARN_IF(fmt,...) 
#endif  // __API_VERBOSE 

#define HICL_FAIL(fmt,...)                                              \
    {                                                                   \
        fprintf(stderr, C_RED"[HICL FATAL]: "fmt" @%s:%d.\n"C_END,      \
                ##__VA_ARGS__, __FILE__, __LINE__);                     \
        exit(EXIT_FAILURE);                                             \
    }

#define HICL_FAIL_IF(predicate, fmt,...)                                \
    if (predicate) {                                                    \
        fprintf(stderr, C_RED"[HICL FATAL]: "fmt" @%s:%d.\n"C_END,      \
                ##__VA_ARGS__, __FILE__, __LINE__);                     \
        exit(EXIT_FAILURE);                                             \
    }

#define HICL_ABORT(status, fmt,...)                                     \
    {                                                                   \
        if(status != CL_SUCCESS) {                                      \
            fprintf(stderr,                                             \
                    C_RED"[HICL ABORT(%s)]: "fmt" @%s:%d.\n"C_END,      \
                    __api_error_msg(status),                            \
                    ##__VA_ARGS__, __FILE__, __LINE__);                 \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    }

#define HICL_EXIT(fmt,...)                                              \
    {                                                                   \
        fprintf(hicl->fderr, C_RED"[HICL ERR]: "fmt" @%s:%d.\n"C_END,     \
            ##__VA_ARGS__, __FILE__, __LINE__);                         \
        hicl_release();                                                 \
        exit(EXIT_FAILURE);                                             \
    }

#define HICL_EXIT_IF(predicate, fmt,...)                                \
    if (predicate) {                                                    \
        fprintf(hicl->fderr, C_RED"[HICL ERR]: "fmt" @%s:%d.\n"C_END,     \
                ##__VA_ARGS__, __FILE__, __LINE__);                     \
        hicl_release();                                                 \
        exit(EXIT_FAILURE);                                             \
    }

#define HICL_CHECK(status, fmt,...)                                     \
    {                                                                   \
        if(status != CL_SUCCESS) {                                      \
            fprintf(hicl->fderr,                                          \
                    C_RED"[HICL ERR(%s)]: "fmt" @%s:%d.\n"C_END,        \
                    __api_error_msg(status),                            \
                    ##__VA_ARGS__, __FILE__, __LINE__);                 \
            hicl_release();                                             \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    }

#define __API_CONTEXT_GET(context, context_info, value)                 \
    HICL_CHECK(clGetContextInfo(context,                                \
                              context_info, sizeof(value),              \
                              &value, NULL),                            \
             "failed to query context info")

#define __API_CONTEXT_GET_PTR(context, context_info, value)             \
    HICL_CHECK(clGetContextInfo(context, context_info, sizeof(value),   \
                              value, NULL),                             \
             "failed to query context info")

CPPGUARD_BEGIN()

extern void hicl_release();
extern hienv_t hicl;

PRIVATE int
__api_address_cmp(address_t cur_ptr, address_t otr_ptr) {
    return (int64_t)cur_ptr - (int64_t)otr_ptr;
}

PRIVATE int
__api_int_cmp(int cur, int otr) {
    return cur - otr;
}

PRIVATE int
__api_knl_cmp(hiknl_t cur, hiknl_t otr) {
    return (int64_t)cur - (int64_t)otr;
}

PRIVATE int64_t 
__api_nmult64(int64_t val, int64_t q) {
    return val + q - 1 - (val - 1)%q;
}

PRIVATE void
__api_generate_filename(char *filename) {
    time_t tloc;
    struct tm *lc_time;
    time(&tloc);
    lc_time=localtime(&tloc);
    sprintf(filename, "%04d%02d%02d_%02d%02d",
            lc_time->tm_year + 1900, lc_time->tm_mon+1,
            lc_time->tm_mday, lc_time->tm_hour, lc_time->tm_min);
    printf("%s\n", filename);
}

PRIVATE char*
__api_strstep(char *dest, char *src, const char *delim) {
    size_t src_len = strlen(src);
    size_t dst_len = strcspn(src, delim);
    snprintf(dest, dst_len+1, "%s", src);
    if(src_len == dst_len) return NULL;
    else return src + dst_len + 1;
}

PRIVATE void
__api_strupper(char *s) {
    while (*s) {
        if ((*s >= 'a' ) && (*s <= 'z')) *s -= ('a'-'A');
        s++;
    }
}

PRIVATE size_t
__api_tell_file(const char* filename) {
    FILE* fd = NULL;
    size_t size;
    HICL_EXIT_IF((fd = fopen(filename, "rb"))==NULL,
               "file '%s' not found", filename);
    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    fclose(fd);
    return size;
}

PRIVATE void
__api_read_from_file(char* buffer, size_t size, const char* filename) {
    FILE* fd = NULL;
    HICL_EXIT_IF((fd = fopen(filename, "rb"))==NULL,
                "file '%s' not found", filename);
    if (fread(buffer, 1, size-1, fd) != (size-1)) {        
        fclose(fd);
        HICL_EXIT("couldn't read file '%s'", filename);
    }
    fclose(fd);
    buffer[size-1] = '\0';
}

PRIVATE const char*
__api_error_msg(int error) {
    switch (error) {
    case CL_SUCCESS:
        return "CL_SUCCESS";
    case CL_DEVICE_NOT_FOUND:
        return "CL_DEVICE_NOT_FOUND";
    case CL_DEVICE_NOT_AVAILABLE:
        return "CL_DEVICE_NOT_AVAILABLE";
	case CL_COMPILER_NOT_AVAILABLE:
        return "CL_COMPILER_NOT_AVAILABLE";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
        return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case CL_OUT_OF_RESOURCES:
        return "CL_OUT_OF_RESOURCES";
	case CL_OUT_OF_HOST_MEMORY:
        return "CL_OUT_OF_HOST_MEMORY";    
	case CL_PROFILING_INFO_NOT_AVAILABLE:
        return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case CL_MEM_COPY_OVERLAP:
        return "CL_MEM_COPY_OVERLAP";
	case CL_IMAGE_FORMAT_MISMATCH:
        return "CL_IMAGE_FORMAT_MISMATCH";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
        return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case CL_BUILD_PROGRAM_FAILURE:
        return "CL_BUILD_PROGRAM_FAILURE";
    case CL_MAP_FAILURE:
        return "CL_MAP_FAILURE";
	case CL_INVALID_VALUE:
        return "CL_INVALID_VALUE";
	case CL_INVALID_DEVICE_TYPE:
        return "CL_INVALID_DEVICE_TYPE";
	case CL_INVALID_PLATFORM:
        return "CL_INVALID_PLATFORM";
	case CL_INVALID_DEVICE:
        return "CL_INVALID_DEVICE";
	case CL_INVALID_CONTEXT:
        return "CL_INVALID_CONTEXT";
	case CL_INVALID_QUEUE_PROPERTIES:
        return "CL_INVALID_QUEUE_PROPERTIES";
	case CL_INVALID_COMMAND_QUEUE:
        return "CL_INVALID_COMMAND_QUEUE";
	case CL_INVALID_HOST_PTR:
        return "CL_INVALID_HOST_PTR";
	case CL_INVALID_MEM_OBJECT:
        return "CL_INVALID_MEM_OBJECT";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
        return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case CL_INVALID_IMAGE_SIZE:
        return "CL_INVALID_IMAGE_SIZE";
	case CL_INVALID_SAMPLER:
        return "CL_INVALID_SAMPLER";
	case CL_INVALID_BINARY:
        return "CL_INVALID_BINARY";
	case CL_INVALID_BUILD_OPTIONS:
        return "CL_INVALID_BUILD_OPTIONS";
	case CL_INVALID_PROGRAM:
        return "CL_INVALID_PROGRAM";
	case CL_INVALID_PROGRAM_EXECUTABLE:
        return "CL_INVALID_PROGRAM_EXECUTABLE";
	case CL_INVALID_KERNEL_NAME:
        return "CL_INVALID_KERNEL_NAME";
	case CL_INVALID_KERNEL_DEFINITION:
        return "CL_INVALID_KERNEL_DEFINITION";
	case CL_INVALID_KERNEL:
        return "CL_INVALID_KERNEL";
	case CL_INVALID_ARG_INDEX:
        return "CL_INVALID_ARG_INDEX";
	case CL_INVALID_ARG_VALUE:
        return "CL_INVALID_ARG_VALUE";
	case CL_INVALID_ARG_SIZE:
        return "CL_INVALID_ARG_SIZE";
	case CL_INVALID_KERNEL_ARGS:
        return "CL_INVALID_KERNEL_ARGS";
	case CL_INVALID_WORK_DIMENSION:
        return "CL_INVALID_WORK_DIMENSION";
	case CL_INVALID_WORK_GROUP_SIZE:
        return "CL_INVALID_WORK_GROUP_SIZE";
	case CL_INVALID_WORK_ITEM_SIZE:
        return "CL_INVALID_WORK_ITEM_SIZE";
	case CL_INVALID_GLOBAL_OFFSET:
        return "CL_INVALID_GLOBAL_OFFSET";
	case CL_INVALID_EVENT_WAIT_LIST:
        return "CL_INVALID_EVENT_WAIT_LIST";
	case CL_INVALID_EVENT:
        return "CL_INVALID_EVENT";
	case CL_INVALID_OPERATION:
        return "CL_INVALID_OPERATION";
	case CL_INVALID_GL_OBJECT:
        return "CL_INVALID_GL_OBJECT";
	case CL_INVALID_BUFFER_SIZE:
        return "CL_INVALID_BUFFER_SIZE";
	case CL_INVALID_MIP_LEVEL:
        return "CL_INVALID_MIP_LEVEL";
	default:
        return "OTHER";
    }
}

PRIVATE cl_ulong
__api_event_time(cl_event event) {
    cl_ulong start, end;
    cl_int cl_ret;
    cl_ret  = clGetEventProfilingInfo(event,
                                      CL_PROFILING_COMMAND_START,
                                      sizeof(start),
                                      &start, NULL);
    cl_ret |= clGetEventProfilingInfo(event,
                                      CL_PROFILING_COMMAND_END,
                                      sizeof(end),
                                      &end, NULL);
    HICL_CHECK(cl_ret, "failed to get profiling infos"); 
    return end-start;
}

CPPGUARD_END()

#endif  // __API_UTIL_INL_H_