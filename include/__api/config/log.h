#ifndef __API_CONFIG_LOG_H_
#define __API_CONFIG_LOG_H_

//#define __API_LOG_STD
//#define __API_VERBOSE          
//#define __API_DEBUG           

#ifdef __API_LOG_STD
// Terminal colors (only available if out/err to tty).
#define C_GREEN  "\x1B[32m"
#define C_PURPLE "\x1B[35m"
#define C_YELLOW "\x1B[33m"
#define C_RED    "\x1B[31m"
#define C_END    "\x1B[0m"
#else
#define C_GREEN  ""
#define C_PURPLE ""
#define C_YELLOW ""
#define C_RED    ""
#define C_END    ""
#endif  // __API_LOG_STD

#endif  // __API_CONFIG_LOG_H_
