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
!! @file m_hicl_flags.f90
!! @author Issam SAID
!! @brief This file implements the hiCL flags in Fortran.
!! @see hiCL/flags.h
!<
module m_hicl_flags
  use, intrinsic :: iso_c_binding

  implicit none

  private

  integer(kind=c_int64_t), &
        parameter, public :: FIRST           = ishft(1_c_int64_t, 1)
  integer(kind=c_int64_t), &
        parameter, public :: SECOND          = ishft(1_c_int64_t, 2)
  integer(kind=c_int64_t), &
        parameter, public :: THIRD           = ishft(1_c_int64_t, 3)
  integer(kind=c_int64_t), &
        parameter, public :: FOURTH          = ishft(1_c_int64_t, 4)
  integer(kind=c_int64_t), &
        parameter, public :: FIFTH           = ishft(1_c_int64_t, 5)
  integer(kind=c_int64_t), &
        parameter, public :: SIXTH           = ishft(1_c_int64_t, 6)
  integer(kind=c_int64_t), &
        parameter, public :: SEVENTH         = ishft(1_c_int64_t, 7)
  integer(kind=c_int64_t), &
        parameter, public :: EIGHTH          = ishft(1_c_int64_t, 8)
  !! reserved ishft(1_c_int64_t, 9)
  !! reserved ishft(1_c_int64_t, 10)
  !! reserved ishft(1_c_int64_t, 11)
  integer(kind=c_int64_t), &
        parameter, public :: CPU              = ishft(1_c_int64_t, 12)
  integer(kind=c_int64_t), &
        parameter, public :: GPU              = ishft(1_c_int64_t, 13)
  integer(kind=c_int64_t), &
        parameter, public :: ACCELERATOR      = ishft(1_c_int64_t, 14)
  integer(kind=c_int64_t), & 
        parameter, public :: ALL              = ishft(1_c_int64_t, 15)
  integer(kind=c_int64_t), &
        parameter, public :: AMD              = ishft(1_c_int64_t, 16)
  integer(kind=c_int64_t), &
        parameter, public :: APPLE            = ishft(1_c_int64_t, 17)
  integer(kind=c_int64_t), &
        parameter, public :: INTEL            = ishft(1_c_int64_t, 18)
  integer(kind=c_int64_t), &
        parameter, public :: NVIDIA           = ishft(1_c_int64_t, 19)
  !! reserved ishft(1_c_int64_t, 20)
  !! reserved ishft(1_c_int64_t, 21)
  !! reserved ishft(1_c_int64_t, 22)
  !! reserved ishft(1_c_int64_t, 23)
  !! reserved ishft(1_c_int64_t, 24)
  !! reserved ishft(1_c_int64_t, 25)
  !! reserved ishft(1_c_int64_t, 26)
  integer(kind=c_int64_t), &
        parameter, public :: HWA              = ishft(1_c_int64_t, 27)
  integer(kind=c_int64_t), &
        parameter, public :: PINNED           = ishft(1_c_int64_t, 28)
  integer(kind=c_int64_t), &
        parameter, public :: ZERO_COPY        = ishft(1_c_int64_t, 29)
  integer(kind=c_int64_t), &
        parameter, public :: HOST_ALLOCATED   = ishft(1_c_int64_t, 30)
  integer(kind=c_int64_t), &
        parameter, public :: READ_ONLY        = ishft(1_c_int64_t, 31)
  integer(kind=c_int64_t), &
        parameter, public :: WRITE_ONLY       = ishft(1_c_int64_t, 32)
  integer(kind=c_int64_t), &
        parameter, public :: READ_WRITE       = ishft(1_c_int64_t, 33)
  integer(kind=c_int64_t), &
        parameter, public :: CHAR             = ishft(1_c_int64_t, 34)
  integer(kind=c_int64_t), &
        parameter, public :: INT              = ishft(1_c_int64_t, 35)
  integer(kind=c_int64_t), &
        parameter, public :: UNSIGNED_INT     = ishft(1_c_int64_t, 36)
  integer(kind=c_int64_t), &
        parameter, public :: LONG             = ishft(1_c_int64_t, 37)
  integer(kind=c_int64_t), &
        parameter, public :: UNSIGNED_LONG    = ishft(1_c_int64_t, 38)
  integer(kind=c_int64_t), &
        parameter, public :: SIZET            = ishft(1_c_int64_t, 39)
  integer(kind=c_int64_t), &
        parameter, public :: FLOAT            = ishft(1_c_int64_t, 40)
  integer(kind=c_int64_t), &
        parameter, public :: DOUBLE           = ishft(1_c_int64_t, 41)
  !! ...
  integer(kind=c_int64_t), &
        parameter, public :: DEFAULT          = ishft(1_c_int64_t, 53)

end module m_hicl_flags
