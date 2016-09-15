##
## @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
## All rights reserved.
##
## <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
## funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
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
## @file test/src/CMakeLists.txt
## @author Issam SAID
## @brief CMake build script for the hiCL C/C++ unit testing.
##
project (hicl_test CXX)
cmake_minimum_required (VERSION 2.8)

## Download and install googletest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
)

## Get googletest source and binary directories from CMake project
ExternalProject_Get_Property(gtest SOURCE_DIR BINARY_DIR)

## Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

## Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${BINARY_DIR}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

## Include the googletest headers
include_directories("${SOURCE_DIR}/googletest/include")

## Include the hiCL headers
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB CXX_SRCS "*.cc")

add_executable(hicl_test ${CXX_SRCS})

## Link the executable to the hiCL, OpenCL, ulist and urb_tree and gtest
target_link_libraries (hicl_test LINK_PUBLIC hicl)
target_link_libraries (hicl_test LINK_PUBLIC libulist)
target_link_libraries (hicl_test LINK_PUBLIC liburb_tree)
target_link_libraries (hicl_test LINK_PUBLIC OpenCL)
target_link_libraries (hicl_test LINK_PUBLIC libgtest)
add_dependencies(hicl_test hicl)
install(TARGETS hicl_test DESTINATION test/bin OPTIONAL)