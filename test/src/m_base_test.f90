module m_base_test
    use m_hicl_types
    use m_hicl_base
    use m_handler
    
    implicit none

    private

    public :: base_test
    
contains
    
    subroutine setup()
    end subroutine setup

    subroutine teardown()
    end subroutine teardown

    subroutine base_test()
        call run_standalone(setup, teardown, get_defaults, &
              "base_test.get_defaults")
        call run_standalone(setup, teardown, get_explicit_defaults, &
              "base_test.get_explicit_defaults")
        call run_standalone(setup, teardown, get_amd, &
              "base_test.get_amd")
        call run_standalone(setup, teardown, get_apple, &
              "base_test.get_apple")
        call run_standalone(setup, teardown, get_nvidia, &
              "base_test.get_nvidia")
        call run_standalone(setup, teardown, get_undefined_with_cpu, &
              "base_test.get_undefined_with_cpu")
        call run_standalone(setup, teardown, get_undefined_with_gpu, &
              "base_test.get_undefined_with_gpu")
    end subroutine base_test

    logical function get_defaults() result(status)
        implicit none
        call hicl_init()
        call hicl_release()
        status = .true.
    end function get_defaults

    logical function get_explicit_defaults() result(status)
        implicit none
         call hicl_init(DEFAULT)
        call hicl_release()
        status = .true.
    end function get_explicit_defaults

    logical function get_amd() result(status)
        implicit none
        call hicl_init(AMD)
        call hicl_release()
        status = .true.
    end function get_amd

    logical function get_apple() result(status)
        implicit none
        call hicl_init(APPLE)
        call hicl_release()
        status = .true.
    end function get_apple

    logical function get_nvidia() result(status)
        implicit none
        call hicl_init(NVIDIA)
        call hicl_release()
        status = .true.
    end function get_nvidia

    logical function get_undefined_with_cpu() result(status)
        implicit none
        call hicl_init(ior(CPU, FIRST))
        call hicl_release()
        status = .true.
    end function get_undefined_with_cpu

    logical function get_undefined_with_gpu() result(status)
        implicit none
        if (hicl_has(DEFAULT).eqv. .true.) then
            call hicl_init(ior(GPU, FIRST))
            call hicl_release()
        end if
        status = .true.
    end function get_undefined_with_gpu

    logical function get_undefined_with_types() result(status)
        implicit none
        if (hicl_has(DEFAULT).eqv. .true.) then
            call hicl_init(ior(GPU, CPU))
            call hicl_release()
        end if
        status = .true. ! todo
    end function get_undefined_with_types

    logical function get_undefined_with_indexes() result(status)
        implicit none
        if (hicl_has(DEFAULT).eqv. .true.) then
            call hicl_init(ior(FIRST, SECOND))
            call hicl_release()
        end if
        status = .true. ! todo
    end function get_undefined_with_indexes

    logical function get_undefined_with_mixed() result(status)
        implicit none
        if (hicl_has(DEFAULT).eqv. .true.) then
            call hicl_init(ior(ior(GPU, CPU), ior(FIRST, SECOND)))
            call hicl_release()
        end if
        status = .true. ! todo
    end function get_undefined_with_mixed

end module m_base_test
