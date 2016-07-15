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
## @file fortran_interface/CMakeLists.txt
## @author Issam SAID
## @brief CMake build script for the hiCL Fortran interface.
##
project (hiCL_fortran Fortran)
cmake_minimum_required (VERSION 2.8)

set (CMAKE_Fortran_FLAGS_DEBUG   
	"${CMAKE_Fortran_FLAGS_DEBUG} -Wall -O0 -g")
set (CMAKE_Fortran_FLAGS_RELEASE 
	"${CMAKE_Fortran_FLAGS_RELEASE} -funroll-all-loops -fno-f2c -O3")

## Build the hiCL fortran static library.
file(GLOB F_SRCS "*.f90")
set(CMAKE_Fortran_MODULE_DIRECTORY "${CMAKE_SOURCE_DIR}/modules")
add_library(hiCL_fortran STATIC ${F_SRCS})
install(TARGETS hiCL_fortran ARCHIVE DESTINATION lib)
add_dependencies(hiCL_fortran hiCL) 