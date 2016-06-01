#ifndef HICL_MEM_H_
#define HICL_MEM_H_
///
/// \copyright Copyright 2012-2013 TOTAL S.A. All rights reserved.
/// This file is part of \b hicl.
///
/// \b hicl is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// \b hicl is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with \b hicl.  If not, see <http://www.gnu.org/licenses/>.
///
/// \author Issam Said
/// \file hicl_mem.h
/// \version $Id: mem.h 2383 2014-05-05 01:09:12Z issam $
/// \brief Defines an OpenCL memory object descriptor
///
/// Defines a class that describes an hicl memory.
///
#include "hiCL/types.h"

CPPGUARD_BEGIN()

mem  hicl_mem_wrap(dev d, address h, size_t size, flags_t flags);

void hicl_mem_release(address h);

void hicl_mem_update(address h, flags_t flags);

void hicl_mem_pop(address h, int ix, int ex, 
                    int iy, int ey, int iz, int ez, 
                    int xpitch, int ypitch, cl_bool blocking);

void hicl_mem_push(address h, int ix, int ex, 
                    int iy, int ey, int iz, int ez, 
                    int xpitch, int ypitch, cl_bool blocking);

void hicl_mem_dtoh(address h, cl_bool blocking);

void hicl_mem_htod(address h, cl_bool blocking);

void hicl_mem_info(address h);

CPPGUARD_END()

#endif  // HICL_MEM_H_