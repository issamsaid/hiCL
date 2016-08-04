set(CMAKE_CXX_FLAGS_DEBUG   
    "${CMAKE_CXX_FLAGS_DEBUG} -D__API_DEBUG -g -O0 -debug all")
set(CMAKE_CXX_FLAGS_DEBUG   
    "${CMAKE_CXX_FLAGS_DEBUG} -fp-stack-check -fp-model strict")
set(CMAKE_CXX_FLAGS_RELEASE 
    "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fno-fnalias -fno-alias -vec-report1")
find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
else (OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunknown-pragmas")
endif (OPENMP_FOUND)
