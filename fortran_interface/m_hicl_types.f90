module m_hicl_types
  use, intrinsic :: iso_c_binding

  implicit none

  private

  public :: dev
  type, bind(c) :: dev
      type(c_ptr) :: id
      type(c_ptr) :: queue
  end type dev

  public :: mem
  type, bind(c) :: mem
      integer(c_int64_t) :: flags
      integer(c_size_t)  :: size
      integer(c_size_t)  :: unit_size
      type(c_ptr)        :: id
      type(c_ptr)        :: pinned
      type(c_ptr)        :: queue
      type(c_ptr)        :: h
  end type mem

  public :: knl
  type, bind(c) :: knl
      integer(c_size_t), dimension(3) :: gws
      integer(c_size_t), dimension(3) :: lws
      integer(c_int)                  :: wrk
      type(c_ptr)                     :: id
      type(c_ptr)                     :: mems
  end type knl

  enum, bind(c)
     enumerator :: NANO_SECONDS  = 1
     enumerator :: MICRO_SECONDS = 1000
     enumerator :: MILLI_SECONDS = 1000000
     enumerator :: SECONDS       = 1000000000
  end enum

  public :: NANO_SECONDS
  public :: MICRO_SECONDS
  public :: MILLI_SECONDS
  public :: SECONDS

end module m_hicl_types
