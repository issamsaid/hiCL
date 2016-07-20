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
!! @file m_hicl_base
!! @author Issam SAID
!! @brief This file implements the Fortran interface of the hiCL main setup 
!! routines.
!! @see hiCL/base.h
!<
module m_hicl_base
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    implicit none

    private

    interface
        subroutine c_hicl_init(flags) bind(c, name = "hicl_init")
            use, intrinsic :: iso_c_binding, only: c_int64_t
            implicit none
            integer(kind=c_int64_t), intent(in), value :: flags
        end subroutine c_hicl_init

        subroutine c_hicl_load(filename, options) bind(c, name = "hicl_load")
            use, intrinsic :: iso_c_binding, only: c_char
            implicit none
            character(kind=c_char), intent(in) :: filename
            character(kind=c_char), intent(in) :: options
        end subroutine c_hicl_load
    end interface

    interface
        subroutine hicl_release() bind(c, name = "hicl_release")
        end subroutine hicl_release

        subroutine hicl_info() bind(c, name = "hicl_info")
        end subroutine hicl_info 
    end interface

    public :: hicl_init
    public :: hicl_load
    public :: hicl_release
    public :: hicl_info

contains
    
    subroutine hicl_init(flags)
        integer(kind=c_int64_t), optional, intent(in) :: flags
        if (present(flags)) then
            call c_hicl_init(flags)
        else
            call c_hicl_init(DEFAULT)
        end if
    end subroutine hicl_init
    
    subroutine hicl_load(filename, options)
        character(len=*),           intent(in) :: filename
        character(len=*), optional, intent(in) :: options
        if (present(options)) then
            call c_hicl_load(filename // c_null_char, options // c_null_char)
        else
            call c_hicl_load(filename // c_null_char, c_null_char)
        end if
    end subroutine hicl_load
    
end module m_hicl_base