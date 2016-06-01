#ifndef HICL_DEV_H_
#define HICL_DEV_H_
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
/// \file dev.h
/// \version $Id: dev.h 2383 2014-05-05 01:09:12Z issam $
/// \brief Defines a descriptor of an OpenCL device.
///
#include "hiCL/types.h"

CPPGUARD_BEGIN()

dev  hicl_dev_init(cl_device_id id);

void hicl_dev_release(dev d);

bool hicl_dev_support(dev d, char* extension);

void hicl_dev_info(dev d);

dev  hicl_dev_find(flags_t flags);

void hicl_dev_wait(dev d);

CPPGUARD_END()

#endif  // HICL_DEV_H_
