#ifndef HICL_KNL_H_
#define HICL_KNL_H_
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
/// \file knl.h
/// \version $Id: knl.h 2412 2014-05-15 22:10:52Z issam $
/// \brief Set of functions related to an OpenCL kernel.
///
#include "hiCL/types.h"
#include <stdint.h>

CPPGUARD_BEGIN()

knl    hicl_knl_init(cl_kernel id);

void   hicl_knl_release(knl k);

void   hicl_knl_info(knl k);

knl    hicl_knl_find(const char *name);

void   hicl_knl_build(knl k, const char *options);

void   hicl_knl_set_int32(knl k, int index, int32_t i32);

void   hicl_knl_set_int64(knl k, int index, int64_t i64);

void   hicl_knl_set_float(knl k, int index, float f);

void   hicl_knl_set_double(knl k, int index, double d);

void   hicl_knl_set_mem(knl k, int index, address h);

void   hicl_knl_set_args(knl k, ...);

void   hicl_knl_set_wrk(knl k, cl_uint wrk, size_t *gws, size_t *lws);

void   hicl_knl_set_ofs(knl k, size_t *ofs);

void   hicl_knl_set_and_run(knl k, dev d, ...);

void   hicl_knl_set_and_srun(knl k, dev d, ...);

double hicl_knl_set_and_trun(knl k, dev d, ...);

double hicl_knl_flat_trun(knl k, size_t *gws, 
                            size_t *lws, size_t *ofs, dev d, ...);

void   hicl_knl_run(knl k, dev d);

void   hicl_knl_srun(knl k, dev d);

double hicl_knl_trun(knl k, dev d);


///
/// Same functions, nicer names
///
void   hicl_build(const char *name, const char *options);

void   hicl_set_int32(const char *name, int index, int32_t i32);

void   hicl_set_int64(const char *name, int index, int64_t i64);

void   hicl_set_float(const char *name, int index, float f);

void   hicl_set_double(const char *name, int index, double d);

void   hicl_set_mem(const char *name, int index, address h);

void   hicl_set_args(const char *name, ...);

void   hicl_set_wrk(const char *name, cl_uint wrk, size_t *gws, size_t *lws);

void   hicl_set_ofs(const char *name, size_t *ofs);

void   hicl_set_and_run(const char *name, dev d, ...);

void   hicl_set_and_srun(const char *name, dev d, ...);

double hicl_set_and_trun(const char *name, dev d, ...);

void   hicl_run(const char *name, dev d);

void   hicl_srun(const char *name, dev d);

double hicl_trun(const char *name, dev d);

CPPGUARD_END()

#endif  // HICL_KNL_H_
