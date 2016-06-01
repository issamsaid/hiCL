cmake_minimum_required (VERSION 2.8)

project (hiCL)

TUDE_DIRS}")
	message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
	message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")

	include_directories(${OPENCL_INCLUDE_DIRS})
	#target_link_libraries (hiCL ${OPENCL_LIBRARIES})
else (OpenCL_FOUND)
	message(FATAL_ERROR "Unable to find the OpenCL include dirs:\n"
			            "Please set OPENCL_INCLUDE_DIRS env variable")
endif (OPENCL_FOUND)

add_library(hiCL STATIC ${C_SRCS})
install(TARGETS hiCL ARCHIVE DESTINATION lib)
