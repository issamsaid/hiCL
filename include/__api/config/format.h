#ifndef CODE_OMNICL_TRUNK_INCLUDE_OMNICL_CONFIG_FORMAT_H_
#define CODE_OMNICL_TRUNK_INCLUDE_OMNICL_CONFIG_FORMAT_H_
///
/// \copyright Copyright 2012-2013 TOTAL S.A. All rights reserved.
/// This file is part of \b omnicl.
///
/// \b omnicl is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// \b omnicl is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with \b omnicl.  If not, see <http://www.gnu.org/licenses/>.
///
/// \author Issam Said
/// \file format.h
/// \version $Id: format.h 2383 2014-05-05 01:09:12Z issam $
/// \brief Portable formatting config file
///

/// \brief Portable formatters
#if defined(__APPLE__) || defined(__MACOSX)
#define FHEX   "%#016llx"
#define FULONG "%llu"
#define FSIZET "%zu"
#else
#define FHEX   "%#016jx"
#define FULONG "%lu"
#define FSIZET "%zu"
#endif  // defined(__APPLE__) || defined(__MACOSX)

#endif  // CODE_OMNICL_TRUNK_INCLUDE_OMNICL_CONFIG_FORMAT_H_
