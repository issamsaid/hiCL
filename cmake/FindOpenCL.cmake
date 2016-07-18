##
## @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
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
## 
## It defines the following variables:
##      - OPENCL_FOUND (found the OPENCL framework)
##      - OPENCL_INCLUDE_DIRS (include directories)
##
## It also defines the library variables below:
##      - OPENCL_LIBRARIES libOpenCL
##
## It accepts the following variables as input:
## @param OPENCL_INCLUDE_DIR the root directory of the OpenCL implementation 
## headers can be found.
##     
function(_FIND_OPENCL_VERSION)
  include(CheckSymbolExists)
  include(CMakePushCheckState)
  set(CMAKE_REQUIRED_QUIET ${OpenCL_FIND_QUIETLY})

  CMAKE_PUSH_CHECK_STATE()
  foreach(VERSION "2_0" "1_2" "1_1" "1_0")
    set(CMAKE_REQUIRED_INCLUDES "${OpenCL_INCLUDE_DIR}")

    if(APPLE)
      CHECK_SYMBOL_EXISTS(
        CL_VERSION_${VERSION}
        "${OpenCL_INCLUDE_DIR}/OpenCL/cl.h"
        OPENCL_VERSION_${VERSION})
    else()
      CHECK_SYMBOL_EXISTS(
        CL_VERSION_${VERSION}
        "${OpenCL_INCLUDE_DIR}/CL/cl.h"
        OPENCL_VERSION_${VERSION})
    endif()

    if(OPENCL_VERSION_${VERSION})
      string(REPLACE "_" "." VERSION "${VERSION}")
      set(OpenCL_VERSION_STRING ${VERSION} PARENT_SCOPE)
      string(REGEX MATCHALL "[0-9]+" version_components "${VERSION}")
      list(GET version_components 0 major_version)
      list(GET version_components 1 minor_version)
      set(OpenCL_VERSION_MAJOR ${major_version} PARENT_SCOPE)
      set(OpenCL_VERSION_MINOR ${minor_version} PARENT_SCOPE)
      break()
    endif()
  endforeach()
  CMAKE_POP_CHECK_STATE()
endfunction()

find_path(OPENCL_INCLUDE_DIRS
    NAMES OpenCL/cl.h CL/cl.h
    HINTS
        ${OPENCL_INCLUDE_DIR}/
        $ENV{AMDAPPSDKROOT}/include
        $ENV{CUDA_PATH}/include
        $ENV{AMDAPPSDKROOT}/include
        $ENV{INTELOCLSDKROOT}/include
        $ENV{NVSDKCOMPUTE_ROOT}/include
        $ENV{ATISTREAMSDKROOT}/include
    PATHS
        /usr/include
        /usr/local/include
        /usr/local/cuda/include
        /opt/cuda/include
    DOC "OpenCL header file path"
)
mark_as_advanced( OPENCL_INCLUDE_DIRS )

if (OPENCL_FOUND)
_FIND_OPENCL_VERSION()
endif()

## Search for 64bit libs if FIND_LIBRARY_USE_LIB64_PATHS is set to 
## true in the global environment, 32bit libs else
get_property( LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS )

if( LIB64 )
    find_library( OPENCL_LIBRARIES
        NAMES OpenCL
        HINTS
            ${OPENCL_ROOT}/lib
            $ENV{AMDAPPSDKROOT}/lib
            $ENV{CUDA_PATH}/lib
        DOC "OpenCL dynamic library path"
        PATH_SUFFIXES x86_64 x64 x86_64/sdk
        PATHS
            /usr/lib
            /usr/local/cuda/lib
            /opt/cuda/lib
    )
else( )
    find_library( OPENCL_LIBRARIES
        NAMES OpenCL
        HINTS
            ${OPENCL_ROOT}/lib
            $ENV{AMDAPPSDKROOT}/lib
            $ENV{CUDA_PATH}/lib
        DOC "OpenCL dynamic library path"
        PATH_SUFFIXES x86 Win32
        
        PATHS
            /usr/lib
            /usr/local/cuda/lib
            /opt/cuda/lib
    )
endif( )
mark_as_advanced( OPENCL_LIBRARIES )

include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OPENCL DEFAULT_MSG 
                                  OPENCL_LIBRARIES 
                                  OPENCL_INCLUDE_DIRS )

