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
!! @file m_hicl_timer.f90
!! @author Issam SAID
!! @brief This file implements the Fortran interface of the hiCL timer.
!! @see hiCL/timer.h
!<
module m_hicl_timer
    use, intrinsic :: iso_c_binding
    use m_hicl_types
    
    implicit none
  
    private

    interface
        real(c_double) &
        function c_hicl_timer_read() bind(c, name = "hicl_timer_read")
            use, intrinsic :: iso_c_binding, only: c_double
        end function c_hicl_timer_read
    end interface

    public :: hicl_timer_uset
    public :: hicl_timer_tick
    
    interface
        subroutine hicl_timer_uset(unit) bind(c, name = "hicl_timer_uset")
            use, intrinsic :: iso_c_binding, only: c_int
            integer(kind=c_int), value, intent(in) :: unit
        end subroutine hicl_timer_uset
                
        subroutine hicl_timer_tick() bind(c, name = "hicl_timer_tick")
        end subroutine hicl_timer_tick
    end interface
    
    public :: hicl_timer_read
    
contains
  
    subroutine hicl_timer_read(time)
        real(kind=8), intent(inout) :: time
        time = c_hicl_timer_read()
    end subroutine hicl_timer_read

end module m_hicl_timer
