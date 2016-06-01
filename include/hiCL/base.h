#ifndef HICL_CORE_H_
#define HICL_CORE_H_
#include "hiCL/types.h"
#include "__api/config/guard.h"

CPPGUARD_BEGIN()

/// Init the hiCL library
void hicl_init(flags_t flags);

/// OpenCL release function.
void hicl_release(void);

/// OpenCL code loader.
void hicl_load(const char *filename, const char *options);

/// OpenCL info function.
void hicl_info(void);

CPPGUARD_END()

#endif  // HICL_CORE_H_
