set(CMAKE_Fortran_FLAGS_DEBUG   
    "${CMAKE_Fortran_FLAGS_DEBUG} -g -O0 -debug all -check bounds -check pointers")
set(CMAKE_Fortran_FLAGS_DEBUG   
    "${CMAKE_Fortran_FLAGS_DEBUG} -check uninit -fp-stack-check -fp-model strict")
set(CMAKE_Fortran_FLAGS_RELEASE 
    "${CMAKE_Fortran_FLAGS_RELEASE} -sox -cpp -assume byterecl -traceback -c -w -ftz")
set(CMAKE_Fortran_FLAGS_RELEASE 
    "${CMAKE_Fortran_FLAGS_RELEASE} -O3 -xSSSE3 -fno-fnalias -fno-alias -vec-report1")
set(CMAKE_Fortran_FLAGS         
    "${CMAKE_Fortran_FLAGS} -backslash")
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${OpenMP_C_FLAGS}")
else (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Wunknown-pragmas")
endif (OPENMP_FOUND)