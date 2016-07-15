module m_hicl
    use m_hicl_flags
    use m_hicl_types
    use m_hicl_base
    use m_hicl_dev
    use m_hicl_mem
    use m_hicl_knl
    use m_hicl_util
    use m_hicl_timer

    type(dev),            pointer, public :: hicl_hdw
    integer(kind=8), dimension(3), public :: hicl_gws 
    integer(kind=8), dimension(3), public :: hicl_lws
    integer(kind=8), dimension(3), public :: hicl_ofs
end module m_hicl