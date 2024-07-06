#ifndef BNIK_CONFIG_HPP
#define BNIK_CONFIG_HPP

#if defined(_MSC_VER) && !defined(__clang__)
# ifdef _CPPUNWIND
#  define BNIK_EXCEPTIONS_ENABLED 1
# else
#  define BNIK_EXCEPTIONS_ENABLED 0
# endif
#elif defined(__clang__)
# if __has_feature(cxx_exceptions)
#  define BNIK_EXCEPTIONS_ENABLED 1
# else
#  define BNIK_EXCEPTIONS_ENABLED 0
# endif
#else
# ifdef __EXCEPTIONS
#  define BNIK_EXCEPTIONS_ENABLED 1
# else
#  define BNIK_EXCEPTIONS_ENABLED 0
# endif
#endif

#ifdef BNIK_CXX_MODULES
#define BNIK_CXXM_EXPORT export
#else
#define BNIK_CXXM_EXPORT
#endif

/*
 * Adjust [[no_unique_address]] attribute for Microsoft VC++
*/

#if defined(__GNU__)
# error "This uses gcc" // for testing
# define BNIK_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif defined(_MSC_VER)
# define BNIK_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif defined(__clang__)
//# error "This uses non-MSVC clang" // for testing
# define BNIK_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
//# error "This uses a different compiler" // for testing
# define BNIK_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif // defined(__GNU)

#include <version>

#if defined(__cpp_lib_modules) || (defined(BNIK_HAS_CXX_MODULE_STD) && BNIK_HAS_CXX_MODULE_STD)
# define BNIK_CAN_USE_STD_MODULES
# if __cplusplus >= 202302L || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L)
#  if !defined(BNIK_USE_STD_MODULES)
#   define BNIK_USE_STD_MODULES 1
#  endif
# else // __cplusplus < 202302L
#  if !defined(BNIK_USE_STD_MODULES)
#   define BNIK_USE_STD_MODULES 0
#  else
#   if BNIK_USE_STD_MODULES && ((defined(_MSVC_LANG) && _MSVC_LANG < 202002L) || (!defined(_MSVC_LANG) && __cplusplus < 202002L))
#    error "Compiler definition BNIK_USE_STD_MODULES set to non-zero but using earlier unsupported C++ standard (< C++20)"
#   endif
#  endif
# endif // __cplusplus >= 202302L
#endif


#if defined(__cpp_lib_allocate_at_least) && BNIK_HAS_STD_ALLOCATE_AT_LEAST
# define BNIK_CAN_USE_STD_ALLOCATE_AT_LEAST 1
#else
# define BNIK_CAN_USE_STD_ALLOCATE_AT_LEAST 0
#endif  // defined(__cpp_lib_allocate_at_least)

#if BNIK_CXX23_CONSTEXPR_AVAILABLE
# define BNIK_CXX23_CONSTEXPR constexpr
#else
# define BNIK_CXX23_CONSTEXPR
#endif

#endif // BNIK_CONFIG_HPP
