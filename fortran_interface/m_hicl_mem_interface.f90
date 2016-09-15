!>
!! @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
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
!! @file m_hicl_mem_interface.f90
!! @author Issam SAID
!! @brief This file implements the Fortran interface of the hiCL memory 
!! manipulation routines.
!! @see hiCL/mem.h
!<
module m_hicl_mem_interface
    use, intrinsic :: iso_c_binding
    
    implicit none
    
    public

    interface
        type(c_ptr) &
        function c_hicl_mem_wrap(c_dev_ptr, c_h, size, flags) &
            bind(c, name="hicl_mem_wrap")
            use, intrinsic :: iso_c_binding,  only: c_ptr, c_size_t, c_int64_t
            type(c_ptr),             value, intent(in) :: c_dev_ptr
            type(c_ptr),             value, intent(in) :: c_h
            integer(kind=c_size_t),  value, intent(in) :: size
            integer(kind=c_int64_t), value, intent(in) :: flags
        end function c_hicl_mem_wrap

        subroutine c_hicl_mem_release(c_ptr_m) &
            bind(c, name="hicl_mem_release")
            use, intrinsic :: iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_ptr_m
        end subroutine c_hicl_mem_release

        subroutine c_hicl_mem_update(c_address, mode) &
            bind(c, name="hicl_mem_update")
            use, intrinsic  :: iso_c_binding
            type(c_ptr),             value, intent(in) :: c_address
            integer(kind=c_int64_t), value, intent(in) :: mode
        end subroutine c_hicl_mem_update
        
        subroutine c_hicl_mem_htod(c_address, blocking) &
            bind(c, name="hicl_mem_htod")
            use, intrinsic  :: iso_c_binding
            type(c_ptr),          value, intent(in) :: c_address
            logical(kind=c_bool), value, intent(in) :: blocking
        end subroutine
        
        subroutine c_hicl_mem_dtoh(c_address, blocking) &
            bind(c, name="hicl_mem_dtoh")
            use, intrinsic  :: iso_c_binding
            type(c_ptr),          value, intent(in) :: c_address
            logical(kind=c_bool), value, intent(in) :: blocking
        end subroutine

        subroutine c_hicl_mem_pop(c_address,              &
                                  ix, ex, iy, ey, iz, ez, &
                                  xpitch, ypitch, blocking) &
            bind(c, name="hicl_mem_pop")
            use, intrinsic  :: iso_c_binding, only: c_ptr, c_int, c_bool
            type(c_ptr),         value, intent(in) :: c_address
            integer(kind=c_int), value, intent(in) :: ix
            integer(kind=c_int), value, intent(in) :: ex
            integer(kind=c_int), value, intent(in) :: iy
            integer(kind=c_int), value, intent(in) :: ey
            integer(kind=c_int), value, intent(in) :: iz
            integer(kind=c_int), value, intent(in) :: ez
            integer(kind=c_int), value, intent(in) :: xpitch
            integer(kind=c_int), value, intent(in) :: ypitch
            logical(kind=c_bool), value, intent(in) :: blocking
        end subroutine c_hicl_mem_pop

        subroutine c_hicl_mem_push(c_address,             &
                                  ix, ex, iy, ey, iz, ez, &
                                  xpitch, ypitch, blocking) &
            bind(c, name="hicl_mem_push")
            use, intrinsic  :: iso_c_binding, only: c_ptr, c_int, c_bool
            type(c_ptr),          value, intent(in) :: c_address
            integer(kind=c_int),  value, intent(in) :: ix
            integer(kind=c_int),  value, intent(in) :: ex
            integer(kind=c_int),  value, intent(in) :: iy
            integer(kind=c_int),  value, intent(in) :: ey
            integer(kind=c_int),  value, intent(in) :: iz
            integer(kind=c_int),  value, intent(in) :: ez
            integer(kind=c_int),  value, intent(in) :: xpitch
            integer(kind=c_int),  value, intent(in) :: ypitch
            logical(kind=c_bool), value, intent(in) :: blocking
        end subroutine c_hicl_mem_push
    end interface

end module m_hicl_mem_interface