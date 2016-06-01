#ifndef __API_TSK_INL_H_
#define __API_TSK_INL_H_
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
/// \file tsk-inl.h
/// \version $Id: tsk-inl.h 2412 2014-05-15 22:10:52Z issam $
/// \brief Defines the private routines for an OpenCL task.
///
#include "__api/knl-inl.h"

CPPGUARD_BEGIN()

PRIVATE void
__api_tsk_sync_run(cl_kernel kernel, cl_command_queue queue) {
    HICL_CHECK(clEnqueueTask(queue, kernel, 0, NULL, NULL),
             "failed to run OpenCL task");
    clFinish(queue);
}

PRIVATE void
__api_tsk_async_run(cl_kernel kernel, cl_command_queue queue) {
    HICL_CHECK(clEnqueueTask(queue, kernel, 0, NULL, NULL),
             "failed to run OpenCL task");
}

CPPGUARD_END()

#endif  // __API_TSK_INL_H_
