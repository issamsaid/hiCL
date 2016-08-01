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
!! @file m_hicl_mem_update.f90
!! @author Issam SAID
!! @brief This file implements the Fortran interface of the hiCL memory 
!! update routine.
!! @see hiCL/mem.h
!<
module m_hicl_mem_update
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    use m_hicl_mem_interface
   
    implicit none
    
    private
   
    public :: hicl_mem_update
    interface hicl_mem_update
        module procedure hicl_mem_update_int32_1d
        module procedure hicl_mem_update_int32_1d_allocatable
        module procedure hicl_mem_update_int64_1d
        module procedure hicl_mem_update_int64_1d_allocatable
        module procedure hicl_mem_update_float_1d
        module procedure hicl_mem_update_float_1d_allocatable
        module procedure hicl_mem_update_double_1d
        module procedure hicl_mem_update_double_1d_allocatable

        module procedure hicl_mem_update_int32_2d
        module procedure hicl_mem_update_int32_2d_allocatable
        module procedure hicl_mem_update_int64_2d
        module procedure hicl_mem_update_int64_2d_allocatable
        module procedure hicl_mem_update_float_2d
        module procedure hicl_mem_update_float_2d_allocatable
        module procedure hicl_mem_update_double_2d
        module procedure hicl_mem_update_double_2d_allocatable

        module procedure hicl_mem_update_int32_3d
        module procedure hicl_mem_update_int32_3d_allocatable
        module procedure hicl_mem_update_int64_3d
        module procedure hicl_mem_update_int64_3d_allocatable
        module procedure hicl_mem_update_float_3d
        module procedure hicl_mem_update_float_3d_allocatable
        module procedure hicl_mem_update_double_3d
        module procedure hicl_mem_update_double_3d_allocatable

        module procedure hicl_mem_update_int32_1d_pointer
        module procedure hicl_mem_update_int64_1d_pointer
        module procedure hicl_mem_update_float_1d_pointer
        module procedure hicl_mem_update_double_1d_pointer

        module procedure hicl_mem_update_int32_2d_pointer
        module procedure hicl_mem_update_int64_2d_pointer
        module procedure hicl_mem_update_float_2d_pointer
        module procedure hicl_mem_update_double_2d_pointer

        module procedure hicl_mem_update_int32_3d_pointer
        module procedure hicl_mem_update_int64_3d_pointer
        module procedure hicl_mem_update_float_3d_pointer
        module procedure hicl_mem_update_double_3d_pointer
    end interface hicl_mem_update

contains
    
    subroutine hicl_mem_push(h, ix, ex, iy, ey, iz, ez, xp, yp, blocking)
        real,   allocatable, target, intent(in) :: h(:,:,:)
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
        real,   allocatable, target, intent(in) :: h(:,:,:)
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
    !!  hicl_mem_update 1d
    !!
    subroutine hicl_mem_update_int32_1d(h, size_x, flags)
        integer(kind=4), target, intent(in) :: h(size_x)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_1d

    subroutine hicl_mem_update_int32_1d_allocatable(h, flags)
        integer(kind=4), allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t),              intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_1d_allocatable

    subroutine hicl_mem_update_int64_1d(h, size_x, flags)
        integer(kind=8), target, intent(in) :: h(size_x)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_1d

    subroutine hicl_mem_update_int64_1d_allocatable(h, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t),      intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_1d_allocatable

    subroutine hicl_mem_update_float_1d(h, size_x, flags)
        real,            target, intent(in) :: h(size_x)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
     end subroutine hicl_mem_update_float_1d

    subroutine hicl_mem_update_float_1d_allocatable(h, flags)
        real,       allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_float_1d_allocatable
    
    subroutine hicl_mem_update_double_1d(h, size_x, flags)
        real(kind=8),    target, intent(in) :: h(size_x)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_1d

    subroutine hicl_mem_update_double_1d_allocatable(h, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:)
        integer(kind=c_int64_t),           intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_1d_allocatable
    
    !!
    !!  hicl_mem_update 2d
    !!
    subroutine hicl_mem_update_int32_2d(h, size_x, size_y, flags)
        integer(kind=4), target, intent(in) :: h(size_x, size_y)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=4),         intent(in) :: size_y
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_2d

    subroutine hicl_mem_update_int32_2d_allocatable(h, flags)
        integer,    allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t),         intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_2d_allocatable

    subroutine hicl_mem_update_int64_2d(h, size_x, size_y, flags)
        integer(kind=8), target, intent(in) :: h(size_x, size_y)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=4),         intent(in) :: size_y
        integer(kind=c_int64_t),      intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_2d

    subroutine hicl_mem_update_int64_2d_allocatable(h, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t),      intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_2d_allocatable

    subroutine hicl_mem_update_float_2d(h, size_x, size_y, flags)
        real,            target, intent(in) :: h(size_x, size_y)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=4),         intent(in) :: size_y
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_float_2d

    subroutine hicl_mem_update_float_2d_allocatable(h, flags)
        real,       allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_float_2d_allocatable
    
    subroutine hicl_mem_update_double_2d(h, size_x, size_y, flags)
        real(kind=8),      target, intent(in) :: h(size_x, size_y)
        integer(kind=4),           intent(in) :: size_x
        integer(kind=4),           intent(in) :: size_y
        integer(kind=c_int64_t),   intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_2d

    subroutine hicl_mem_update_double_2d_allocatable(h, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:,:)
        integer(kind=c_int64_t),   intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_2d_allocatable

    !!
    !!  hicl_mem_update 3d
    !!
    subroutine hicl_mem_update_int32_3d(h, size_x, size_y, size_z, flags)
        integer,         target, intent(in) :: h(size_x, size_y, size_z)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=4),         intent(in) :: size_y
        integer(kind=4),         intent(in) :: size_z
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_3d

    subroutine hicl_mem_update_int32_3d_allocatable(h, flags)
        integer,    allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int32_3d_allocatable

    subroutine hicl_mem_update_int64_3d(h, size_x, size_y, size_z, flags)
        integer(kind=8), target, intent(in) :: h(size_x, size_y, size_z)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=4),         intent(in) :: size_y
        integer(kind=4),         intent(in) :: size_z
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_3d

    subroutine hicl_mem_update_int64_3d_allocatable(h, flags)
        integer(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_int64_3d_allocatable

    subroutine hicl_mem_update_float_3d(h, size_x, size_y, size_z, flags)
        real,            target, intent(in) :: h(size_x, size_y, size_z)
        integer(kind=4),         intent(in) :: size_x
        integer(kind=4),         intent(in) :: size_y
        integer(kind=4),         intent(in) :: size_z
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_float_3d

    subroutine hicl_mem_update_float_3d_allocatable(h, flags)
        real,       allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t), intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_float_3d_allocatable

    subroutine hicl_mem_update_double_3d(h, size_x, size_y, size_z, flags)
        real(kind=8),      target, intent(in) :: h(size_x, size_y, size_z)
        integer(kind=4),           intent(in) :: size_x
        integer(kind=4),           intent(in) :: size_y
        integer(kind=4),           intent(in) :: size_z
        integer(kind=c_int64_t),   intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_3d

    subroutine hicl_mem_update_double_3d_allocatable(h, flags)
        real(kind=8), allocatable, target, intent(in) :: h(:,:,:)
        integer(kind=c_int64_t),   intent(in) :: flags
        call c_hicl_mem_update(c_loc(h), flags)
    end subroutine hicl_mem_update_double_3d_allocatable

    !!
    !!  hicl_mem_update pointers
    !!
    subroutine hicl_mem_update_int32_1d_pointer(h, flags)
        integer(kind=4), pointer, dimension(:), intent(in) :: h
        integer(kind=c_int64_t),                intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1))), flags)
    end subroutine hicl_mem_update_int32_1d_pointer

    subroutine hicl_mem_update_int64_1d_pointer(h, flags)
        integer(kind=8), pointer, dimension(:), intent(in) :: h
        integer(kind=c_int64_t),                intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1))), flags)
    end subroutine hicl_mem_update_int64_1d_pointer

    subroutine hicl_mem_update_float_1d_pointer(h, flags)
        real(kind=4), pointer, dimension(:), intent(in) :: h
        integer(kind=c_int64_t),             intent(in) :: flags
        write(*,*) lbound(h)
        call c_hicl_mem_update(c_loc(h(lbound(h, 1))), flags)
    end subroutine hicl_mem_update_float_1d_pointer

    subroutine hicl_mem_update_double_1d_pointer(h, flags)
        real(kind=8), pointer, dimension(:), intent(in) :: h
        integer(kind=c_int64_t),             intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1))), flags)
    end subroutine hicl_mem_update_double_1d_pointer


    subroutine hicl_mem_update_int32_2d_pointer(h, flags)
        integer(kind=4), pointer, dimension(:,:), intent(in) :: h
        integer(kind=c_int64_t),                  intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2))), flags)
    end subroutine hicl_mem_update_int32_2d_pointer

    subroutine hicl_mem_update_int64_2d_pointer(h, flags)
        integer(kind=8), pointer, dimension(:,:), intent(in) :: h
        integer(kind=c_int64_t),                  intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2))), flags)
    end subroutine hicl_mem_update_int64_2d_pointer

    subroutine hicl_mem_update_float_2d_pointer(h, flags)
        real(kind=4), pointer, dimension(:,:), intent(in) :: h
        integer(kind=c_int64_t),               intent(in) :: flags
        write(*,*) lbound(h)
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2))), flags)
    end subroutine hicl_mem_update_float_2d_pointer

    subroutine hicl_mem_update_double_2d_pointer(h, flags)
        real(kind=8), pointer, dimension(:,:), intent(in) :: h
        integer(kind=c_int64_t),               intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2))), flags)
    end subroutine hicl_mem_update_double_2d_pointer


    subroutine hicl_mem_update_int32_3d_pointer(h, flags)
        integer(kind=4), pointer, dimension(:,:,:), intent(in) :: h
        integer(kind=c_int64_t),                    intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2), &
                                       lbound(h, 3))), flags)
    end subroutine hicl_mem_update_int32_3d_pointer

    subroutine hicl_mem_update_int64_3d_pointer(h, flags)
        integer(kind=8), pointer, dimension(:,:,:), intent(in) :: h
        integer(kind=c_int64_t),                    intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2), &
                                       lbound(h, 3))), flags)
    end subroutine hicl_mem_update_int64_3d_pointer

    subroutine hicl_mem_update_float_3d_pointer(h, flags)
        real(kind=4), pointer, dimension(:,:,:), intent(in) :: h
        integer(kind=c_int64_t),                 intent(in) :: flags
        write(*,*) lbound(h)
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2), &
                                       lbound(h, 3))), flags)
    end subroutine hicl_mem_update_float_3d_pointer

    subroutine hicl_mem_update_double_3d_pointer(h, flags)
        real(kind=8), pointer, dimension(:,:,:), intent(in) :: h
        integer(kind=c_int64_t),                 intent(in) :: flags
        call c_hicl_mem_update(c_loc(h(lbound(h, 1), &
                                       lbound(h, 2), &
                                       lbound(h, 3))), flags)
    end subroutine hicl_mem_update_double_3d_pointer
end module m_hicl_mem_update