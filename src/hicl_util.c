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
/// @file hicl_util.c
/// @author Issam SAID
/// @brief The implementation of the hiCL utilities.
///
#include "hiCL/util.h"
#include "__api/plt-inl.h"
#include "__api/dev-inl.h"
#include "__api/mem-inl.h"
#include "__api/knl-inl.h"
#include "__api/util-inl.h"
#include <string.h>

extern hienv_t hicl;

flags_t hicl_str_to_flags_t(const char* str) {
    flags_t flags;
    if (strcmp(str, "HWA") == 0)
        flags = HWA;
    else if (strcmp(str, "PINNED") == 0)
        flags = PINNED;
    else if (strcmp(str, "ZERO_COPY") == 0)
        flags = ZERO_COPY;
    else if (strcmp(str, "READ_ONLY") == 0)
        flags = READ_ONLY;
    else if (strcmp(str, "WRITE_ONLY") == 0)
        flags = WRITE_ONLY;
    else if (strcmp(str, "READ_WRITE") == 0)
        flags = READ_WRITE;
    else
        HICL_EXIT("unknown flags_t");
    return flags;
}

bool hicl_has(flags_t flags) {
    return hicl_count(flags) > 0;
}

unsigned int hicl_count(flags_t flags) {
    cl_platform_id id, *plt_ids = NULL;
    cl_device_id *dev_ids = NULL;
    unsigned int n, nb_platforms, nb_devices;
    nb_platforms = __api_plt_count();
    if (nb_platforms == 0) return 0;
    plt_ids      = (cl_platform_id*)malloc(nb_platforms*sizeof(cl_platform_id));
    __api_plt_query(plt_ids, nb_platforms);
    id = __api_plt_select(plt_ids, nb_platforms, flags);
    if (id == NULL) return 0; 
    if (flags & (__API_PLT_VENDOR_MASK)) return 1;
    nb_devices = __api_dev_count(id);
    dev_ids    = (cl_device_id*)malloc(nb_devices*sizeof(cl_device_id));
    __api_dev_query(id, dev_ids, nb_devices);
    n = __api_dev_select(dev_ids, nb_devices, flags);
    free(dev_ids);
    free(plt_ids);
    return n;
}

unsigned int hicl_mem_count() {
    if (hicl != NULL) return hicl->mems.size;
    else return 0;
}

unsigned int hicl_knl_count() {
    list_hiknl_t *i_knl;
    unsigned int n = 0;
    for (i_knl=hicl->knls; i_knl != NULL; i_knl=i_knl->next) n++;
    return n;
}

///
/// This is a workaround since some compilers such as the Intel
/// compiler generates a runtime error if a variadic C function
/// is interoperable with Fortran.
/// 
void hicl_load_f(const char *filename, const char *options) {
   hicl_load(filename, options);
}
