##
## @copyright Copyright (c) 2013-2016, Université Pierre et Marie Curie
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
## @file src/CMakeLists.txt
## @author Issam SAID
## @brief CMake build script for the hiCL C library.
##
project (hicl C)
cmake_minimum_required (VERSION 2.8)

## Build the hiCL C static library.
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB C_SRCS "*.c")
add_library(hicl STATIC ${C_SRCS})
add_dependencies(hicl libulist)
add_dependencies(hicl liburb_tree)
set_target_properties(hicl PROPERTIES OUTPUT_NAME "hiCL")
install(TARGETS hicl ARCHIVE DESTINATION lib)

## Add the pathes to hicl which we need during post install.
get_target_property(HICL_BUILD_LIB hicl LOCATION)
get_filename_component(HICL_BUILD_DIR ${HICL_BUILD_LIB} DIRECTORY)
string(REPLACE ${HICL_BUILD_DIR} ${CMAKE_SOURCE_DIR}/lib HICL_INSTALL_LIB 
       ${HICL_BUILD_LIB})

install(CODE "set(HICL_BUILD_LIB \"${HICL_BUILD_LIB}\")")
install(CODE "set(HICL_INSTALL_LIB \"${HICL_INSTALL_LIB}\")")
