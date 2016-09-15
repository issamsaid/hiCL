///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
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
/// @file hicl_dev.c
/// @author Issam SAID
/// @brief The implementation of the hiCL devices manipulation routines.
///
#include <hiCL/dev.h>
#include <stdio.h>
#include <string.h>

#include "__api/config/util.h"
#include "__api/dev-inl.h"

hidev_t hicl_dev_find(flags_t flags) {
    ulist_t *i_dev;
    hidev_t      d;
    cl_int i_dev_idx = 0, idx;
    cl_device_type i_dev_type, type;
    flags_t hidev_type_flags  = flags & (__API_DEV_TYPE_MASK);
    flags_t dev_index_flags = flags & (__API_DEV_INDEX_MASK);
    HICL_EXIT_IF(!__API_DEV_CHECK_TYPE_FLAGS(hidev_type_flags), 
                 "invalid hiCL device type flags");
    HICL_EXIT_IF(!__API_DEV_CHECK_INDEX_FLAGS(dev_index_flags), 
                 "invalid hiCL device index flags");
    if (__API_FLAGS_HAVE(flags, DEFAULT)) return hicl->devs->data;
    if (hidev_type_flags) type = __api_dev_flags_to_type(hidev_type_flags);
    else type = CL_DEVICE_TYPE_ALL;
    
    if (dev_index_flags) idx = __api_dev_flags_to_index(dev_index_flags);
    else idx  = 0;
    
    HICL_DEBUG("hicl_dev_find: type  from flags %s", __API_DEV_TYPE_STR(type));
    HICL_DEBUG("hicl_dev_find: index from flags %u", idx);
    
    for (i_dev=hicl->devs; i_dev != NULL; i_dev=i_dev->next) {
        d = (hidev_t)i_dev->data;
        __API_DEV_GET(d->id, CL_DEVICE_TYPE, i_dev_type);
        if ((i_dev_type & type) && (i_dev_idx++ == idx)) return d;
    }
    HICL_EXIT("device not found (type = %s, index = %d)",
              __API_DEV_TYPE_STR(type), idx);
}

bool hicl_dev_support(hidev_t d, char* extension) {
    return __api_dev_extension_supported(d->id, extension);
}

void hicl_dev_wait(hidev_t d) { clFinish(d->queue); }
