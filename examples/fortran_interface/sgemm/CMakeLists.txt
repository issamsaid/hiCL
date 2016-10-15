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
## @file examples/fortran_interface/sgemm/CMakeLists.txt
## @author Issam SAID
## @brief CMake build script for the hiCL Fortran sgemm.
##
project (sgemm_fortran Fortran)
cmake_minimum_required (VERSION 2.8)

file(GLOB F_SRCS "*.f90")
add_definitions(-DPREFIX=\"${CMAKE_CURRENT_SOURCE_DIR}\")

add_executable(sgemm_fortran ${F_SRCS})
target_link_libraries (sgemm_fortran LINK_PUBLIC hicl_fortran)
target_link_libraries (sgemm_fortran LINK_PUBLIC hicl)
target_link_libraries (sgemm_fortran LINK_PUBLIC OpenCL)
target_link_libraries (sgemm_fortran LINK_PUBLIC libulist)
target_link_libraries (sgemm_fortran LINK_PUBLIC liburb_tree)
add_dependencies(sgemm_fortran hicl_fortran)

##
## Workaround to install this target simultaneously with the main target.
## This is due to a cmake restriction: 
##  - Installing a target with the EXCLUDE_FROM_ALL target property 
##    set to TRUE has undefined behavior. 
##  
## Inform the build location of the target. 
get_target_property(VAR sgemm_fortran LOCATION)
set (HICL_FORTRAN_EXAMPLES_FILES 
     "${HICL_FORTRAN_EXAMPLES_FILES}" "${VAR}" PARENT_SCOPE)