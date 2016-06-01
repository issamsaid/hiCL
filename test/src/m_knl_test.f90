!>
!! @copyright Copyright 2013 - 2014 Total S.A. All rights reserved.
!!
!! NOTICE: All information contained herein is, and remains
!! the property of Total S.A. and its suppliers, if any.
!! The intellectual and technical concepts contained herein
!! are proprietary to Total S.A. and its suppliers and may be
!! covered by U.S. and Foreign Patents, patents in process,
!! and are protected by trade secret or copyright law.
!! Dissemination of this information or reproduction of this
!! material is strictly forbidden unless prior written permission
!! is obtained from Total S.A.
!! 
!! @author Issam Said
!! @file m_knl_test.f90
!! @version $Id$
!! @brief Unit test for hicl_knl.
!<
module m_knl_test
    use iso_c_binding
    use m_hicl_types
    use m_hicl_base
    use m_hicl_mem
    use m_hicl_dev
    use m_hicl_knl
    use m_hicl_tsk
    use m_handler
    
    implicit none

    private

    type(knl),         pointer :: k
    type(dev),         pointer :: d
    integer(kind=4), parameter :: N = 64

    public :: knl_test

contains

    subroutine setup()
        call hicl_load("data/foo.cl", "-D STENCIL=4")
        call hicl_load("data/bar.cl", "-D STENCIL=4")
        call hicl_knl_find("test_hicl_1", k)
        call hicl_dev_find(DEFAULT, d)
    end subroutine setup

    subroutine teardown()
    end subroutine teardown

    subroutine knl_test()
        call run(setup, teardown, info, &
              "knl_test.info")
        call run(setup, teardown, find, &
              "knl_test.find")
        call run(setup, teardown, build_by_name, &
              "knl_test.build_by_name")
        call run(setup, teardown, build, &
              "knl_test.build")
        call run(setup, teardown, set_wrk_by_name, &
              "knl_test.set_wrk_by_name")
        call run(setup, teardown, set_wrk, &
              "knl_test.set_wrk")
        call run(setup, teardown, set_arg_by_name, &
              "knl_test.set_arg_by_name")
        call run(setup, teardown, set_arg, &
              "knl_test.set_arg")
        call run(setup, teardown, sync_run_by_name, &
            "knl_test.sync_run_by_name")
        call run(setup, teardown, sync_run, &
            "knl_test.run")
        call run(setup, teardown, async_run_by_name, &
            "knl_test.async_run_by_name")
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

    logical function info() result(status)
        implicit none
        call hicl_knl_info(k)
        status = associated(k)
    end function info

    logical function find() result(status)
        implicit none
        type(knl), pointer :: k2
        call hicl_knl_find("test_hicl_2", k2)
        status = associated(k2)
    end function find

    logical function build_by_name() result(status)
        implicit none
        call hicl_build("test_hicl_1", "-DSTENCIL=29");
        status = (hicl_count(KERNEL) == 6)
    end function build_by_name

    logical function build() result(status)
        implicit none
        call hicl_knl_build(k, "-DSTENCIL=29");
        status = (hicl_count(KERNEL) == 6)
    end function build

    logical function set_wrk_by_name() result(status)
        implicit none
        integer(kind=8), dimension(3), parameter :: gws = (/256, 384, 128/)
        integer(kind=8), dimension(3), parameter :: lws = (/64, 64, 1/)
        call hicl_set_wrk("test_hicl_1", 3, gws, lws)
        status = ((k%gws(1) == 256) .and. &
                  (k%gws(2) == 384) .and. &
                  (k%gws(3) == 128) .and. &
                  (k%lws(1) == 64)  .and. &
                  (k%lws(2) == 64)  .and. &
                  (k%lws(3) == 1))
    end function set_wrk_by_name

    logical function set_wrk() result(status)
        implicit none
        integer(kind=8), dimension(3), parameter :: gws = (/256, 384, 128/)
        integer(kind=8), dimension(3), parameter :: lws = (/64, 64, 1/)
        call hicl_knl_set_wrk(k, 3, gws, lws)
        status = ((k%gws(1) == 256) .and. &
                  (k%gws(2) == 384) .and. &
                  (k%gws(3) == 128) .and. &
                  (k%lws(1) == 64)  .and. &
                  (k%lws(2) == 64)  .and. &
                  (k%lws(3) == 1))
    end function set_wrk

    logical function set_arg_by_name() result(status)
        implicit none
        integer, parameter :: i = 9
        call hicl_set("test_hicl_3", 2, i)
        status = .true.
    end function set_arg_by_name

    logical function set_arg() result(status)
        implicit none
        integer, parameter :: i = 9
        call hicl_knl_set(k, 2, i)
        status = .true.
    end function set_arg

    logical function sync_run_by_name() result(status)
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
        call hicl_set_wrk("test_hicl_1", 1, gws, lws)
        call hicl_set("test_hicl_1", 0, src)
        call hicl_set("test_hicl_1", 1, dst)
        call hicl_set("test_hicl_1", 0, src)
        call hicl_set("test_hicl_1", 1, dst)
        call hicl_set("test_hicl_1", 2, 64)
        call hicl_srun("test_hicl_1", d);
        call hicl_mem_update(dst, READ_ONLY)
        do i=1,N
            status = status .and. (abs(dst(i) - 1.2) < 1.e-4)
        end do
        deallocate(src, dst)
    end function sync_run_by_name

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
        call hicl_knl_set_wrk(k, 1, gws, lws)
        call hicl_knl_set(k, 0, src)
        call hicl_knl_set(k, 1, dst)
        call hicl_knl_set(k, 2, 64)
        call hicl_knl_srun(k, d);
        call hicl_mem_update(dst, READ_ONLY)
        do i=1,N
            status = status .and. (abs(dst(i) - 1.2) < 1.e-4)
        end do
        deallocate(src, dst)
    end function sync_run

    logical function async_run_by_name() result(status)
        implicit none
        integer                       :: i
        integer(kind=8), dimension(3) :: gws = (/1_8, 1_8, 1_8/)
        integer(kind=8), dimension(3) :: lws = (/1_8, 1_8, 1_8/)
        real, allocatable, target :: src(:) 
        real, allocatable, target :: dst(:) 

        gws(1) = N
        lws(1) = 64

        allocate(src(N), dst(N))
        src = 1.2

        call hicl_dev_find(DEFAULT, d)
        call hicl_mem_wrap(src, d, ior(HWA, READ_ONLY))
        call hicl_mem_wrap(dst, d, ior(HWA, WRITE_ONLY))
        call hicl_mem_update(src, WRITE_ONLY)
        status = .true.
        call hicl_set_wrk("test_hicl_1", 1, gws, lws)
        call hicl_set("test_hicl_1", 0, src)
        call hicl_set("test_hicl_1", 1, dst)
        call hicl_set("test_hicl_1", 2, 64)
        call hicl_run("test_hicl_1", d);
        call hicl_mem_update(dst, READ_ONLY)
        do i=1,N
            status = status .and. (abs(dst(i) - 1.2) < 1.e-4)
        end do
        deallocate(src, dst)
    end function async_run_by_name

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
        call hicl_knl_set_wrk(k, 1, gws, lws)
        call hicl_knl_set(k, 0, src)
        call hicl_knl_set(k, 1, dst)
        call hicl_knl_set(k, 2, N)
        call hicl_knl_run(k, d);
        call hicl_mem_update(dst, READ_ONLY)
        status = (hicl_count(MEMORY) .eq. 2)
        do i=1,N
            status = status .and. (dst(i) .eq. 1.2)
        enddo
        deallocate(src, dst)
    end function async_run

    logical function sync_run_hzc() result(status)
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
        call hicl_mem_wrap(src, d, ior(ior(CPU, ZERO_COPY), READ_ONLY))
        call hicl_mem_wrap(dst, d, ior(ior(CPU, ZERO_COPY), WRITE_ONLY))
        call hicl_mem_update(src, WRITE_ONLY)
        status = .true.
        call hicl_knl_set_wrk(k, 1, gws, lws)
        call hicl_knl_set(k, 0, src)
        call hicl_knl_set(k, 1, dst)
        call hicl_knl_set(k, 2, 64)
        call hicl_knl_srun(k, d);
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

        call hicl_knl_set_wrk(k, 1, gws, lws)
        call hicl_knl_set(k, 0, src)
        call hicl_knl_set(k, 1, dst)
        call hicl_knl_set(k, 2, 64)
        call hicl_knl_srun(k, d);
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

        type(knl), pointer :: ksrc
        integer            :: i, nt, n

        nt = 4925
        n  = 16

        allocate(src(nt))
        allocate(fld(-3:n+4,-3:n+4))
        allocate(cpu(-3:n+4,-3:n+4))
        
        fld = 0.
        cpu = 0.
        call init_buffer(src, nt)

        call hicl_load("data/add_source_2d.cl")
        call hicl_dev_find(DEFAULT, d)
        call hicl_mem_wrap(src, d, ior(HWA, READ_ONLY))
        call hicl_mem_wrap(fld, d, ior(HWA, READ_WRITE))
                
        call hicl_knl_find("add_source_2d", ksrc)
        call hicl_knl_set(ksrc, 0, 10)
        call hicl_knl_set(ksrc, 1, 10)
        call hicl_knl_set(ksrc, 2, 11)
        call hicl_knl_set(ksrc, 3, 11)
        call hicl_knl_set(ksrc, 4, 0.25)
        call hicl_knl_set(ksrc, 5, 0.)
        call hicl_knl_set(ksrc, 6, n)
        call hicl_knl_set(ksrc, 7, 4)
        call hicl_knl_set(ksrc, 8, 4)
        call hicl_knl_set(ksrc, 9, src)
        call hicl_knl_set(ksrc, 10, 0)
        call hicl_knl_set(ksrc, 11, fld)

        status = .true.
        do i = 1, nt
            call hicl_knl_set(ksrc, 10, i-1)
            call hicl_tsk_srun(ksrc, d)
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

end module m_knl_test
