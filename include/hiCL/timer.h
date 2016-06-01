#ifndef HICL_TIMER_H_
#define HICL_TIMER_H_
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
/// \file timer.h
/// \version $Id: timer.h 2383 2014-05-05 01:09:12Z issam $
/// \brief Defines a set of timing utilities.
///
#include "__api/config/guard.h"

CPPGUARD_BEGIN()

typedef enum time_unit_ {
    NANO_SECONDS  = 1,
    MICRO_SECONDS = 1000,
    MILLI_SECONDS = 1000000,
    SECONDS       = 1000000000
} time_unit;

void   hicl_timer_uset(time_unit unit);

char*  hicl_timer_uget();

void   hicl_timer_tick();

double hicl_timer_read();

double hicl_timer_coef();

CPPGUARD_END()

#endif  // HICL_TIMER_H_
