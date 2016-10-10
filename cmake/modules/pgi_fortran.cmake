set(CMAKE_Fortran_FLAGS_DEBUG   
    "${CMAKE_Fortran_FLAGS_DEBUG} -Mpreprocess -w -Mextend -D__API_DEBUG -O0 -Mbounds -Mchkptr -Mchkstk -DLOG_DEBUG")
set(CMAKE_Fortran_FLAGS_RELEASE 
    "${CMAKE_Fortran_FLAGS_RELEASE}  -Mpreprocess -w -Mextend  -O3 -Minfo=opt")
set(CMAKE_Fortran_FLAGS         
    "${CMAKE_Fortran_FLAGS} -Mbackslash")
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -mp=numa")
else (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Wunknown-pragmas")
endif (OPENMP_FOUND)
