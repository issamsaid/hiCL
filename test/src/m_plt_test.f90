module m_plt_test
    use iso_c_binding
    use m_hicl_types
    use m_hicl_plt
    use m_handler
    
    implicit none

    private

    public :: plt_test

contains
    
    subroutine setup()
    end subroutine setup

    subroutine teardown()
    end subroutine teardown

    subroutine plt_test()
        call run(setup, teardown, find_default, "plt_test.find_default")
        call run(setup, teardown, find_apple,   "plt_test.find_apple")
        call run(setup, teardown, find_amd,     "plt_test.find_amd")
        call run(setup, teardown, find_nvidia,  "plt_test.find_nvidia")
        call run(setup, teardown, info, "plt_test.info")
        call run(setup, teardown, extension_supported, &
              "plt_test.extension_supported")
    end subroutine plt_test

    logical function find_default() result(status)
        implicit none
        type(c_ptr) :: plt
        plt = hicl_plt_find(DEFAULT)
        status = (c_associated(plt))
    end function find_default

    logical function find_apple() result(status)
        implicit none
        type(c_ptr) :: plt
        plt = hicl_plt_find(APPLE)
        status = (c_associated(plt))
    end function find_apple

    logical function find_amd() result(status)
        implicit none
        type(c_ptr) :: plt
        plt = hicl_plt_find(AMD)
        status = (c_associated(plt))
    end function find_amd

    logical function find_nvidia() result(status)
        implicit none
        type(c_ptr) :: plt
        plt = hicl_plt_find(NVIDIA)
        status = (c_associated(plt))
    end function find_nvidia

    logical function info() result(status)
        implicit none
        type(c_ptr)   :: plt
        plt = hicl_plt_find(DEFAULT)
        status = (c_associated(plt))
        call hicl_plt_info(plt)
    end function info

    logical function extension_supported() result(status)
        implicit none
        type(c_ptr)   :: plt
        plt = hicl_plt_find(DEFAULT)
        status = hicl_plt_extension_supported(plt, "cl_khr_icd") &
              .and. .not. hicl_plt_extension_supported(plt, "fake")
    end function extension_supported

end module m_plt_test
