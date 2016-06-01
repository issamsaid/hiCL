#ifndef __API_CONFIG_OPENCL_H_
#define __API_CONFIG_OPENCL_H_

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#include <OpenCL/cl_ext.h>
#else
#include <CL/cl.h>
#include <CL/cl_ext.h>
#endif  // defined(__APPLE__) || defined(__MACOSX)

//#define CL_USE_DEPRECATED_OPENCL_1_1_APIS

#ifndef CL_MEM_USE_PERSISTENT_MEM_AMD
#define CL_MEM_USE_PERSISTENT_MEM_AMD CL_MEM_ALLOC_HOST_PTR
#endif

#endif  // __API_CONFIG_OPENCL_H_
