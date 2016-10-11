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
## @file CMakeLists.txt
## @author Issam SAID
## @brief CMake main build script for hiCL.
##
project (hicl_distribution C CXX Fortran)
cmake_minimum_required (VERSION 2.8)

## Enable ExternalProject CMake module
include(ExternalProject)
find_package(Threads REQUIRED)

## Overwrite the install prefix
set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

## Add extra cmake modules dir
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## Configurable options for how we want to build hiCL
option(hicl_debug       "Build hiCL with the debug mode."                  OFF)
option(hicl_verbose     "Build hiCL with the verbose mode activated."       ON)
option(hicl_use_tty     "Build hiCL with logging into  the terminal."       ON)
option(hicl_roundup_wrk "Build hiCL with rounding up the workgroup sizes."  ON)

## Set the build type (DEFAULT is Release)
if (NOT CMAKE_BUILD_TYPE)
   set (CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)
if (hicl_debug)
	 set (CMAKE_BUILD_TYPE Debug)
endif (hicl_debug)
message(STATUS "... CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")

## Set the compilation flags 
if ("${CMAKE_C_COMPILER_ID}"     MATCHES "Cray")
  	# using Cray C
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
    include(${CMAKE_MODULE_PATH}/gnu_c.cmake)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Intel")
    include(${CMAKE_MODULE_PATH}/intel_c.cmake)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "PGI")
    include(${CMAKE_MODULE_PATH}/pgi_c.cmake)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "MSVC")
  	# using Visual Studio 
endif ("${CMAKE_C_COMPILER_ID}"     MATCHES "Cray")

if ("${CMAKE_Fortran_COMPILER_ID}"     MATCHES "Cray")
  	# using Cray Fortran
elseif ("${CMAKE_Fortran_COMPILER_ID}" MATCHES "GNU")
	include(${CMAKE_MODULE_PATH}/gnu_fortran.cmake)
elseif ("${CMAKE_Fortran_COMPILER_ID}" MATCHES "Intel")
  	include(${CMAKE_MODULE_PATH}/intel_fortran.cmake)
elseif ("${CMAKE_Fortran_COMPILER_ID}" MATCHES "PGI")
    	include(${CMAKE_MODULE_PATH}/pgi_fortran.cmake)
elseif ("${CMAKE_Fortran_COMPILER_ID}" MATCHES "MSVC")
  	# using Visual Studio Fortran
endif ("${CMAKE_Fortran_COMPILER_ID}"     MATCHES "Cray")

if ("${CMAKE_CXX_COMPILER_ID}"     MATCHES "Cray")
  	# using Cray C++
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	include(${CMAKE_MODULE_PATH}/gnu_cxx.cmake)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    include(${CMAKE_MODULE_PATH}/intel_cxx.cmake)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "PGI")
    include(${CMAKE_MODULE_PATH}/pgi_cxx.cmake)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  	# using Visual Studio C++
endif ("${CMAKE_CXX_COMPILER_ID}"     MATCHES "Cray")

if (hicl_verbose)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__API_VERBOSE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__API_VERBOSE")
endif (hicl_verbose)
if (hicl_use_tty)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__API_LOG_STD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__API_LOG_STD")
endif (hicl_use_tty)
if (hicl_roundup_wrk)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__API_ROUNDUP_WRK")
endif (hicl_roundup_wrk)

## Check whether OpenCL is detected:
## - The OpenCL library queered in usual directories
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
			"... Set the OPENCL_INCLUDE_DIRS variable to locate the headers.\n")
endif (OPENCL_FOUND)
	
if (${OPENCL_LIBRARIES} EQUAL 'OPENCL_LIBRARIES-NOTFOUND')
	message(FATAL_ERROR 
			"... Unable to find your OpenCL implementation library:\n"
			"... Set the OPENCL_LIB_DIRS variable to locate the library.\n"
			"... You may also add the library path to the LD_LIBRARY_PATH env.")
else (${OPENCL_LIBRARIES} EQUAL 'OPENCL_LIBRARIES-NOTFOUND')
	message(STATUS "... OpenCL library paths : ${OPENCL_LIBRARY_DIRS}")
	message(STATUS "... OpenCL libraries     : ${OPENCL_LIBRARIES}")
endif (${OPENCL_LIBRARIES} EQUAL 'OPENCL_LIBRARIES-NOTFOUND')

##
## Use the ulist library if the user sets its path 
## or download and install it as an external project
##
if (DEFINED ENV{ULIST_DIR})
    include_directories("$ENV{ULIST_DIR}/include")
    add_library(libulist STATIC IMPORTED GLOBAL)
    set_target_properties(libulist PROPERTIES
        "IMPORTED_LOCATION" "$ENV{ULIST_DIR}/lib/libulist.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
else (DEFINED ENV{ULIST_DIR})
    ExternalProject_Add(
        ulist
        GIT_REPOSITORY https://github.com/issamsaid/ulist.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ulist
        INSTALL_COMMAND ""
        UPDATE_COMMAND  "")

    ## Get uilst source and binary directories
    ExternalProject_Get_Property(ulist SOURCE_DIR BINARY_DIR)

    ## Create a libulist target to be used as a endency by hiCL
    add_library(libulist STATIC IMPORTED GLOBAL)
    add_dependencies(libulist ulist)

    ## Set libulist properties
    set_target_properties(libulist PROPERTIES
        "IMPORTED_LOCATION" "${BINARY_DIR}/src/libulist.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

    ## Include ulist headers.
    include_directories("${SOURCE_DIR}/include")
endif (DEFINED ENV{ULIST_DIR})

##
## Use the urb_tree library if the user sets its path 
## or download and install it as an external project.
##
if (DEFINED ENV{URB_TREE_DIR})
    include_directories("$ENV{URB_TREE_DIR}/include")
    add_library(liburb_tree STATIC IMPORTED GLOBAL)
    set_target_properties(liburb_tree PROPERTIES
        "IMPORTED_LOCATION" "$ENV{URB_TREE_DIR}/lib/liburb_tree.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
else (DEFINED ENV{URB_TREE_DIR})
    ExternalProject_Add(
        urb_tree
        GIT_REPOSITORY https://github.com/issamsaid/urb_tree.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/urb_tree
        INSTALL_COMMAND ""
        UPDATE_COMMAND  "")

    ## Get urb_tree source and binary directories.
    ExternalProject_Get_Property(urb_tree SOURCE_DIR BINARY_DIR)

    ## Create a liburb_tree target to be used as a dependency by hiCL.
    add_library(liburb_tree IMPORTED STATIC GLOBAL)
    add_dependencies(liburb_tree urb_tree)

    ## Set liburb_tree properties.
    set_target_properties(liburb_tree PROPERTIES
        "IMPORTED_LOCATION" "${BINARY_DIR}/src/liburb_tree.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

    ## Include urb_tree headers.
    include_directories("${SOURCE_DIR}/include")
endif (DEFINED ENV{URB_TREE_DIR})

## Build the hiCL C/C++ library.
add_subdirectory(src)

## Build the hiCL Fortran interface.
add_subdirectory(fortran_interface EXCLUDE_FROM_ALL)

## Build the hiCL unit testing suit (develop branch only).
add_subdirectory(test EXCLUDE_FROM_ALL)

## Build the hiCL examples.
add_subdirectory(examples EXCLUDE_FROM_ALL)

## Build the hiCL documentation.
add_subdirectory(doc EXCLUDE_FROM_ALL)

##
## Workarounds for the lack of post_install steps:
##
## Add the path to ulist which we need during post-install.
get_target_property(ULIST_LIB libulist IMPORTED_LOCATION)
install(CODE "set(ULIST_LIB \"${ULIST_LIB}\")")

## Add the path to urb_tree which we need during post install.
get_target_property(URB_TREE_LIB liburb_tree IMPORTED_LOCATION)
install(CODE "set(URB_TREE_LIB \"${URB_TREE_LIB}\")")

## Add the paths of the hicl_fortran target which we need during post-install.
get_directory_property(HICL_FORTRAN_BUILD_LIB DIRECTORY 
                       fortran_interface DEFINITION HICL_FORTRAN_BUILD_LIB)
get_directory_property(HICL_FORTRAN_INSTALL_DIR DIRECTORY 
                       fortran_interface DEFINITION HICL_FORTRAN_INSTALL_DIR)
install(CODE "set(HICL_FORTRAN_BUILD_LIB \"${HICL_FORTRAN_BUILD_LIB}\")")
install(CODE "set(HICL_FORTRAN_INSTALL_DIR \"${HICL_FORTRAN_INSTALL_DIR}\")")

## Add the paths of the hicl_test target which we need during post-install.
get_directory_property(HICL_TEST_BUILD_EXE DIRECTORY 
                       test/src DEFINITION HICL_TEST_BUILD_EXE)
get_directory_property(HICL_TEST_INSTALL_DIR DIRECTORY 
                       test/src DEFINITION HICL_TEST_INSTALL_DIR)
install(CODE "set(HICL_TEST_BUILD_EXE \"${HICL_TEST_BUILD_EXE}\")")
install(CODE "set(HICL_TEST_INSTALL_DIR \"${HICL_TEST_INSTALL_DIR}\")")

## Add the paths of hicl_test_fortran which we need during post-install.
get_directory_property(HICL_TEST_FORTRAN_BUILD_EXE DIRECTORY 
                       test/fortran_interface 
                       DEFINITION HICL_TEST_FORTRAN_BUILD_EXE)
get_directory_property(HICL_TEST_FORTRAN_INSTALL_DIR DIRECTORY 
                       test/fortran_interface 
                       DEFINITION HICL_TEST_FORTRAN_INSTALL_DIR)
install(CODE 
        "set(HICL_TEST_FORTRAN_BUILD_EXE \"${HICL_TEST_FORTRAN_BUILD_EXE}\")")
install(CODE 
        "set(HICL_TEST_FORTRAN_INSTALL_DIR \"${HICL_TEST_FORTRAN_INSTALL_DIR}\")")

## Add the paths of the c_examples target which we need during post-install.
get_directory_property(HICL_C_EXAMPLES_FILES DIRECTORY 
                       examples/src DEFINITION HICL_C_EXAMPLES_FILES)
get_directory_property(HICL_C_EXAMPLES_INSTALL_DIR DIRECTORY 
                       examples/src DEFINITION HICL_C_EXAMPLES_INSTALL_DIR)
install(CODE 
        "set(HICL_C_EXAMPLES_FILES \"${HICL_C_EXAMPLES_FILES}\")")
install(CODE 
        "set(HICL_C_EXAMPLES_INSTALL_DIR \"${HICL_C_EXAMPLES_INSTALL_DIR}\")")

## Add the paths of the fortran_examples target which 
## we need during post-install.
get_directory_property(HICL_FORTRAN_EXAMPLES_FILES DIRECTORY 
                       examples/fortran_interface 
                       DEFINITION HICL_FORTRAN_EXAMPLES_FILES)
get_directory_property(HICL_FORTRAN_EXAMPLES_INSTALL_DIR DIRECTORY 
                       examples/fortran_interface 
                       DEFINITION HICL_FORTRAN_EXAMPLES_INSTALL_DIR)
install(CODE 
        "set(HICL_FORTRAN_EXAMPLES_FILES \"${HICL_FORTRAN_EXAMPLES_FILES}\")")
install(CODE 
        "set(HICL_FORTRAN_EXAMPLES_INSTALL_DIR \"${HICL_FORTRAN_EXAMPLES_INSTALL_DIR}\")")

## Add a subdirectory that contains the post-install scripts.
add_subdirectory(${CMAKE_SOURCE_DIR}/cmake/post_install)