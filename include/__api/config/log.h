#ifndef __API_CONFIG_LOG_H_
#define __API_CONFIG_LOG_H_
///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
/// All rights reserved.
///
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
/// funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions
/// are met:
///
/// 1. Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright
///    notice, this list of conditions and the following disclaimer in the
///    documentation and/or other materials provided with the distribution.
/// 3. Neither the name of the UPMC nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
/// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
/// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
/// LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// @file config/log.h
/// @author Issam SAID
/// @brief Configuration file for logging in hiCL.
///

///
/// @brief Use tty as the output for logging, otherwise use files.
///
#ifdef __API_LOG_STD
#endif // __API_LOG_STD

///
/// @brief Set the verbose mode of hiCL up.
///
#ifdef __API_VERBOSE
#endif // __API_VERBOSE          

///
/// @brief Set the debug mode of hiCL up.
///
#ifdef __API_DEBUG

#ifndef __API_VERBOSE
#define __API_VERBOSE
#endif // __API_VERBOSE         

#ifndef __API_LOG_STD
#define __API_LOG_STD
#endif // __API_LOG_STD 

#endif // __API_DEBUG           

///
/// @brief Terminal colors (only available if out/err to tty).
///
#ifdef __API_LOG_STD
#define HICL_GREEN  "\x1B[32m"
#define HICL_PURPLE "\x1B[35m"
#define HICL_YELLOW "\x1B[33m"
#define HICL_RED    "\x1B[31m"
#define HICL_END    "\x1B[0m"
#else
#define HICL_GREEN  ""
#define HICL_PURPLE ""
#define HICL_YELLOW ""
#define HICL_RED    ""
#define HICL_END    ""
#endif  // __API_LOG_STD

#endif  // __API_CONFIG_LOG_H_