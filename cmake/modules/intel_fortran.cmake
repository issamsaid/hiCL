set(CMAKE_Fortran_FLAGS_DEBUG   
    "${CMAKE_Fortran_FLAGS_DEBUG} -D__API_DEBUG -g -O0 -debug all")
set(CMAKE_Fortran_FLAGS_DEBUG   
    "${CMAKE_Fortran_FLAGS_DEBUG} -check uninit -fp-stack-check -fp-model strict")
set(CMAKE_Fortran_FLAGS_RELEASE 
    "${CMAKE_Fortran_FLAGS_RELEASE} -sox -cpp -assume byterecl -traceback -w -ftz")
set(CMAKE_Fortran_FLAGS_RELEASE 
    "${CMAKE_Fortran_FLAGS_RELEASE} -O3 -fno-fnalias -fno-alias -vec-report1")
set(CMAKE_Fortran_FLAGS         
    "${CMAKE_Fortran_FLAGS} -assume bscc")
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${OpenMP_C_FLAGS}")
else (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Wunknown-pragmas")
endif (OPENMP_FOUND)
