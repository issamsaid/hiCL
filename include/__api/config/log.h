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
/// modification, are permetted provided that the following conditions
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
#define __API_LOG_STD

///
/// @brief Set the verbose mode of hiCL up.
///
#define __API_VERBOSE          

///
/// @brief Set the debug mode of hiCL up.
///
//#define __API_DEBUG           

#ifdef __API_LOG_STD
///
/// @brief Terminal colors (only available if out/err to tty).
///
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