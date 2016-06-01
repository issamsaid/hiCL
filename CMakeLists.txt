cmake_minimum_required (VERSION 2.8)

project (hiCL)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package (OpenCL)

if (OPENCL_FOUND)
	message(STATUS "OpenCL found: ${OPENCL_FOUND}")
	message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
	message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
	message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")

	include_directories(${OPENCL_INCLUDE_DIRS})
	#target_link_libraries (hiCL ${OPENCL_LIBRARIES})
else (OPENCL_FOUND)
	message(FATAL_ERROR "Unable to find the OpenCL include dirs:\n"
			            "Please set OPENCL_INCLUDE_DIRS env variable")
endif (OPENCL_FOUND)

include_directories(include)
file(GLOB C_SRCS "src/*.c")
add_library(hiCL STATIC ${C_SRCS})
install(TARGETS hiCL ARCHIVE DESTINATION lib)
