#ifndef HICL_UTIL_H_
#define HICL_UTIL_H_
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
/// along with \b hiCL.  If not, see <http://www.gnu.org/licenses/>.
///
/// \author Issam Said
/// \file hicl_mem.h
/// \version $Id: mem.h 2383 2014-05-05 01:09:12Z issam $
/// \brief Defines an OpenCL memory object descriptor
///
/// hiCL utilities.
///
#include "hiCL/flags.h"
#include "hiCL/types.h"

CPPGUARD_BEGIN()

flags_t hicl_str_to_flags_t(const char *str);

CPPGUARD_END()

#endif  // HICL_UTIL_H_
