module m_dev_test
    use, intrinsic :: iso_c_binding
    use m_hicl_types
    use m_hicl_base
    use m_hicl_dev
    use m_handler
    
    implicit none

    private

    public :: dev_test

contains
    
    subroutine setup()
    end subroutine setup

    subroutine teardown()
    end subroutine teardown

    subroutine dev_test()
        call run(setup, teardown, find, &
              "dev_test.find")
        call run(setup, teardown, all_devs, &
              "dev_test.all_devs")
        call run(setup, teardown, default_dev, &
              "dev_test.default_dev")
        call run(setup, teardown, cpu_dev, &
              "dev_test.cpu_dev")
        call run(setup, teardown, gpu_dev, &
              "dev_test.gpu_dev")
        call run(setup, teardown, acc_dev, &
              "dev_test.acc_dev")
        call run(setup, teardown, extension_supported, &
              "dev_test.extension_supported")
        call run(setup, teardown, info, "dev_test.info")
    end subroutine dev_test

    logical function find() result(status)
        type(dev), pointer :: dev0
        call hicl_dev_find(DEFAULT, dev0)
        status = associated(dev0) .and. (hicl_count(DEVICE) > 0)
    end function find

    logical function all_devs() result(status)
        type(dev), pointer :: dev0
        type(dev), pointer :: dev1
        type(dev), pointer :: dev2
        call hicl_dev_find(ALL, dev0);
        call hicl_dev_find(ior(ALL, FIRST), dev1);
        call hicl_dev_find(FIRST, dev2);
        status = &
              associated(dev0, dev1) .and. &
              associated(dev1, dev2) .and. &
              associated(dev2, dev0) .and. &
              (hicl_count(DEVICE) .ge. 1)
    end function all_devs

    logical function default_dev() result(status)
        type(dev), pointer :: dev0
        type(dev), pointer :: dev1
        type(dev), pointer :: dev2
        call hicl_dev_find(DEFAULT, dev0);
        call hicl_dev_find(ior(DEFAULT, FIRST), dev1);
        call hicl_dev_find(FIRST, dev2);
        status = &
              associated(dev0, dev1) .and. &
              associated(dev1, dev2) .and. &
              associated(dev2, dev0) .and. &
              (hicl_count(DEVICE) .ge. 1)
    end function default_dev

    logical function cpu_dev() result(status)
        type(dev), pointer :: dev0
        type(dev), pointer :: dev1
        call hicl_dev_find(CPU, dev0);
        call hicl_dev_find(ior(CPU, FIRST), dev1);
        status = &
              associated(dev0, dev1) .and. &
              (hicl_count(DEVICE) .ge. 1)
    end function cpu_dev

    logical function gpu_dev() result(status)
        type(dev), pointer :: dev0
        type(dev), pointer :: dev1
        if (hicl_has(DEFAULT)) then
            call hicl_dev_find(GPU, dev0);
            call hicl_dev_find(ior(GPU, FIRST), dev1);
            status = &
                  associated(dev0, dev1) .and. &
                  (hicl_count(DEVICE) .ge. 1)
        else
            status = .true.
        end if
    end function gpu_dev

    logical function acc_dev() result(status)
        type(dev), pointer :: dev0
        type(dev), pointer :: dev1
        if (hicl_has(ACCELERATOR)) then
            call hicl_dev_find(ACCELERATOR, dev0);
            call hicl_dev_find(ior(ACCELERATOR, FIRST), dev1);
            status = &
                  associated(dev0, dev1) .and. &
                  (hicl_count(DEVICE) .ge. 1)
        else
            status = .true.
        end if
    end function acc_dev

    logical function extension_supported() result(status)
        implicit none
        type(dev), pointer :: dev0
        call hicl_dev_find(DEFAULT, dev0)
        status = hicl_dev_extension_supported(dev0, "cl_khr_fp64") .and. &
              .not. hicl_dev_extension_supported(dev0, "fake")
    end function extension_supported

    logical function info() result(status)
        type(dev), pointer :: dev0
        call hicl_dev_find(DEFAULT, dev0)
        status = associated(dev0) .and. (hicl_count(DEVICE) > 0)
        call hicl_dev_info(dev0)
    end function info

end module m_dev_test
