##
## @copyright Copyright (c) 2013-2016, Université Pierre et Marie Curie
## All rights reserved.
##
## <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
## funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permetted provided that the following conditions
## are met:
##
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. Neither the name of the UPMC nor the names of its contributors
##    may be used to endorse or promote products derived from this software
##    without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
## INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
## ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
## EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
## LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
## NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
## SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
## @file FindOpenCL.cmake
## @author Issam SAID
## @brief FindOpenCL.cmake is a cmake module to locate an OpenCL implementation.
##  
## @details It currently supports: 
## 		- AMD APP SDK 
##    - NVIDIA SDK
##    - Apple SDK
##    - Intel SDK
## 
## It defines the following variables:
##      - OPENCL_FOUND (found the OPENCL framework)
##      - OPENCL_INCLUDE_DIRS (OpenCL headers directories)
##      - OPENCL_LIBRARIES (OpenCL libraries names)
##
## It accepts the following variables as inputs, which will overwrite the 
## behavior of the package locater:
## @param OPENCL_INCLUDE_DIRS if is set by the user it will be used instead of 
## script hints, to locate the directories of the OpenCL headers.
## @param OPENCL_LIBRARIES_DIRS if is set by the user it will be used to locate 
## the OpenCL library. Note that the OpenCL library path can be manually added 
## to the variable LD_LIBRARY_PATH.
##     
if (NOT OPENCL_INCLUDE_DIRS)
  find_path(OPENCL_INCLUDE_DIRS
    NAMES OpenCL/cl.h CL/cl.h
    HINTS
        $ENV{CUDA_PATH}/include
        $ENV{CUDA_ROOT}/include
        $ENV{CUDATOOLKIT}/include
        $ENV{CUDATOOLKIT_HOME}/include
        $ENV{AMDAPPSDK}/include
        $ENV{AMDAPPSDKROOT}/include
        $ENV{ATISTREAMSDKROOT}/include
        $ENV{INTELOCLSDKROOT}/include
        $ENV{NVSDKCOMPUTE_ROOT}/include
    PATHS
        /usr/include
        /usr/local/include
        /usr/local/cuda/include
        /opt/cuda/include
    DOC "OpenCL headers possible paths"
  )
  mark_as_advanced(OPENCL_INCLUDE_DIRS)
endif()

get_property(USE_LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

find_library(OPENCL_LIBRARIES
    NAMES OpenCL
    HINTS
        $ENV{CUDA_PATH}/lib64
        $ENV{CUDA_PATH}/lib
        $ENV{CUDA_ROOT}/lib64
        $ENV{CUDA_ROOT}/lib
        $ENV{CUDATOOLKIT}/lib64
        $ENV{CUDATOOLKIT}/lib
        $ENV{CUDATOOLKIT_HOME}/lib64
        $ENV{CUDATOOLKIT_HOME}/lib
        $ENV{OPENCL_LIBRARY_DIRS}
        $ENV{AMDAPPSDKROOT}/lib64
        $ENV{AMDAPPSDKROOT}/lib
        $ENV{AMDAPPSDK}/lib64
        $ENV{AMDAPPSDK}/lib
        $ENV{ATISTREAMSDKROOT}/lib64
        $ENV{ATISTREAMSDKROOT}/lib
    DOC "OpenCL dynamic libraries (64 bits) paths"
    PATH_SUFFIXES x86_64 x64 x86_64/sdk x86 Win32 
    PATHS
      /usr/lib
      /usr/local/cuda/lib
      /opt/cuda/lib
  )
if (NOT OPENCL_LIBRARIES_DIRS)
    get_filename_component(OPENCL_LIBRARIES_DIRS ${OPENCL_LIBRARIES} DIRECTORY)
endif ()
link_directories(${OPENCL_LIBRARIES_DIRS})
mark_as_advanced(OPENCL_LIBRARIES)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OPENCL 
                                  DEFAULT_MSG 
                                  OPENCL_LIBRARIES 
                                  OPENCL_INCLUDE_DIRS)
