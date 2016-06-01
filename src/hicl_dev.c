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
/// \file easyhicl_dev.c
/// \version $Id$
/// \brief Implements an OpenCL device descriptor.
///
#include "hiCL/dev.h"
#include <stdio.h>
#include <string.h>

#include "__api/config/util.h"
#include "__api/dev-inl.h"
#include "__api/list-inl.h"

GENERATE_LIST_BODY(dev);

dev hicl_dev_init(cl_device_id id) {
    cl_int cl_ret;
    dev d    = (dev)malloc(sizeof(dev_desc));
    d->id    = id; 
    d->queue = clCreateCommandQueue(cl->context, id,
                                    __API_USE_EVENTS ?
                                    CL_QUEUE_PROFILING_ENABLE : 0, &cl_ret);
    HICL_CHECK(cl_ret, "failed to create OpenCL queue");
    return d;
}

void hicl_dev_release(dev d) {
    remove_node_dev(&cl->devs, d);
}

dev hicl_dev_find(flags_t flags) {
    node_dev *nd;
    cl_int nd_idx = 0, idx;
    cl_device_type nd_type, type;
    if (flags == DEFAULT)
        return cl->devs.head->data;
    if (__API_DEV_FLAGS_HAVE_TYPE(flags))
        type = __api_dev_flags_to_type(flags);
    else
        type = CL_DEVICE_TYPE_ALL;
    if (__API_DEV_FLAGS_HAVE_INDEX(flags))
        idx  = __api_dev_flags_to_index(flags);
    else
        idx  = 0;
    
    HICL_DEBUG("hicl_dev_find: type  from flags %s",
             __API_DEV_TYPE_STR(type));
    HICL_DEBUG("hicl_dev_find: index from flags %u", idx);
    for (nd=cl->devs.head; nd != NULL; nd=nd->next) {
        __API_DEV_GET(nd->data->id, CL_DEVICE_TYPE, nd_type);
        if ((nd_type & type) && (nd_idx++ == idx)) return nd->data;
    }
    HICL_EXIT("device not found (type = %s, index = %d)",
            __API_DEV_TYPE_STR(type), idx);
}

bool hicl_dev_support(dev d, char* extension) {
    return __api_dev_extension_supported(d->id, extension);
}

void hicl_dev_info(dev d) { return __api_dev_info(d->id); }

void hicl_dev_wait(dev d) { clFinish(d->queue); }
