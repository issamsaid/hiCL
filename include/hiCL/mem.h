#ifndef HICL_MEM_H_
#define HICL_MEM_H_
///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
/// All rights reserved.
///
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
/// funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permetted provided that the following conditions
/// are met:
///
/// 1. Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright
///    notice, this list of conditions and the following disclaimer in the
///    documentation and/or other materials provided with the distribution.
/// 3. Neither the name of the UPMC nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
/// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
/// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
/// LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// @file hiCL/mem.h
/// @author Issam SAID
/// @brief Function prototypes of OpenCL memory objects manipulation routines.
///
/// @details This file describes the library functions used by hiCL to 
/// initialize, release and manipulate an OpenCL memory descriptor.
///
#include "hiCL/types.h"

CPPGUARD_BEGIN()

///
/// @brief Allocate a memory object on a given device using a preallocated 
///        host pointer.
///
/// This routine allocates a memory object on a given device, the location of 
/// memory object is determined thanks to a set of hiCL flags provided by
/// the user. In all cases, the memory descriptor that is 
/// to be created uses a preallocated pointer as the host memory. For example, 
/// if the memory is to be created on the device, the host memory will be 
/// duplicated on the device, wheareas if the memory is zero-copy the same 
/// host memory will be accessed by the device.
/// @see hiCL/flags.h
/// @param d is the y penCL device in which the device memory will be allocated.
/// @param h is the preallocated host memory.
/// @param size is the size of the memory to be allocated in number of elements.
/// @param flags is the hiCL flags that describe the memory type.
/// @return a hiCL memory descriptor.
///
himem_t hicl_mem_wrap(hidev_t d, address_t h, size_t size, flags_t flags);

///
/// @brief Release a given hiCL memory descriptor.
///
/// This routine frees the OpenCL resources related to a hiCL memory descriptor,
/// identified by a host pointer, which include the OpenCL memory identifier.
/// @param h is the host memory pointer that identifies the descriptor.
/// @return Nothing.
///
void    hicl_mem_release(address_t h);

///
/// @brief Set the access mode before altering a hiCL memory.
///
/// This routine is used to inform that a given hiCL memory is altered by the 
/// user. The modifications are identified with respect to a set of hiCL flags.
/// @see hiCL/flags.h
/// @param h is the host memory pointer that identifies the descriptor.
/// @param flags is the access mode: READ_ONLY, WRITE_ONLY or READ_WRITE.
/// @return Nothing.
///
void    hicl_mem_update(address_t h, flags_t flags);

///
/// @brief Copy a portion of data from host to device.
///
/// This routine copies a portion of data (a 3D cubic) from host to device, 
/// that are related to a given hiCL memory descriptor.
/// @param h is the host memory pointer that identifies the descriptor.
/// @param ix is the first element along the X axis to be copied.
/// @param ex is the last  element along the X axis to be copied.
/// @param iy is the first element along the Y axis to be copied.
/// @param ey is the last  element along the Y axis to be copied.
/// @param iz is the first element along the Z axis to be copied.
/// @param ez is the last  element along the Z axis to be copied.
/// @param xpitch is the stride along the X axis. 
/// @param ypitch is the stride along the Y axis. 
/// @param blocking is a flag to make the transfer blocking or not.
/// @return Nothing.
///
void    hicl_mem_pop(address_t h, int ix, int ex, 
                     int iy, int ey, int iz, int ez, 
                     int xpitch, int ypitch, cl_bool blocking);

///
/// @brief Copy a portion of data from device to host.
///
/// This routine copies a portion of data (a 3D cubic) from device to host, 
/// that are related to a given hiCL memory descriptor.
/// @param h is the host memory pointer that identifies the descriptor.
/// @param ix is the first element along the X axis to be copied.
/// @param ex is the last  element along the X axis to be copied.
/// @param iy is the first element along the Y axis to be copied.
/// @param ey is the last  element along the Y axis to be copied.
/// @param iz is the first element along the Z axis to be copied.
/// @param ez is the last  element along the Z axis to be copied.
/// @param xpitch is the stride along the X axis. 
/// @param ypitch is the stride along the Y axis. 
/// @param blocking is a flag to make the transfer blocking or not.
/// @return Nothing.
///
void    hicl_mem_push(address_t h, int ix, int ex, 
                      int iy, int ey, int iz, int ez, 
                      int xpitch, int ypitch, cl_bool blocking);

///
/// @brief Copy data from device to host.
///
/// This routine copies data from device to host, that are related to a given
/// hiCL memory descriptor.
/// @param h is the host memory pointer that identifies the descriptor.
/// @param blocking is a flag to make the transfer blocking or not.
/// @return Nothing.
///
void    hicl_mem_dtoh(address_t h, cl_bool blocking);

///
/// @brief Copy data from host to device.
///
/// This routine copies data from host to device, that are related to a given
/// hiCL memory descriptor.
/// @param h is the host memory pointer that identifies the descriptor.
/// @param blocking is a flag to make the transfer blocking or not.
/// @return Nothing.
///
void    hicl_mem_htod(address_t h, cl_bool blocking);

///
/// @brief Print information about a given hiCL memory descriptor.
///
/// This routine shows information about a given memory object, which include
/// the size, the number of references, etc.
/// @param h is the host memory pointer that identifies the descriptor.
/// @return Nothing.
///
void    hicl_mem_info(address_t h);

CPPGUARD_END()

#endif  // HICL_MEM_H_