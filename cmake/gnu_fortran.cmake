set(CMAKE_Fortran_FLAGS_DEBUG   
    "${CMAKE_Fortran_FLAGS_DEBUG} -D__API_DEBUG -Wall -O0 -g")
set(CMAKE_Fortran_FLAGS_RELEASE 
    "${CMAKE_Fortran_FLAGS_RELEASE} -cpp -w -ffree-line-length-none -funroll-all-loops -fno-f2c -O3")
set(CMAKE_Fortran_FLAGS         
    "${CMAKE_Fortran_FLAGS} -fbackslash")
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${OpenMP_C_FLAGS}")
else (OPENMP_FOUND)
    set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Wunknown-pragmas")
endif (OPENMP_FOUND)