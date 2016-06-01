module m_mem_test
    use, intrinsic :: iso_c_binding
    use            :: m_hicl_types
    use            :: m_hicl_base
    use            :: m_hicl_dev
    use            :: m_hicl_mem
    use            :: m_handler
    
    implicit none

    private

    public :: mem_test

    integer, parameter :: N = 64
    type(dev), pointer :: d

contains

    subroutine setup()
        call hicl_dev_find(DEFAULT, d)
    end subroutine setup

    subroutine teardown()
    end subroutine teardown
    
    subroutine mem_test()
        call run(setup, teardown, create_1d, &
              "mem_test.create_1d")
        call run(setup, teardown, create_2d, &
              "mem_test.create_2d")
        call run(setup, teardown, create_3d, &
              "mem_test.create_3d")
        call run(setup, teardown, float_default_buffer, &
              "mem_test.float_default_buffer")
        call run(setup, teardown, int_pinned_buffer, &
              "mem_test.int_pinned_buffer")
        call run(setup, teardown, double_hzc_buffer, &
              "mem_test.double_hzc_buffer")
        call run(setup, teardown, long_dzc_buffer, &
              "mem_test.long_dsz_buffer")
        call run(setup, teardown, float_allocated_buffer, &
              "mem_test.float_allocated_buffer")
        call run(setup, teardown, float_read_only_buffer, &
              "mem_test.float_read_only_buffer")
        call run(setup, teardown, float_write_only_buffer, &
              "mem_test.float_write_only_buffer")
        call run(setup, teardown, info, &
              "mem_test.info")
        call run(setup, teardown, read_access_buffer, &
               "mem_test.read_access_buffer")
        call run(setup, teardown, write_access_buffer, &
              "mem_test.write_access_buffer")
        call run(setup, teardown, access_buffer_2d, &
              "mem_test.access_buffer_2d")
        call run(setup, teardown, access_buffer_3d, &
              "mem_test.access_buffer_3d")
    end subroutine mem_test
    
    logical function create_1d() result(status)
        real(kind=4), dimension(:), contiguous, pointer :: h
        integer(kind=4)                                 :: low(1)  =  (/0/)
        integer(kind=4)                                 :: high(1) =  (/N-1/)
        call hicl_mem_allocate(h, d, ior(HWA, FLOAT), low, high)
        h(0:N-1) = 1.
        status = (hicl_count(MEMORY) == 1).and.(rank(h).eq.1)
    end function create_1d

    logical function create_2d() result(status)
        integer(kind=4), dimension(:,:), contiguous, pointer :: h
        integer(kind=4)                                      :: low(2)  =  (/1,1/)
        integer(kind=4)                                      :: high(2) =  (/N,N/)
        call hicl_mem_allocate(h, d, ior(HWA ,INT), low, high)
        h(1:N,1:N) = 1
        status = (hicl_count(MEMORY) == 1).and.(rank(h).eq.2)
    end function create_2d

    logical function create_3d() result(status)
        real(kind=8), dimension(:,:,:), contiguous, pointer :: h
        integer(kind=4)                                     :: low(3)  =  (/1,1,1/)
        integer(kind=4)                                     :: high(3) =  (/N,N,N/)
        call hicl_mem_allocate(h, d, ior(HWA, DOUBLE), low, high)
        h(1:N,1:N,1:N) = 1.
        status = (hicl_count(MEMORY) == 1).and.(rank(h) == 3)
    end function create_3d

    logical function float_default_buffer() result(status)
        real, pointer, contiguous :: h(:)
        integer(kind=4)                                 :: low(1)  =  (/0/)
        integer(kind=4)                                 :: high(1) =  (/N-1/)
        call hicl_mem_allocate(h, d, HWA, low, high)
        status = associated(d) .and. (hicl_count(MEMORY) == 1)
    end function float_default_buffer

    logical function int_pinned_buffer() result(status)
        integer(kind=4), dimension(:), contiguous, pointer :: h 
        integer(kind=4)                                 :: low(1)  =  (/0/)
        integer(kind=4)                                 :: high(1) =  (/N-1/)
        call hicl_mem_allocate(h, d, ior(ior(HWA, PINNED), INT), low, high)
        status = associated(d) .and. (hicl_count(MEMORY) == 1)
    end function int_pinned_buffer

    logical function double_hzc_buffer() result(status)
        real(kind=8), dimension(:), contiguous, pointer :: h 
        integer(kind=4)                                 :: low(1)  =  (/0/)
        integer(kind=4)                                 :: high(1) =  (/N-1/)
        call hicl_mem_allocate(h, d, ior(ior(CPU, ZERO_COPY), DOUBLE), low, high)
        status = associated(d) .and. (hicl_count(MEMORY) == 1)
    end function double_hzc_buffer

    logical function long_dzc_buffer() result(status)
        integer(kind=c_long), dimension(:), contiguous, pointer :: h 
        integer(kind=4)                                 :: low(1)  =  (/0/)
        integer(kind=4)                                 :: high(1) =  (/N-1/)
        call hicl_mem_allocate(h, d, ior(ior(HWA, ZERO_COPY), LONG), low, high)
        status = associated(d) .and. (hicl_count(MEMORY) == 1)
    end function long_dzc_buffer

    logical function float_allocated_buffer() result(status)
        real, allocatable :: h(:)
        allocate(h(N)) 
        call hicl_mem_wrap(h, d, FLOAT)
        status = associated(d) .and. (hicl_count(MEMORY) == 1)
        deallocate(h)
    end function float_allocated_buffer

    logical function float_read_only_buffer() result(status)
        real(kind=4), dimension(:), contiguous, pointer :: h
        integer(kind=4)                                 :: low(1)  =  (/0/)
        integer(kind=4)                                 :: high(1) =  (/N-1/)
        call hicl_mem_allocate(h, d, ior(READ_ONLY, FLOAT), low, high)
        status = associated(d) .and. (hicl_count(MEMORY) == 1)
    end function float_read_only_buffer

    logical function float_write_only_buffer() result(status)
        real(kind=4), dimension(:), contiguous, pointer :: h 
        integer(kind=4)                                 :: low(1)  =  (/0/)
        integer(kind=4)                                 :: high(1) =  (/N-1/)
        call hicl_mem_allocate(h, d, ior(WRITE_ONLY, FLOAT), low, high)
        status = associated(d) .and. (hicl_count(MEMORY) == 1)
    end function float_write_only_buffer

    logical function info() result(status)
        real(kind=4), allocatable :: h(:)
        allocate(h(N))
        call hicl_mem_wrap(h, d, HWA)
        status = associated(d)
        call hicl_mem_info(h)
        deallocate(h)
    end function info

    logical function read_access_buffer() result(status)
        real, allocatable :: h(:)
        allocate(h(N))
        call hicl_mem_wrap(h, d, READ_WRITE)
        call hicl_mem_update(h, READ_ONLY)
        status = associated(d)
        status = status .and. (hicl_count(MEMORY) == 1)
        deallocate(h)
    end function read_access_buffer

    logical function write_access_buffer() result(status)    
        real, allocatable :: h(:)
        allocate(h(-3:N-4))
        h(-3:N-4) = 0.0
        h(-3)     = 1.2
        call hicl_mem_wrap(h, d, READ_WRITE)        
        call hicl_mem_update(h, READ_ONLY)
        status = h(-3).eq.1.2
        status = status .and. (hicl_count(MEMORY) == 1)
        deallocate(h)
    end function write_access_buffer

    logical function access_buffer_2d() result(status)
        real, allocatable :: h2d(:,:)
        integer :: i, j, H, W
        H = 16
        W = 4
        allocate(h2d(H,W))
        call hicl_mem_wrap(h2d, d, READ_WRITE)
        call hicl_mem_update(h2d, WRITE_ONLY)
        do j=1,W
            do i=1,H
                h2d(i,j) = (j-1)*H+i
            enddo
        enddo
        call hicl_mem_update(h2d, READ_ONLY)
        do j=1,W
            do i=1,H
                status = status .and. (abs(h2d(i,j) - ((j-1)*H+i)) < 1.e-4)
            enddo
        enddo
        status = (hicl_count(MEMORY) == 1)
        call hicl_mem_release(h2d)
        status = status .and. (hicl_count(MEMORY) == 0)
        deallocate(h2d)
    end function access_buffer_2d

    logical function access_buffer_3d() result(status)
        real, allocatable :: h3d(:,:,:)
        integer :: TH, TW, TD
        TH = 16
        TW = 16
        TD = 16
        
        allocate(h3d(-3:TH+4, -3:TW+4, -3:TD+4))
        h3d = 0.
        h3d(1:TH, 1:TW, 1:TD) = 1.2
        call hicl_mem_wrap(h3d, d, READ_WRITE)
        call hicl_mem_update(h3d, READ_ONLY)
        
        status = (hicl_count(MEMORY) == 1)
        call hicl_mem_release(h3d)
        status = status .and. (hicl_count(MEMORY) == 0)
        deallocate(h3d)
    end function access_buffer_3d

end module m_mem_test
