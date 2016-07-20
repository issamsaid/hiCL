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
!! @file m_hicl_dev.f90
!! @author Issam SAID
!! @brief This file implements the Fortran interface of the hiCL device 
!! manipulation routines.
!! @see hiCL/dev.h
!<
module m_hicl_dev
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    implicit none
    
    private

    interface
        type(c_ptr) &
        function c_hicl_dev_init(dev_id) bind(c, name="hicl_dev_init")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: dev_id
        end function c_hicl_dev_init

        subroutine c_hicl_dev_release(c_dev_ptr) bind(c, name="hicl_dev_release")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_dev_ptr            
        end subroutine c_hicl_dev_release

        type(c_ptr) &
        function c_hicl_dev_find(flags) bind(c, name="hicl_dev_find")
            use iso_c_binding, only: c_ptr, c_int64_t
            integer(kind=c_int64_t), value, intent(in) :: flags
        end function c_hicl_dev_find

        subroutine c_hicl_dev_info(c_dev_ptr) bind(c, name="hicl_dev_info")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_dev_ptr
        end subroutine c_hicl_dev_info

        subroutine c_hicl_dev_wait(c_dev_ptr) bind(c, name = "hicl_dev_wait")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_dev_ptr
        end subroutine c_hicl_dev_wait

        logical(kind=c_bool) &
        function c_hicl_dev_support(c_dev_ptr, extension) & 
            bind(c, name="hicl_dev_support")
            use iso_c_binding, only: c_bool, c_char, c_ptr
            type(c_ptr),     value, intent(in) :: c_dev_ptr
            character(kind=c_char), intent(in) :: extension
        end function c_hicl_dev_support
    end interface

    public :: hicl_dev_init
    public :: hicl_dev_release
    public :: hicl_dev_find
    public :: hicl_dev_info
    public :: hicl_dev_wait
    public :: hicl_dev_support

contains

    subroutine hicl_dev_init(d, dev_id)
        type(c_ptr),        value, intent(in) :: dev_id
        type(hidev_t), pointer, intent(inout) :: d
        call c_f_pointer(c_hicl_dev_init(dev_id), d)
    end subroutine hicl_dev_init

    subroutine hicl_dev_release(d) 
        type(hidev_t), pointer, intent(inout) :: d
        call c_hicl_dev_release(c_loc(d))
    end subroutine hicl_dev_release

    subroutine hicl_dev_find(flags, d)
        integer(kind=c_int64_t),  value, intent(in) :: flags
        type(hidev_t),       pointer, intent(inout) :: d
        call c_f_pointer(c_hicl_dev_find(flags), d)
    end subroutine hicl_dev_find

    subroutine hicl_dev_info(d)
        type(hidev_t), pointer, intent(in) :: d
        call c_hicl_dev_info(c_loc(d))
    end subroutine hicl_dev_info
    
    subroutine hicl_dev_wait(d)
        type(hidev_t), pointer, intent(in) :: d
        call c_hicl_dev_wait(c_loc(d))
    end subroutine hicl_dev_wait

    logical(kind=c_bool) &
    function hicl_dev_support (d, extension) result(status)
        type(hidev_t), pointer, intent(in) :: d
        character(len=*),      intent(in) :: extension
        status = c_hicl_dev_support(c_loc(d), &
                                    extension // c_null_char)
    end function hicl_dev_support

end module m_hicl_dev
