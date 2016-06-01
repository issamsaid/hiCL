module m_handler
    use iso_c_binding
    use m_hicl_timer
    use m_hicl_types
    use m_hicl_base

    implicit none
    
    private
    public :: handler_start, handler_end, handler_stat
    public :: global_setup, global_teardown, run, run_standalone

    integer                           :: passed = 0
    integer                           :: failed = 0
    character(len=48), dimension(128) :: failed_tests
    real(kind=8)                      :: elapsed
 
contains

    subroutine global_setup()
        implicit none
        call hicl_init(ALL)
    end subroutine global_setup

    subroutine global_teardown()
        implicit none
        call hicl_release()
    end subroutine global_teardown

    subroutine run(local_setup, local_teardown, func, name)
        implicit none
        external                     :: local_setup
        external                     :: local_teardown
        logical, external            :: func
        character(len=*), intent(in) :: name
        logical                      :: status
        write (*,"(A, A)") '\x1B[32m[ RUN      ]\x1B[0m ', name
        call global_setup()
        call local_setup()
        status = func()
        call local_teardown()
        call global_teardown()
        if (status .eqv. .true.) then
            write (*,"(A)") '\x1B[32m[       OK ]\x1B[0m'
            passed = passed + 1
        else
            write (*,"(A)") '\x1B[31m[ FAILED   ]\x1B[0m'
            failed = failed + 1
            failed_tests(failed) = name
        end if
        write (*,*)
    end subroutine run

    subroutine run_standalone(local_setup, local_teardown, func, name)
        implicit none
        external                     :: local_setup
        external                     :: local_teardown
        logical, external            :: func
        character(len=*), intent(in) :: name
        logical                      :: status
        write (*,"(A, A)") '\x1B[32m[ RUN      ]\x1B[0m ', name
        call local_setup()
        status = func()
        call local_teardown()
        if (status .eqv. .true.) then
            write (*,"(A)") '\x1B[32m[       OK ]\x1B[0m'
            passed = passed + 1
        else
            write (*,"(A)") '\x1B[31m[ FAILED   ]\x1B[0m'
            failed = failed + 1
            failed_tests(failed) = name
        end if
    end subroutine run_standalone

    subroutine handler_start()
        implicit none
        integer(kind(SECONDS)) :: s  = SECONDS
        write (*,"(A)") '\x1B[32m[----------]\x1B[0m'
        call hicl_timer_uset(s)
        call hicl_timer_tick()
    end subroutine handler_start

    subroutine handler_end()
        implicit none
        call hicl_timer_read(elapsed)
        write (*,"(A)") '\x1B[32m[----------]\x1B[0m'
        write (*,*)
    end subroutine handler_end

    subroutine handler_stat()
        implicit none
        integer :: i
        write (*,*)
        write (*,"(A, I3, A, F20.10, A)") '\x1B[32m[==========]\x1B[0m Total of',&
              passed + failed, ' tests ran (', elapsed,' s).'
        if (passed > 0) then
            write (*,"(A, I3, A)") '\x1B[32m[  PASSED  ]\x1B[0m ', &
                  passed, ' tests.'
        end if
        if (failed > 0) then
            write (*,"(A, I3, A)") '\x1B[31m[  FAILED  ]\x1B[0m ', &
                  failed, ' tests:'
            do i=1,failed
               write (*,"(A,I3,A,A,A)") '\x1B[31m', i, " - ", failed_tests(i),'\x1B[0m'
            end do
        end if
        write (*,*)
    end subroutine handler_stat

end module m_handler
