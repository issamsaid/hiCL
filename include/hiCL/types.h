#ifndef HICL_TYPES_H_
#define HICL_TYPES_H_
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
/// \file types.h
/// \version $Id: types.h 2408 2014-05-12 14:13:50Z issam $
/// \brief OpenCL definitions used in hicl.
///
#include <stdio.h>
#include <stdint.h>
#include "__api/config/opencl.h"
#include "__api/config/guard.h"
#include "__api/config/knl.h"
#include "__api/list-inl.h"
#include "__api/rbt-inl.h"

CPPGUARD_BEGIN()

typedef uint64_t flags_t;
typedef void     *address;
typedef struct dev_desc_st dev_desc, *dev;
typedef struct mem_desc_st mem_desc, *mem;
typedef struct knl_desc_st knl_desc, *knl;
typedef struct agt_desc_st agt_desc, *agt;

GENERATE_LIST_HEADER(dev);
GENERATE_LIST_HEADER(knl);
GENERATE_RBT_HEADER(address, mem);
GENERATE_RBT_HEADER(int, mem);
GENERATE_RBT_HEADER(knl, int);

struct dev_desc_st {
    cl_device_id id;
    cl_command_queue queue;
};

struct mem_desc_st {
    flags_t flags;
    size_t size;
    size_t unit_size;
    cl_mem id;
    cl_mem pinned;
    cl_command_queue queue;
    void* h;
    rbt_knl_int knls;
};

struct knl_desc_st {
    size_t gws[__API_KNL_MAX_WORK_SIZE];
    size_t lws[__API_KNL_MAX_WORK_SIZE];
    size_t ofs[__API_KNL_MAX_WORK_SIZE];
    cl_uint wrk;
    cl_uint num_args;
    cl_kernel id;
    rbt_int_mem mems;
};

// OpenCL agt.
struct agt_desc_st {
    // OpenCL default platform ID, we only handle one platform.  
    cl_platform_id platform_id;
    // OpenCL context. 
    cl_context context;
    // OpenCL devices.
    list_dev devs;
    // OpenCL kernels.
    list_knl knls;
    // OpenCL memory objects.
    rbt_address_mem mems;
    FILE* fdout;
    FILE* fderr;
};

CPPGUARD_END()

#endif  // HICL_TYPES_H_
