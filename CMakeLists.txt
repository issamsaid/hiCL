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
## @file CMakeLists.txt
## @author Issam SAID
## @brief CMake main build script for hiCL.
##
project (hiCL-distribution)
cmake_minimum_required (VERSION 2.8)

## Overwrite CMake default behavior
set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if ( NOT CMAKE_BUILD_TYPE )
   set (CMAKE_BUILD_TYPE Release)
endif ()
message(STATUS "... CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")

## Check whether OpenCL is detected:
## - The OpenCL library quered in usual directories
## - This behavior can be changed with env setting:
##   OPENCL_INCLUDE_DIR: contains the OpenCL headers
##   OPENCL_LIBRARY_DIR: contains the OpenCL library
find_package (OpenCL)
if (OPENCL_FOUND)
	message(STATUS "... OpenCL headers paths : ${OPENCL_INCLUDE_DIRS}")
	include_directories(${OPENCL_INCLUDE_DIRS})
else (OPENCL_FOUND)
	message(FATAL_ERROR 
			"... Unable to find your OpenCL implementation headers:\n"
			"... Set the OPENCL_INCLUDE_DIRS variable to locate the headers.\n"
			)
endif (OPENCL_FOUND)
	
if (${OPENCL_LIBRARIES} EQUAL 'OPENCL_LIBRARIES-NOTFOUND')
	message(FATAL_ERROR 
			"... Unable to find your OpenCL implementation library:\n"
			"... Set the OPENCL_LIB_DIRS variable to locate the library.\n"
			"... You may also add the library path to the LD_LIBRARY_PATH env."
			)
else ()
	message(STATUS "... OpenCL library paths : ${OPENCL_LIBRARY_DIRS}")
	message(STATUS "... OpenCL libraries     : ${OPENCL_LIBRARIES}")
endif ()

## Build the hiCL C/C++ library.
add_subdirectory(src)

## Build the hiCL fortran interface.
add_subdirectory(fortran_interface)

## Build the hiCL unit testing suit (develop branch only).
add_subdirectory(test)

## Build the hiCL documentation.
add_subdirectory(doc EXCLUDE_FROM_ALL)

## Build the hiCL examples.
add_subdirectory(examples)