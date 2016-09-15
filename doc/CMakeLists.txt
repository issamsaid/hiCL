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
## @file doc/CMakeLists.txt
## @author Issam SAID
## @brief CMake build script for the hiCL documentation.
##
project (doc)
cmake_minimum_required (VERSION 2.8)

find_package(Doxygen)
if (DOXYGEN_FOUND)
	add_custom_command(
		OUTPUT
			${CMAKE_CURRENT_SOURCE_DIR}/doxygen.stamp
		COMMAND
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
		COMMAND
			cmake -E touch ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.stamp 
		DEPENDS
			${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY 
			${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT 
			"... Generating hiCL documentation with Doxygen" 
		VERBATIM)
	add_custom_target(
    	doc ALL
    	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.stamp 
    )
else (DOXYGEN_FOUND)
	message(STATUS "... Doxygen not found, skipping documentation")
endif (DOXYGEN_FOUND)