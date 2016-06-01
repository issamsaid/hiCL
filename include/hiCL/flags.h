#ifndef HICL_FLAGS_H_
#define HICL_FLAGS_H_
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
/// \file flags.h
/// \version $Id: types.h 2408 2014-05-12 14:13:50Z issam $
/// \brief OpenCL definitions used in hicl.
///

#define DEFAULT           0x0000000000000000
#define FIRST             (uint64_t)1<<0
#define SECOND            (uint64_t)1<<1
#define THIRD             (uint64_t)1<<2
#define FOURTH            (uint64_t)1<<3
#define FIFTH             (uint64_t)1<<4
#define SIXTH             (uint64_t)1<<5
#define SEVENTH           (uint64_t)1<<6
#define EIGHTH            (uint64_t)1<<7
// reserved               (uint64_t)1<<8
// reserved               (uint64_t)1<<9
// reserved               (uint64_t)1<<10
// reserved               (uint64_t)1<<11
#define CPU               (uint64_t)1<<12
#define GPU               (uint64_t)1<<13
#define ACCELERATOR       (uint64_t)1<<14
#define ALL               (uint64_t)1<<15
#define AMD               (uint64_t)1<<16
#define APPLE             (uint64_t)1<<17
#define INTEL             (uint64_t)1<<18
#define NVIDIA            (uint64_t)1<<19
// reserved               (uint64_t)1<<20
// reserved               (uint64_t)1<<21
// reserved               (uint64_t)1<<22
// reserved               (uint64_t)1<<23
#define HOST_DIRTY        (uint64_t)1<<24
#define DEVICE_DIRTY      (uint64_t)1<<25
// reserved               (uint64_t)1<<26
#define HWA               (uint64_t)1<<27
#define PINNED            (uint64_t)1<<28
#define ZERO_COPY         (uint64_t)1<<29
#define HOST_ALLOCATED    (uint64_t)1<<30
#define READ_ONLY         (uint64_t)1<<31
#define WRITE_ONLY        (uint64_t)1<<32
#define READ_WRITE        (uint64_t)1<<33
#define CHAR              (uint64_t)1<<34
#define INT               (uint64_t)1<<35
#define UNSIGNED_INT      (uint64_t)1<<36
#define LONG              (uint64_t)1<<37
#define UNSIGNED_LONG     (uint64_t)1<<38
#define SIZET             (uint64_t)1<<39
#define FLOAT             (uint64_t)1<<40
#define DOUBLE            (uint64_t)1<<41

#define __API_FLAGS_HAVE(flags, mask) \
	(mask == DEFAULT ? (flags == DEFAULT):((flags & (mask)) == mask))

#endif  // HICL_FLAGS_H_