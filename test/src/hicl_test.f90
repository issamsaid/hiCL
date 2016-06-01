program hicl_test
    use m_base_test
    use m_plt_test
    use m_dev_test
    use m_mem_test
    use m_knl_test
    use m_handler

    implicit none
    
    call handler_start()

    call base_test()
    call plt_test()
    call dev_test()
    call mem_test()
    call knl_test()

    call handler_end()
    call handler_stat()

end program hicl_test
