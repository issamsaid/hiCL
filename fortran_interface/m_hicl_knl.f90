module m_hicl_knl
    use, intrinsic :: iso_c_binding
    use m_hicl_types

    implicit none

    private

    interface        
        subroutine c_hicl_knl_info(name) &
            bind(c, name="hicl_knl_info")
              use, intrinsic :: iso_c_binding, only: c_char
            character(kind=c_char), intent(in) :: name
        end subroutine c_hicl_knl_info

        type(c_ptr) &
        function c_hicl_knl_find(name) &
            bind(c, name="hicl_knl_find")
            use, intrinsic :: iso_c_binding, only: c_ptr, c_char
            character(kind=c_char), intent(in) :: name
        end function c_hicl_knl_find

        subroutine c_hicl_knl_build(name, options) &
            bind(c, name="hicl_knl_build")
            use, intrinsic :: iso_c_binding, only: c_char, c_char
            character(kind=c_char), intent(in) :: name
            character(kind=c_char), intent(in) :: options
        end subroutine c_hicl_knl_build

        subroutine c_hicl_knl_set_int32(name, index, arg) &
            bind(c, name="hicl_knl_set_int32")
            use, intrinsic :: iso_c_binding, only: c_char, c_int, c_int32_t
            character(kind=c_char), intent(in)         :: name
            integer(kind=c_int), value, intent(in)     :: index
            integer(kind=c_int32_t), value, intent(in) :: arg
        end subroutine c_hicl_knl_set_int32

        subroutine c_hicl_knl_set_int64(name, index, arg) &
            bind(c, name="hicl_knl_set_int64")
            use, intrinsic :: iso_c_binding, only: c_int, c_int64_t, c_char
            character(kind=c_char), intent(in)         :: name
            integer(kind=c_int),     value, intent(in) :: index
            integer(kind=c_int64_t), value, intent(in) :: arg
        end subroutine c_hicl_knl_set_int64

        subroutine c_hicl_knl_set_float(name, index, arg) &
            bind(c, name="hicl_knl_set_float")
            use, intrinsic :: iso_c_binding, only: c_float, c_int, c_char
            character(kind=c_char), intent(in)     :: name
            integer(kind=c_int), value, intent(in) :: index
            real(kind=c_float),  value, intent(in) :: arg
        end subroutine c_hicl_knl_set_float

        subroutine c_hicl_knl_set_double(name, index, arg) &
            bind(c, name="hicl_knl_set_double")
            use, intrinsic :: iso_c_binding, only: c_double, c_int, c_char
            character(kind=c_char), intent(in)     :: name
            integer(kind=c_int), value, intent(in) :: index
            real(kind=c_double), value, intent(in) :: arg
        end subroutine c_hicl_knl_set_double

        subroutine c_hicl_knl_set_mem(name, index, arg) &
            bind(c, name="hicl_knl_set_mem")
            use, intrinsic :: iso_c_binding, only: c_int, c_char, c_ptr
            character(kind=c_char), intent(in)     :: name
            integer(kind=c_int), value, intent(in) :: index
            type(c_ptr),         value, intent(in) :: arg
        end subroutine c_hicl_knl_set_mem

        subroutine c_hicl_knl_set_wrk(name, wrk, gws, lws) &
            bind(c, name="hicl_knl_set_wrk")
            use, intrinsic :: iso_c_binding, only: c_char, c_int, c_size_t
            character(kind=c_char), intent(in)               :: name
            integer(kind=c_int),           value, intent(in) :: wrk
            integer(kind=c_size_t), dimension(3), intent(in) :: gws
            integer(kind=c_size_t), dimension(3), intent(in) :: lws
        end subroutine c_hicl_knl_set_wrk

        subroutine c_hicl_knl_set_ofs(name, ofs) &
            bind(c, name="hicl_knl_set_ofs")
            use, intrinsic :: iso_c_binding, only: c_char, c_int, c_size_t
            character(kind=c_char), intent(in)               :: name
            integer(kind=c_size_t), dimension(3), intent(in) :: ofs
        end subroutine c_hicl_knl_set_ofs
        
        real(kind=c_double) &
        function c_hicl_knl_timed_exec(name, dev) &
            bind(c, name="hicl_knl_timed_exec")
            use, intrinsic :: iso_c_binding, only: c_char, c_double, c_ptr
            character(kind=c_char), intent(in) :: name
            type(c_ptr),     value, intent(in) :: dev
        end function c_hicl_knl_timed_exec

        subroutine c_hicl_knl_sync_exec(name, dev) &
            bind(c, name="hicl_knl_sync_exec")
            use, intrinsic :: iso_c_binding, only: c_char, c_ptr
            character(kind=c_char), intent(in) :: name
            type(c_ptr),     value, intent(in) :: dev
        end subroutine c_hicl_knl_sync_exec

        subroutine c_hicl_knl_exec(name, dev) &
            bind(c, name="hicl_knl_exec")
            use, intrinsic :: iso_c_binding, only: c_ptr, c_char
            character(kind=c_char), intent(in) :: name
            type(c_ptr),     value, intent(in) :: dev
        end subroutine c_hicl_knl_exec
    end interface
    
    public :: hicl_knl_info
    public :: hicl_knl_build    
    public :: hicl_knl_set_wrk

    public :: hicl_knl_set
    interface hicl_knl_set
        module procedure hicl_knl_set_int32
        module procedure hicl_knl_set_int64
        module procedure hicl_knl_set_float
        module procedure hicl_knl_set_double
        !! hicl_knl_set_mem
        module procedure hicl_knl_set_mem_int32_1d
        module procedure hicl_knl_set_mem_int64_1d
        module procedure hicl_knl_set_mem_float_1d
        module procedure hicl_knl_set_mem_double_1d
        module procedure hicl_knl_set_mem_int32_2d
        module procedure hicl_knl_set_mem_int64_2d
        module procedure hicl_knl_set_mem_float_2d
        module procedure hicl_knl_set_mem_double_2d
        module procedure hicl_knl_set_mem_int32_3d
        module procedure hicl_knl_set_mem_int64_3d
        module procedure hicl_knl_set_mem_float_3d
        module procedure hicl_knl_set_mem_double_3d
    end interface hicl_knl_set
    
    public :: hicl_knl_exec
    public :: hicl_knl_sync_exec
    public :: hicl_knl_timed_exec
    
contains

    subroutine hicl_knl_info(name)
        character(len=*), intent(in) :: name
        call c_hicl_knl_info(name // c_null_char)
    end subroutine hicl_knl_info
    
    subroutine hicl_knl_build(name, options)
        character(len=*),           intent(in) :: name
        character(len=*), optional, intent(in) :: options
        if (present(options)) then
            call c_hicl_knl_build(name // c_null_char, options // c_null_char)
        else
            call c_hicl_knl_build(name // c_null_char, c_null_char)
        end if
    end subroutine hicl_knl_build

    subroutine hicl_knl_set_wrk(name, wrk, gws, lws, ofs)
        character(len=*),                               intent(in) :: name
        integer(kind=c_int),                     value, intent(in) :: wrk
        integer(kind=c_size_t),           dimension(3), intent(in) :: gws
        integer(kind=c_size_t),           dimension(3), intent(in) :: lws
        integer(kind=c_size_t), optional, dimension(3), intent(in) :: ofs
        call c_hicl_knl_set_wrk(name // c_null_char, wrk, gws, lws)
        if (present(ofs)) then
           call c_hicl_knl_set_ofs(name // c_null_char, ofs)
        endif
    end subroutine hicl_knl_set_wrk

    subroutine hicl_knl_set_int32(name, index, arg)
        character(len=*), intent(in) :: name
        integer(kind=4),  intent(in) :: index
        integer(kind=4),  intent(in) :: arg
        call c_hicl_knl_set_int32(name // c_null_char, index, arg)
    end subroutine hicl_knl_set_int32

    subroutine hicl_knl_set_int64(name, index, arg)
        character(len=*),      intent(in) :: name
        integer(kind=4),       intent(in) :: index
        integer(kind=8),       intent(in) :: arg
        call c_hicl_knl_set_int64(name // c_null_char, index, arg)
    end subroutine hicl_knl_set_int64

    subroutine hicl_knl_set_float(name, index, arg)
        character(len=*),      intent(in) :: name
        integer(kind=4),       intent(in) :: index
        real(kind=4),          intent(in) :: arg
        call c_hicl_knl_set_float(name // c_null_char, index, arg)
    end subroutine hicl_knl_set_float
    
    subroutine hicl_knl_set_double(name, index, arg)
        character(len=*),      intent(in) :: name
        integer(kind=4),       intent(in) :: index
        real(kind=8),          intent(in) :: arg
        call c_hicl_knl_set_double(name // c_null_char, index, arg)
    end subroutine hicl_knl_set_double

    !!
    !! hicl_knl_set_mem
    !! 
    subroutine hicl_knl_set_mem_int32_1d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        integer(kind=4), allocatable, target, intent(in) :: arg(:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_int32_1d

    subroutine hicl_knl_set_mem_int64_1d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        integer(kind=8), allocatable, target, intent(in) :: arg(:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_int64_1d

    subroutine hicl_knl_set_mem_float_1d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        real(kind=4),    allocatable, target, intent(in) :: arg(:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_float_1d

    subroutine hicl_knl_set_mem_double_1d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        real(kind=8),    allocatable, target, intent(in) :: arg(:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_double_1d

    subroutine hicl_knl_set_mem_int32_2d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        integer(kind=4), allocatable, target, intent(in) :: arg(:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_int32_2d

    subroutine hicl_knl_set_mem_int64_2d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        integer(kind=8), allocatable, target, intent(in) :: arg(:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_int64_2d

    subroutine hicl_knl_set_mem_float_2d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        real(kind=4),    allocatable, target, intent(in) :: arg(:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_float_2d

    subroutine hicl_knl_set_mem_double_2d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        real(kind=8),    allocatable, target, intent(in) :: arg(:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_double_2d

    subroutine hicl_knl_set_mem_int32_3d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        integer(kind=4), allocatable, target, intent(in) :: arg(:,:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_int32_3d

    subroutine hicl_knl_set_mem_int64_3d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        integer(kind=8), allocatable, target, intent(in) :: arg(:,:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_int64_3d

    subroutine hicl_knl_set_mem_float_3d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        real(kind=4),    allocatable, target, intent(in) :: arg(:,:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_float_3d

    subroutine hicl_knl_set_mem_double_3d(name, index, arg)
        character(len=*),                     intent(in) :: name
        integer(kind=4),                      intent(in) :: index
        real(kind=8),    allocatable, target, intent(in) :: arg(:,:,:)
        call c_hicl_knl_set_mem(name // c_null_char, index, c_loc(arg))
    end subroutine hicl_knl_set_mem_double_3d

    !!
    !! exec
    !!
    subroutine hicl_knl_timed_exec(name, d, elapsed)
        character(len=*),   intent(in) :: name
        type(dev), pointer, intent(in) :: d
        real(kind=8),      intent(out) :: elapsed
        elapsed = c_hicl_knl_timed_exec(name // c_null_char, c_loc(d))
    end subroutine hicl_knl_timed_exec

    subroutine hicl_knl_sync_exec(name, d)
        character(len=*),   intent(in) :: name
        type(dev), pointer, intent(in) :: d
        call c_hicl_knl_sync_exec(name // c_null_char, c_loc(d))
    end subroutine hicl_knl_sync_exec

    subroutine hicl_knl_exec(name, d)
        character(len=*),   intent(in) :: name
        type(dev), pointer, intent(in) :: d
        call c_hicl_knl_exec(name // c_null_char, c_loc(d))
    end subroutine hicl_knl_exec

end module m_hicl_knl