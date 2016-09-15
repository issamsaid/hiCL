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
/// @file sgemm/main.c
/// @author Issam SAID
/// @brief An example of matrix to matrix multiplication code based on 
/// the hiCL C/C++ interface.
///
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "hiCL/hiCL.h"

#define N 1024

///
/// @brief The main program of the hiCL based sgemm C/C++ example.
///
/// This is the main routine that shows how to use the hiCL C/C++ interface 
/// to implement a simple matrix to matrix multiplication.
/// Note that the OpenCL kernel is implemented in a seperate file (sgemm.cl).
/// @return Error code if any.
///
int main(void) {
    unsigned int i;
    float  *a;
    float  *b;
    float  *c;     

    size_t global[3] = {N,   N, 1};
    size_t local[3]  = {16, 16, 1};
    
    hidev_t device;

    fprintf(stdout, "... start of the hiCL sgemm C/C++ example\n");

    ///
    ///< Initialize hiCL with selecting the default GPU.
    ///
    hicl_init(GPU);

    ///
    ///< Load and build the OpenCL kernel that runs the multiplication.
    ///< Note the '-cl-kernel-arg-info' that is added for NVIDIA GPUs to 
    ///< use some OpenCL 1.2 features.
    ///
    hicl_load("sgemm.cl", "-cl-kernel-arg-info");

    ///
    ///< Get a pointer to the desired device (in this case the default GPU).
    ///
    device = hicl_dev_find(DEFAULT);

    a = (float*)malloc(N*N*sizeof(float));
    b = (float*)malloc(N*N*sizeof(float));
    c = (float*)malloc(N*N*sizeof(float));

    memset(c,   0, N*N*sizeof(float));
    srand (time(NULL));
#pragma omp parallel for private(i)
    for (i = 0; i< N*N; ++i) a[i]  = i%2 == 0 ? -rand()%10 : rand()%10;
#pragma omp parallel for private(i)
    for (i = 0; i< N*N; ++i) b[i]  = 1;

    /// 
    ///< Wrap the matrices into hiCL memory objects.
    ///
    hicl_mem_wrap(device, a, N*N, FLOAT | READ_ONLY  | HWA);
    hicl_mem_wrap(device, b, N*N, FLOAT | READ_ONLY  | HWA);
    hicl_mem_wrap(device, c, N*N, FLOAT | READ_WRITE | HWA);

    ///
    ///< Set the work size and the dimensions of the kernel.
    ///
    hicl_knl_set_wrk("sgemm", 2, global, local);

    /// 
    ///< Run the kernel on the default GPU.
    ///
    hicl_knl_run("sgemm", device, a, b, c, N);

    ///
    ///< Update the C matrix on the CPU side so that the results can be seen
    ///< on the host side.
    ///
    hicl_mem_update(c, READ_ONLY);

    free(a);
    free(b);
    free(c);

    ///
    ///< Release hiCL resources.
    ///
    hicl_release();
    fprintf(stdout, "... end   of the hiCL sgemm C/C++ example\n");
    return EXIT_SUCCESS;
}