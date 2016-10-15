!>
!! @copyright Copyright (c) 2013-2016, Université Pierre et Marie Curie
!! All rights reserved.
!!
!! <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
!! funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
!!
!! Redistribution and use in source and binary forms, with or without
!! modification, are permetted provided that the following conditions
!! are met:
!!
!! 1. Redistributions of source code must retain the above copyright
!!    notice, this list of conditions and the following disclaimer.
!! 2. Redistributions in binary form must reproduce the above copyright
!!    notice, this list of conditions and the following disclaimer inthe
!!    documentation and/or other materials provided with the distribution.
!! 3. Neither the name of the UPMC nor the names of its contributors
!!    may be used to endorse or promote products derived from this software
!!    without specific prior written permission.
!!
!! THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
!! INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
!! FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
!! ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
!! EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
!! PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
!! PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
!! LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
!! NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
!! SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
!!
!! @file sgemm/main.f90
!! @author Issam SAID 
!! @brief An example of a 3D finite difference stencil code based on 
!! the hiCL Fortran interface.
!!
!<
program stencil_3d_fortran
	use m_hicl

    integer(kind=8),    parameter :: N = 320
    integer(kind=4)               :: n4

    real,             allocatable :: u(:,:,:) 
    real,             allocatable :: v(:,:,:) 
    real,             allocatable :: coeffs(:) 

    integer(kind=8), dimension(3) :: global = (/N, N, 1_8/)
    integer(kind=8), dimension(3) :: local  = (/16_8, 16_8, 1_8/)

    type(hidev_t),        pointer :: device

	write(*,*) "... start of the hiCL 3D stencil Fortran example"

    !!
    !!< Initialize hiCL with selecting the default GPU.
    !!
    call hicl_init(GPU)

    !!                 
    !!< Load and build the OpenCL kernel that runs the multiplication.
    !!< Note the '-cl-kernel-arg-info' that is added for NVIDIA GPUs to 
    !!< use some OpenCL 1.2 features.
    !! 
    call hicl_load(PREFIX//"/stencil_3d.cl", "-cl-kernel-arg-info");

	!!
    !!< Get a pointer to the desired device (in this case the default GPU).
    !!
    call hicl_dev_find(DEFAULT, device)

    allocate(u(N+8, N+8, N+8))
    allocate(v(N+8, N+8, N+8))
    allocate(coeffs(5))
    u  = 0
    v  = 2.1
    n4 = N

    coeffs(1) = -2.6;
    coeffs(2) =  1.6;
    coeffs(3) = -0.2;
    coeffs(4) =  0.02;
    coeffs(5) = -0.001;

    !!
    !!< Wrap the buffers into hiCL memory objects.
    !!
    call hicl_mem_wrap(u, device, ior(HWA, READ_WRITE))
    call hicl_mem_wrap(v, device, ior(HWA, READ_WRITE))
    call hicl_mem_wrap(coeffs, device, ior(HWA, READ_ONLY))
    
    !!
    !!< Set the work size and the dimensions of the kernel.
    !!
    call hicl_knl_set_wrk("stencil_3d", 2, global, local)
    
    !!
    !!< Set the arguments of the kernel.
    !!< Unfortunately variadic functions cannot be used in Fortran, each 
    !!< argument has to be set apart. 
    !!
    call hicl_knl_set("stencil_3d", 0, v)
    call hicl_knl_set("stencil_3d", 1, u)
    call hicl_knl_set("stencil_3d", 2, coeffs)
    call hicl_knl_set("stencil_3d", 3, n4)
    call hicl_knl_set("stencil_3d", 4, n4)
    call hicl_knl_set("stencil_3d", 5, n4)
        
    !! 
    !!< Run the kernel on the default GPU.
    !!
    call hicl_knl_exec("stencil_3d", device);
    
    !!
    !!< Update the u buffer on the CPU side so that the results can be seen
    !!< on the host side.
    !!
    call hicl_mem_update(u, READ_ONLY)

    deallocate(u, v, coeffs)

    !!
    !!< Release hiCL resources.
    !!
    call hicl_release()
    write(*,*) "... end   of the hiCL 3D stencil Fortran example"
end program stencil_3d_fortran