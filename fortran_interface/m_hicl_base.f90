module m_hicl_base
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    implicit none

    private

    interface
        subroutine c_hicl_init(flags) bind(c, name = "hicl_init")
            use, intrinsic :: iso_c_binding, only: c_int64_t
            implicit none
            integer(kind=c_int64_t), intent(in), value :: flags
        end subroutine c_hicl_init

        subroutine c_hicl_load(filename, options) bind(c, name = "hicl_load")
            use, intrinsic :: iso_c_binding, only: c_char
            implicit none
            character(kind=c_char), intent(in) :: filename
            character(kind=c_char), intent(in) :: options
        end subroutine c_hicl_load
    end interface

    interface
        subroutine hicl_release() bind(c, name = "hicl_release")
        end subroutine hicl_release

        subroutine hicl_info() bind(c, name = "hicl_info")
        end subroutine hicl_info 
    end interface

    public :: hicl_init
    public :: hicl_load
    public :: hicl_release
    public :: hicl_info

contains
    
    subroutine hicl_init(flags)
        integer(kind=c_int64_t), optional, intent(in) :: flags
        if (present(flags)) then
            call c_hicl_init(flags)
        else
            call c_hicl_init(DEFAULT)
        end if
    end subroutine hicl_init
    
    subroutine hicl_load(filename, options)
        character(len=*),           intent(in) :: filename
        character(len=*), optional, intent(in) :: options
        if (present(options)) then
            call c_hicl_load(filename // c_null_char, options // c_null_char)
        else
            call c_hicl_load(filename // c_null_char, c_null_char)
        end if
    end subroutine hicl_load
    
end module m_hicl_base