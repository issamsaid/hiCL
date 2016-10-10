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
/// @file stencil_3d/main.c
/// @author Issam SAID
/// @brief An example of 3D finite difference stencil code based on 
/// the hiCL C/C++ interface.
///
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <hiCL/hiCL.h>

#define N 320

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
    float *u;
    float *v;
    float *coeffs;
    
    size_t global[3] = {N,   N, 1};
    size_t local[3]  = {16, 16, 1};
    
    hidev_t device;

    fprintf(stdout, "... start of the hiCL 3D stencil C/C++ example\n");

    ///
    ///< Initialize hiCL with selecting the default GPU.
    ///
    hicl_init(GPU);

    ///
    ///< Load and build the OpenCL kernel that runs the stencil computations.
    ///< Note the '-cl-kernel-arg-info' that is added for NVIDIA GPUs to 
    ///< use some OpenCL 1.2 features.
    ///
    hicl_load(PREFIX"/stencil_3d.cl", NULL);

    ///
    ///< Get a pointer to the desired device (in this case the default GPU).
    ///
    device = hicl_dev_find(DEFAULT);

    u      = (float*)malloc((N+8)*(N+8)*(N+8)*sizeof(float));
    v      = (float*)malloc((N+8)*(N+8)*(N+8)*sizeof(float));
    coeffs = (float*)malloc(5*sizeof(float));

    memset(u,   0, (N+8)*(N+8)*(N+8)*sizeof(float));
    srand (time(NULL));
#pragma omp parallel for private(i)
    for (i = 0; i< (N+8)*(N+8)*(N+8); ++i) v[i] = (float)(i%100);

    coeffs[0] = -2.6;
    coeffs[1] =  1.6;
    coeffs[2] = -0.2;
    coeffs[3] =  0.02;
    coeffs[4] = -0.001;

    /// 
    ///< Wrap the matrices into hiCL memory objects.
    ///
    hicl_mem_wrap(device, u, (N+8)*(N+8)*(N+8), FLOAT | READ_WRITE  | HWA);
    hicl_mem_wrap(device, v, (N+8)*(N+8)*(N+8), FLOAT | READ_WRITE  | HWA);
    hicl_mem_wrap(device, coeffs, 5, FLOAT | READ_ONLY | HWA);

    ///
    ///< Set the work size an dimension of the kernel.
    ///
    hicl_knl_set_wrk("stencil_3d", 2, global, local);

    /// 
    ///< Run the kernel on the default GPU.
    ///
    hicl_knl_run("stencil_3d", device, v, u, coeffs, N, N, N);

    ///
    ///< Update the u buffer on the CPU side so that the results can be seen
    ///< on the host side.
    ///
    hicl_mem_update(u, READ_ONLY);

    free(u);
    free(v);
    free(coeffs);

    ///
    ///< Release hiCL resources.
    ///
    hicl_release();
    fprintf(stdout, "... end   of the hiCL 3D stencil C/C++ example\n");
    return EXIT_SUCCESS;
}
