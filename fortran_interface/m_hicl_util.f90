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
!! @file m_hicl_util.f90
!! @author Issam SAID
!! @brief This file implements the Fortran interface of the hiCL utilities.
!! @see hiCL/util.h
!<
module m_hicl_util
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    implicit none

    private

    interface
        logical(kind=c_bool) &
        function hicl_has(flags) bind(c, name="hicl_has")
            use, intrinsic :: iso_c_binding, only: c_bool, c_int64_t
            implicit none
            integer(kind=c_int64_t), intent(in), value :: flags 
        end function hicl_has

        integer(kind=c_int) &
        function hicl_count(flags) bind(c, name="hicl_count")
            use, intrinsic :: iso_c_binding, only: c_int64_t, c_int
            implicit none
            integer(kind=c_int64_t), intent(in), value :: flags 
        end function hicl_count

        integer(kind=c_int) &
        function hicl_knl_count() bind(c, name="hicl_knl_count")
            use, intrinsic :: iso_c_binding, only: c_int
            implicit none
        end function

        integer(kind=c_int) &
        function hicl_mem_count() bind(c, name="hicl_mem_count")
            use, intrinsic :: iso_c_binding, only: c_int
            implicit none
        end function        
    end interface

    public :: hicl_has
    public :: hicl_count
    public :: hicl_knl_count
    public :: hicl_mem_count
 
end module m_hicl_util