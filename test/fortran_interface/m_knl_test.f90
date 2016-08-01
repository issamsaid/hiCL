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
!! @file m_knl_test.f90
!! @author Issam SAID
!! @brief Unit testing file for the hiCL Fortran interface for OpenCL
!! kernels manipulation routines.
!<
module m_knl_test
    use iso_c_binding
    use m_hicl_flags
    use m_hicl_types
    use m_hicl_base
    use m_hicl_mem
    use m_hicl_dev
    use m_hicl_knl
    use m_hicl_util
    use m_handler
    
    implicit none

    private

    type(hidev_t),         pointer :: d
    integer(kind=4), parameter :: N = 64

    public :: knl_test

contains

    logical function info() result(status)
        implicit none
        call hicl_knl_info("test_hicl_1")
        status = (hicl_knl_count() > 0)
    end function info

    logical function build() result(status)
        implicit none
        call hicl_knl_build("test_hicl_1", "-DSTENCIL=29");
        status = (hicl_knl_count() == 6)
    end function build

    logical function set_wrk() result(status)
        implicit none
        integer(kind=8), dimension(3), parameter :: gws = (/256, 384, 128/)
        integer(kind=8), dimension(3), parameter :: lws = (/64, 64, 1/)
        call hicl_knl_set_wrk("test_hicl_1", 3, gws, lws)
        status = .true.
        ! status = ((k%gws(1) == 256) .and. &
        !           (k%gws(2) == 384) .and. &
        !           (k%gws(3) == 128) .and. &
        !           (k%lws(1) == 64)  .and. &
        !           (k%lws(2) == 64)  .and. &
        !           (k%lws(3) == 1))
    end function set_wrk

    logical function set_arg() result(status)
        implicit none
        integer, parameter :: i = 9
        call hicl_knl_set("test_hicl_3", 2, i)
        status = .true.
    end function set_arg

    logical function sync_run() result(status)
        implicit none
        integer                       :: i
        integer(kind=8), dimension(3) :: gws = (/1_8, 1_8, 1_8/)
        integer(kind=8), dimension(3) :: lws = (/1_8, 1_8, 1_8/)
        real, allocatable :: src(:) 
        real, allocatable :: dst(:) 

        gws(1) = N
        lws(1) = 64

        allocate(src(N), dst(N))
        src = 1.2
        
        call hicl_dev_find(DEFAULT, d)
        call hicl_mem_wrap(src, d, ior(HWA, READ_ONLY))
        call hicl_mem_wrap(dst, d, ior(HWA, WRITE_ONLY))
        call hicl_mem_update(src, WRITE_ONLY)
        status = .true.
        call hicl_knl_set_wrk("test_hicl_1", 1, gws, lws)
        call hicl_knl_set("test_hicl_1", 0, src)
        call hicl_knl_set("test_hicl_1", 1, dst)
        call hicl_knl_set("test_hicl_1", 0, src)
        call hicl_knl_set("test_hicl_1", 1, dst)
        call hicl_knl_set("test_hicl_1", 2, 64)
        call hicl_knl_sync_exec("test_hicl_1", d);
        call hicl_mem_update(dst, READ_ONLY)
        do i=1,N
            status = status .and. (abs(dst(i) - 1.2) < 1.e-4)
        end do
        deallocate(src, dst)
    end function sync_run

    logical function async_run() result(status)
        implicit none
        integer                       :: i
        integer(kind=8), dimension(3) :: gws = (/1_8, 1_8, 1_8/)
        integer(kind=8), dimension(3) :: lws = (/1_8, 1_8, 1_8/)
        real, allocatable :: src(:) 
        real, allocatable :: dst(:) 

        gws(1) = N
        lws(1) = 64

        allocate(src(N), dst(N))
        src = 1.2

        call hicl_dev_find(DEFAULT, d)
        call hicl_mem_wrap(src, d, ior(HWA, READ_ONLY))
        call hicl_mem_wrap(dst, d, ior(HWA, WRITE_ONLY))
        call hicl_mem_update(src, WRITE_ONLY)
        status = .true.
        call hicl_knl_set_wrk("test_hicl_1", 1, gws, lws)
        call hicl_knl_set("test_hicl_1", 0, src)
        call hicl_knl_set("test_hicl_1", 1, dst)
        call hicl_knl_set("test_hicl_1", 2, 64)
        call hicl_knl_exec("test_hicl_1", d);
        call hicl_mem_update(dst, READ_ONLY)
        do i=1,N
            status = status .and. (abs(dst(i) - 1.2) < 1.e-4)
        end do
        deallocate(src, dst)
    end function async_run

    logical function sync_run_hzc() result(status)
        implicit none
        integer                       :: i
        integer(kind=8), dimension(3) :: gws = (/1_8, 1_8, 1_8/)
        integer(kind=8), dimension(3) :: lws = (/1_8, 1_8, 1_8/)
        real,             allocatable :: src(:) 
        real,             allocatable :: dst(:) 
 
        gws(1) = N
        lws(1) = 64

        allocate(src(N), dst(N))
        src = 1.2
       
        call hicl_dev_find(DEFAULT, d)
        call hicl_mem_wrap(src, d, ior(ior(CPU, ZERO_COPY), READ_ONLY))
        call hicl_mem_wrap(dst, d, ior(ior(CPU, ZERO_COPY), WRITE_ONLY))
        call hicl_mem_update(src, WRITE_ONLY)
        status = .true.
        call hicl_knl_set_wrk("test_hicl_1", 1, gws, lws)
        call hicl_knl_set("test_hicl_1", 0, src)
        call hicl_knl_set("test_hicl_1", 1, dst)
        call hicl_knl_set("test_hicl_1", 2, 64)
        call hicl_knl_sync_exec("test_hicl_1", d);
        call hicl_mem_update(dst, READ_ONLY)
        do i=1,N
            status = status .and. (abs(dst(i) - 1.2) < 1.e-4)
        end do
        deallocate(src, dst)
    end function sync_run_hzc

    logical function sync_run_pinned() result(status)
        implicit none
        integer                       :: i
        integer(kind=8), dimension(3) :: gws = (/1_8, 1_8, 1_8/)
        integer(kind=8), dimension(3) :: lws = (/1_8, 1_8, 1_8/)

        real, allocatable :: src(:) 
        real, allocatable :: dst(:)

        allocate(src(N))
        allocate(dst(N))
        src = 1.2

        gws(1) = N
        lws(1) = 64
        
        call hicl_dev_find(DEFAULT, d)
        call hicl_mem_wrap(src, d, ior(ior(HWA, PINNED), READ_ONLY))
        call hicl_mem_wrap(dst, d, ior(ior(HWA, PINNED), WRITE_ONLY))

        call hicl_knl_set_wrk("test_hicl_1", 1, gws, lws)
        call hicl_knl_set("test_hicl_1", 0, src)
        call hicl_knl_set("test_hicl_1", 1, dst)
        call hicl_knl_set("test_hicl_1", 2, 64)
        call hicl_knl_sync_exec("test_hicl_1", d);
        call hicl_mem_update(dst, READ_ONLY)
        status = .true.
        do i=1,N
            status = status .and. (abs(dst(i) - 1.2) < 1.e-4)
        end do
        deallocate(src, dst)
    end function sync_run_pinned

    subroutine init_buffer(buff, nt)
        integer,   intent(in) :: nt 
        real,   intent(inout) :: buff(:)
        real    :: dt, lam, pi, t
        integer :: k
        pi  = 4*atan(1.)
        lam = pi*pi*15.*15.
        dt  = 0.00248
        do k=1, nt
            t       = -0.2+(k-1)*dt
            buff(k) = -2.*lam*(2.*lam*t*t-1)*exp(-lam*t*t)
        enddo
    end subroutine init_buffer

    logical function test_precision_one_thread() result(status)
        implicit none
        real,  allocatable :: src(:) 
        real,  allocatable :: fld(:,:) 
        real,  allocatable :: cpu(:,:) 
        integer(kind=8), dimension(3), parameter :: gws = (/1, 1, 1/)
        integer(kind=8), dimension(3), parameter :: lws = (/1, 1, 1/)
        
        integer            :: i, nt, n

        nt = 4925
        n  = 16

        allocate(src(nt))
        allocate(fld(-3:n+4,-3:n+4))
        allocate(cpu(-3:n+4,-3:n+4))
        
        fld = 0.
        cpu = 0.
        call init_buffer(src, nt)

        call hicl_load("data/add_source_2d.cl", "-cl-kernel-arg-info")
        call hicl_dev_find(DEFAULT, d)
        call hicl_mem_wrap(src, d, ior(HWA, READ_ONLY))
        call hicl_mem_wrap(fld, d, ior(HWA, READ_WRITE))
                
        call hicl_knl_set_wrk("add_source_2d", 1, gws, lws)
        call hicl_knl_set("add_source_2d", 0, 10)
        call hicl_knl_set("add_source_2d", 1, 10)
        call hicl_knl_set("add_source_2d", 2, 11)
        call hicl_knl_set("add_source_2d", 3, 11)
        call hicl_knl_set("add_source_2d", 4, 0.25)
        call hicl_knl_set("add_source_2d", 5, 0.)
        call hicl_knl_set("add_source_2d", 6, n)
        call hicl_knl_set("add_source_2d", 7, 4)
        call hicl_knl_set("add_source_2d", 8, 4)
        call hicl_knl_set("add_source_2d", 9, src)
        call hicl_knl_set("add_source_2d", 10, 0)
        call hicl_knl_set("add_source_2d", 11, fld)

        status = .true.
        do i = 1, nt
            call hicl_knl_set("add_source_2d", 10, i-1)
            call hicl_knl_sync_exec("add_source_2d", d)
            call hicl_mem_update(fld, READ_ONLY)

            cpu(11, 11) = cpu(11, 11) + src(i)*(0.75)
            cpu(11, 12) = cpu(11, 12) + src(i)*0.*(1.0-0.25)
            cpu(12, 11) = cpu(12, 11) + src(i)*(1.0-0.)*0.25
            cpu(12, 12) = cpu(12, 12) + src(i)*0.
            status = (abs(cpu(11,11) - fld(11,11)) < 1.e-4)
            if (.not.status) then
                write(*, '(a, i0, a, e20.7, a, e20.7)') "it: ", &
                    i, " gpu ", fld(11, 11), " cpu ", cpu(11, 11)
                write(*, '(a, i0, a, e20.7, a, e20.7)') "it: ", &
                    i, " gpu ", fld(11, 12), " cpu ", cpu(11, 12)
                write(*, '(a, i0, a, e20.7, a, e20.7)') "it: ", &
                    i, " gpu ", fld(12, 11), " cpu ", cpu(12, 11)
                write(*, '(a, i0, a, e20.7, a, e20.7)') "it: ", &
                    i, " gpu ", fld(12, 12), " cpu ", cpu(12, 12)
                write(*, *)
            endif
        enddo
        deallocate(cpu, fld, src)
    end function test_precision_one_thread



    subroutine setup()
        call hicl_init(ALL)
        call hicl_load("data/foo.cl", "-cl-kernel-arg-info -D STENCIL=4")
        call hicl_load("data/bar.cl", "-cl-kernel-arg-info -D STENCIL=4")
        call hicl_dev_find(DEFAULT, d)
    end subroutine setup

    subroutine teardown()
        call hicl_release()
    end subroutine teardown

    subroutine knl_test()
        call run(setup, teardown, info, &
              "knl_test.info")
        call run(setup, teardown, build, &
              "knl_test.build")
        call run(setup, teardown, set_wrk, &
              "knl_test.set_wrk")
        call run(setup, teardown, set_arg, &
              "knl_test.set_arg")
        call run(setup, teardown, sync_run, &
            "knl_test.run")
        call run(setup, teardown, async_run, &
            "knl_test.async_run")
        call run(setup, teardown, sync_run_hzc, &
            "knl_test.sync_run_hzc")
        !call run(setup, teardown, sync_run_dzc, &
        !    "knl_test.sync_run_dzc")
        call run(setup, teardown, sync_run_pinned, &
            "knl_test.sync_run_pinned")
        call run(setup, teardown, test_precision_one_thread, &
            "knl_test.test_precision_one_thread")
    end subroutine knl_test

end module m_knl_test