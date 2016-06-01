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
/// \file hicl_util.c
/// \version $Id$
/// \brief Implements hicl utilities.
///
#include "hiCL/util.h"
#include "__api/util-inl.h"
#include <string.h>

flags_t hicl_str_to_flags_t(const char* str) {
    flags_t flags;
    if (strcmp(str, "HWA") == 0)
        flags = HWA;
    else if (strcmp(str, "PINNED") == 0)
        flags = PINNED;
    else if (strcmp(str, "ZERO_COPY") == 0)
        flags = ZERO_COPY;
    else if (strcmp(str, "READ_ONLY") == 0)
        flags = READ_ONLY;
    else if (strcmp(str, "WRITE_ONLY") == 0)
        flags = WRITE_ONLY;
    else if (strcmp(str, "READ_WRITE") == 0)
        flags = READ_WRITE;
    else
        HICL_EXIT("unknown flags_t");
    return flags;
}
