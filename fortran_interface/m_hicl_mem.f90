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
!! @file m_hicl_mem.f90
!! @author Issam SAID
!! @brief This file implements the Fortran interface of the hiCL memory 
!! manipulation routines.
!! @see hiCL/mem.h
!<
module m_hicl_mem
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    implicit none
    
    private

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
        end subroutine c_hicl_mem_push

        subroutine c_hicl_mem_info(c_ptr_m) &
            bind(c, name="hicl_mem_info")
            use, intrinsic :: iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_ptr_m
        end subroutine c_hicl_mem_info
    end interface

    public :: hicl_mem_wrap
    interface hicl_mem_wrap
        module procedure hicl_mem_wrap_int32_1d
        module procedure hicl_mem_wrap_int32_2d
        module procedure hicl_mem_wrap_int32_3d
        module procedure hicl_mem_wrap_int64_1d
        module procedure hicl_mem_wrap_int64_2d
        module procedure hicl_mem_wrap_int64_3d
        module procedure hicl_mem_wrap_float_1d
        module procedure hicl_mem_wrap_float_2d
        module procedure hicl_mem_wrap_float_3d
        module procedure hicl_mem_wrap_double_1d
        module procedure hicl_mem_wrap_double_2d
        module procedure hicl_mem_wrap_double_3d
    end interface hicl_mem_wrap

    public :: hicl_mem_release
    interface hicl_mem_release
        module procedure hicl_mem_release_int32_1d
        module procedure hicl_mem_release_int64_1d
        module procedure hicl_mem_release_float_1d
        module procedure hicl_mem_release_double_1d
        module procedure hicl_mem_release_int32_2d
        module procedure hicl_mem_release_int64_2d
        module procedure hicl_mem_release_float_2d
        module procedure hicl_mem_release_double_2d
        module procedure hicl_mem_release_int32_3d
        module procedure hicl_mem_release_int64_3d
        module procedure hicl_mem_release_float_3d
        module procedure hicl_mem_release_double_3d
    end interface hicl_mem_release

    public :: hicl_mem_pop
    public :: hicl_mem_push
    public :: hicl_mem_htod
    public :: hicl_mem_dtoh
    public :: hicl_mem_update
    interface hicl_mem_update
        module procedure hicl_mem_update_int32_1d
        module procedure hicl_mem_update_int64_1d
        module procedure hicl_mem_update_float_1d
        module procedure hicl_mem_update_double_1d
        module procedure hicl_mem_update_int32_2d
        module procedure hicl_mem_update_int64_2d
        module procedure hicl_mem_update_float_2d
        module procedure hicl_mem_update_double_2d
        module procedure hicl_mem_update_int32_3d
        module procedure hicl_mem_update_int64_3d
        module procedure hicl_mem_update_float_3d
        module procedure hicl_mem_update_double_3d
    end interface hicl_mem_update

    public :: hicl_mem_info
    interface hicl_mem_info
        module procedure hicl_mem_info_int32_1d
        module procedure hicl_mem_info_int64_1d
        module procedure hicl_mem_info_float_1d
        module procedure hicl_mem_info_double_1d
        module procedure hicl_mem_info_int32_2d
        module procedure hicl_mem_info_int64_2d
        module procedure hicl_mem_info_float_2d
        module procedure hicl_mem_info_double_2d
        module procedure hicl_mem_info_int32_3d
        module procedure hicl_mem_info_int64_3d
        module procedure hicl_mem_info_float_3d
        module procedure hicl_mem_info_double_3d
    end interface hicl_mem_info

contains

    !!
    !! hicl_mem_wrap
    !!
    subroutine hicl_mem_wrap_int32_1d(h, d, flags)
        integer(kind=4), allocatable, target ,intent(in) :: h(:)
        type(hidev_t),                   pointer, intent(in) :: d
        integer(kind=c_int64_t),              intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_int32_1d

    subroutine hicl_mem_wrap_int64_1d(h, d, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:)
        type(hidev_t),                   pointer, intent(in) :: d
        integer(kind=c_int64_t),              intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_int64_1d

    subroutine hicl_mem_wrap_float_1d(h, d, flags)
        real(kind=4), allocatable, target, intent(in) :: h(:)
        type(hidev_t),                pointer, intent(in) :: d
        integer(kind=c_int64_t),           intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_float_1d

    subroutine hicl_mem_wrap_double_1d(h, d, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:)
        type(hidev_t),                pointer, intent(in) :: d
        integer(kind=c_int64_t),           intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_double_1d

    subroutine hicl_mem_wrap_int32_2d(h, d, flags)
        integer(kind=4), allocatable, target ,intent(in) :: h(:,:)
        type(hidev_t),                   pointer, intent(in) :: d
        integer(kind=c_int64_t),              intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_int32_2d

    subroutine hicl_mem_wrap_int64_2d(h, d, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:)
        type(hidev_t),                   pointer, intent(in) :: d
        integer(kind=c_int64_t),              intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_int64_2d

    subroutine hicl_mem_wrap_float_2d(h, d, flags)
        real(kind=4), allocatable, target, intent(in) :: h(:,:)
        type(hidev_t),                pointer, intent(in) :: d
        integer(kind=c_int64_t),           intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_float_2d

    subroutine hicl_mem_wrap_double_2d(h, d, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:,:)
        type(hidev_t),                pointer, intent(in) :: d
        integer(kind=c_int64_t),           intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_double_2d

    subroutine hicl_mem_wrap_int32_3d(h, d, flags)
        integer(kind=4), allocatable, target ,intent(in) :: h(:,:,:)
        type(hidev_t),                   pointer, intent(in) :: d
        integer(kind=c_int64_t),              intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)*size(h, 3)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_int32_3d

    subroutine hicl_mem_wrap_int64_3d(h, d, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        type(hidev_t),                   pointer, intent(in) :: d
        integer(kind=c_int64_t),              intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)*size(h, 3)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_int64_3d

    subroutine hicl_mem_wrap_float_3d(h, d, flags)
        real(kind=4), allocatable, target, intent(in) :: h(:,:,:)
        type(hidev_t),                pointer, intent(in) :: d
        integer(kind=c_int64_t),           intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)*size(h, 3)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_float_3d

    subroutine hicl_mem_wrap_double_3d(h, d, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:, :, :)
        type(hidev_t),                pointer, intent(in) :: d
        integer(kind=c_int64_t),           intent(in) :: flags
        integer(kind=c_size_t) :: s8
        type(c_ptr) :: tmp
        s8  = size(h, 1)*size(h, 2)*size(h, 3)
        tmp = c_hicl_mem_wrap(c_loc(d), c_loc(h), s8, flags)
    end subroutine hicl_mem_wrap_double_3d

    !!
    !!  hicl_mem_release
    !!
    subroutine hicl_mem_release_int32_1d(h)
        integer(kind=4), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_int32_1d

    subroutine hicl_mem_release_int64_1d(h)
        integer(kind=8), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_int64_1d

    subroutine hicl_mem_release_float_1d(h)
        real(kind=4), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_float_1d

    subroutine hicl_mem_release_double_1d(h)
        real(kind=8), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_double_1d

    subroutine hicl_mem_release_int32_2d(h)
        integer(kind=4), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_int32_2d

    subroutine hicl_mem_release_int64_2d(h)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_int64_2d

    subroutine hicl_mem_release_float_2d(h)
        real(kind=4), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_float_2d

    subroutine hicl_mem_release_double_2d(h)
        real(kind=8), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_double_2d

    subroutine hicl_mem_release_int32_3d(h)
        integer(kind=4), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_int32_3d

    subroutine hicl_mem_release_int64_3d(h)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_int64_3d

    subroutine hicl_mem_release_float_3d(h)
        real(kind=4), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_float_3d

    subroutine hicl_mem_release_double_3d(h)
        real(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_release(c_loc(h))
    end subroutine hicl_mem_release_double_3d


    subroutine hicl_mem_htod(h, blocking)
        real,  allocatable, target, intent(in) :: h(:,:,:)
        logical                                :: blocking
        logical(kind=c_bool)                   :: cblocking
        cblocking = blocking
        call c_hicl_mem_htod(c_loc(h), cblocking)
    end subroutine hicl_mem_htod

    subroutine hicl_mem_dtoh(h, blocking)
        real,  allocatable, target, intent(in) :: h(:,:,:)
        logical,                    intent(in) :: blocking
        logical(kind=c_bool)                   :: cblocking
        cblocking = blocking
        call c_hicl_mem_dtoh(c_loc(h), cblocking)
    end subroutine hicl_mem_dtoh
    
    subroutine hicl_mem_push(h, ix, ex, iy, ey, iz, ez, xp, yp, blocking)
        real,       allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=4),             intent(in) :: ix
        integer(kind=4),             intent(in) :: ex
        integer(kind=4),             intent(in) :: iy
        integer(kind=4),             intent(in) :: ey
        integer(kind=4),             intent(in) :: iz
        integer(kind=4),             intent(in) :: ez
        integer(kind=4),             intent(in) :: xp
        integer(kind=4),             intent(in) :: yp
        logical(kind=c_bool),        intent(in) :: blocking
        call c_hicl_mem_push(c_loc(h), ix, ex, iy, ey, iz, ez, &
                            xp, yp, blocking)
    end subroutine hicl_mem_push

    subroutine hicl_mem_pop(h, ix, ex, iy, ey, iz, ez, xp, yp, blocking)
        real,       allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=4),             intent(in) :: ix
        integer(kind=4),             intent(in) :: ex
        integer(kind=4),             intent(in) :: iy
        integer(kind=4),             intent(in) :: ey
        integer(kind=4),             intent(in) :: iz
        integer(kind=4),             intent(in) :: ez
        integer(kind=4),             intent(in) :: xp
        integer(kind=4),             intent(in) :: yp
        logical(kind=c_bool),        intent(in) :: blocking
        call c_hicl_mem_pop(c_loc(h), ix, ex, iy, ey, iz, ez, &
                            xp, yp, blocking)
    end subroutine hicl_mem_pop

    !!
    !!  hicl_mem_update
    !!
    subroutine hicl_mem_update_int32_1d(h, flags)
        integer, allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_1d

    subroutine hicl_mem_update_int64_1d(h, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t),      intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_1d

    subroutine hicl_mem_update_float_1d(h, flags)
        real,       allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
     end subroutine hicl_mem_update_float_1d

    subroutine hicl_mem_update_double_1d(h, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t),   intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_1d

    subroutine hicl_mem_update_int32_2d(h, flags)
        integer,    allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_2d

    subroutine hicl_mem_update_int64_2d(h, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t),      intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_2d

    subroutine hicl_mem_update_float_2d(h, flags)
        real,       allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_float_2d

    subroutine hicl_mem_update_double_2d(h, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t),   intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_2d

    subroutine hicl_mem_update_int32_3d(h, flags)
        integer,    allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_3d

    subroutine hicl_mem_update_int64_3d(h, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_3d

    subroutine hicl_mem_update_float_3d(h, flags)
        real,       allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_float_3d

    subroutine hicl_mem_update_double_3d(h, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t),   intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_3d

    !!
    !!  hicl_mem_info
    !!
    subroutine hicl_mem_info_int32_1d(h)
        integer(kind=4), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_int32_1d

    subroutine hicl_mem_info_int64_1d(h)
        integer(kind=8), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_int64_1d

    subroutine hicl_mem_info_float_1d(h)
        real(kind=4), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_float_1d

    subroutine hicl_mem_info_double_1d(h)
        real(kind=8), allocatable, target, intent(in) :: h(:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_double_1d

    subroutine hicl_mem_info_int32_2d(h)
        integer(kind=4), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_int32_2d

    subroutine hicl_mem_info_int64_2d(h)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_int64_2d

    subroutine hicl_mem_info_float_2d(h)
        real(kind=4), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_float_2d

    subroutine hicl_mem_info_double_2d(h)
        real(kind=8), allocatable, target, intent(in) :: h(:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_double_2d

    subroutine hicl_mem_info_int32_3d(h)
        integer(kind=4), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_int32_3d

    subroutine hicl_mem_info_int64_3d(h)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_int64_3d

    subroutine hicl_mem_info_float_3d(h)
        real(kind=4), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_float_3d

    subroutine hicl_mem_info_double_3d(h)
        real(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        call c_hicl_mem_info(c_loc(h))
    end subroutine hicl_mem_info_double_3d

end module m_hicl_mem