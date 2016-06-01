#ifndef __API_CONFIG_GUARD_H_
#define __API_CONFIG_GUARD_H_

#ifdef __cplusplus     
#define CPPGUARD_BEGIN() extern "C" {
#define CPPGUARD_END()   }
#else
#define CPPGUARD_BEGIN()
#define CPPGUARD_END()
#endif

#endif  // __API_CONFIG_GUARD_H_
