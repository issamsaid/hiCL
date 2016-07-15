module m_hicl_timer
    use, intrinsic :: iso_c_binding
    use m_hicl_types
    
    implicit none
  
    private

    interface
        real(c_double) &
        function c_hicl_timer_read() bind(c, name = "hicl_timer_read")
            use, intrinsic :: iso_c_binding, only: c_double
        end function c_hicl_timer_read
    end interface

    public :: hicl_timer_uset
    public :: hicl_timer_tick
    
    interface
        subroutine hicl_timer_uset(unit) bind(c, name = "hicl_timer_uset")
            use, intrinsic :: iso_c_binding, only: c_int
            integer(kind=c_int), value, intent(in) :: unit
        end subroutine hicl_timer_uset
                
        subroutine hicl_timer_tick() bind(c, name = "hicl_timer_tick")
        end subroutine hicl_timer_tick
    end interface
    
    public :: hicl_timer_read
    
contains
  
    subroutine hicl_timer_read(time)
        real(kind=8), intent(inout) :: time
        time = c_hicl_timer_read()
    end subroutine hicl_timer_read

end module m_hicl_timer
