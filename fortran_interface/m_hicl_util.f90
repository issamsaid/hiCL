module m_hicl_util
    use, intrinsic :: iso_c_binding
    use m_hicl_flags
    use m_hicl_types

    implicit none

    private

    interface
        logical(kind=c_bool) &
        function hicl_has(flags) bind(c, name="hicl_has")
            use, intrinsic :: iso_c_binding, only: c_bool, c_int64_t
            implicit none
            integer(kind=c_int64_t), intent(in), value :: flags 
        end function hicl_has

        integer(kind=c_int) &
        function hicl_count(flags) bind(c, name="hicl_count")
            use, intrinsic :: iso_c_binding, only: c_int64_t, c_int
            implicit none
            integer(kind=c_int64_t), intent(in), value :: flags 
        end function hicl_count

        integer(kind=c_int) &
        function hicl_knl_count() bind(c, name="hicl_knl_count")
            use, intrinsic :: iso_c_binding, only: c_int
            implicit none
        end function

        integer(kind=c_int) &
        function hicl_mem_count() bind(c, name="hicl_mem_count")
            use, intrinsic :: iso_c_binding, only: c_int
            implicit none
        end function        
    end interface

    public :: hicl_has
    public :: hicl_count
    public :: hicl_knl_count
    public :: hicl_mem_count
 
end module m_hicl_util
