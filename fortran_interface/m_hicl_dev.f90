module m_hicl_dev
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    implicit none
    
    private

    interface
        type(c_ptr) &
        function c_hicl_dev_init(dev_id) bind(c, name="hicl_dev_init")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: dev_id
        end function c_hicl_dev_init

        subroutine c_hicl_dev_release(c_dev_ptr) bind(c, name="hicl_dev_release")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_dev_ptr            
        end subroutine c_hicl_dev_release

        type(c_ptr) &
        function c_hicl_dev_find(flags) bind(c, name="hicl_dev_find")
            use iso_c_binding, only: c_ptr, c_int64_t
            integer(kind=c_int64_t), value, intent(in) :: flags
        end function c_hicl_dev_find

        subroutine c_hicl_dev_info(c_dev_ptr) bind(c, name="hicl_dev_info")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_dev_ptr
        end subroutine c_hicl_dev_info

        subroutine c_hicl_dev_wait(c_dev_ptr) bind(c, name = "hicl_dev_wait")
            use iso_c_binding, only: c_ptr
            type(c_ptr), value, intent(in) :: c_dev_ptr
        end subroutine c_hicl_dev_wait

        logical(kind=c_bool) &
        function c_hicl_dev_support(c_dev_ptr, extension) & 
            bind(c, name="hicl_dev_support")
            use iso_c_binding, only: c_bool, c_char, c_ptr
            type(c_ptr),     value, intent(in) :: c_dev_ptr
            character(kind=c_char), intent(in) :: extension
        end function c_hicl_dev_support
    end interface

    public :: hicl_dev_init
    public :: hicl_dev_release
    public :: hicl_dev_find
    public :: hicl_dev_info
    public :: hicl_dev_wait
    public :: hicl_dev_support

contains

    subroutine hicl_dev_init(d, dev_id)
        type(c_ptr),       value, intent(in) :: dev_id
        type(dev), pointer, intent(inout) :: d
        call c_f_pointer(c_hicl_dev_init(dev_id), d)
    end subroutine hicl_dev_init

    subroutine hicl_dev_release(d) 
        type(dev), pointer, intent(inout) :: d
        call c_hicl_dev_release(c_loc(d))
    end subroutine hicl_dev_release

    subroutine hicl_dev_find(flags, d)
        integer(kind=c_int64_t), value, intent(in) :: flags
        type(dev),       pointer, intent(inout) :: d
        call c_f_pointer(c_hicl_dev_find(flags), d)
    end subroutine hicl_dev_find

    subroutine hicl_dev_info(d)
        type(dev), pointer, intent(in) :: d
        call c_hicl_dev_info(c_loc(d))
    end subroutine hicl_dev_info
    
    subroutine hicl_dev_wait(d)
        type(dev), pointer, intent(in) :: d
        call c_hicl_dev_wait(c_loc(d))
    end subroutine hicl_dev_wait

    logical(kind=c_bool) &
    function hicl_dev_support (d, extension) result(status)
        type(dev), pointer, intent(in) :: d
        character(len=*),      intent(in) :: extension
        status = c_hicl_dev_support(c_loc(d), &
                                    extension // c_null_char)
    end function hicl_dev_support

end module m_hicl_dev
