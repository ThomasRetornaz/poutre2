
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include <format>

/**
 * @file   config.hpp
 * @author Thomas Retornaz
 * @brief  Various macro and alias
 *
 *
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* clang-format off */
  /* ------------------------------------------------------------------------- */
  /* Compiler detection (order matters https://stackoverflow.com/a/28166605)   */

#  if defined(_MSC_VER)
#    define POUTRE_IS_MSVC
#  elif defined(__HIPCC__)
#    define POUTRE_IS_HIPCC
#  elif defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#    define POUTRE_IS_DPCPP
#  elif defined(__NVCC__) || defined(__CUDACC__)
#    define POUTRE_IS_NVCC
#  elif defined(__INTEL_COMPILER)
#    define POUTRE_IS_ICC
#  elif defined(__clang__)
#    define POUTRE_IS_CLANG
#  elif defined(__GNUC__) || defined(__GNUG__)
#    define POUTRE_IS_GCC
#  endif

/* ------------------------------------------------------------------------- */
  /* C++ standard detection */

#  ifdef POUTRE_IS_MSVC
#    define POUTRE__cplusplus _MSVC_LANG
#  else
#    ifdef __cplusplus
#      define POUTRE__cplusplus __cplusplus
#    else
#      define POUTRE__cplusplus 0
#    endif
#  endif

#  if POUTRE__cplusplus > 0 && POUTRE__cplusplus < 201103L
#    define POUTRE_CXX 1998
#  elif POUTRE__cplusplus >= 201103L && POUTRE__cplusplus < 201402L
#    define POUTRE_CXX 2011
#  elif POUTRE__cplusplus >= 201402L && POUTRE__cplusplus < 201703L
#    define POUTRE_CXX 2014
#  elif POUTRE__cplusplus == 201703L
#    define POUTRE_CXX 2017
#  elif POUTRE__cplusplus >= 201704L
#    define POUTRE_CXX 2020
#  else
#    define POUTRE_CXX 0
#  endif

  /* ------------------------------------------------------------------------- */
/* Architecture detection */

#if defined(i386) || defined(__i386__) || defined(__i486__) ||                \
    defined(__i586__) || defined(__i686__) || defined(__i386) ||              \
    defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) ||           \
    defined(__I86__) || defined(__INTEL__) || defined(__x86_64) ||            \
    defined(__x86_64__) || defined(__amd64__) || defined(__amd64) ||          \
    defined(_M_X64)
  #define POUTRE_X86
#elif defined(__arm__) || defined(__arm64) || defined(__thumb__) ||           \
    defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) ||             \
    defined(_M_ARM) || defined(_M_ARM64) || defined(__arch64__)
  #define POUTRE_ARM
#elif defined(__ppc__) || defined(__powerpc__) || defined(__PPC__)
  #define POUTRE_POWERPC
#else
  #define POUTRE_CPU
#endif

#  if POUTRE_CXX >= 2020
#    include <concepts>
#  endif

#  define POUTRE_NOEXCEPT noexcept // NOLINT

#  define POUTRE_NOEXCEPT_IF(Predicate) noexcept((Predicate)) // NOLINT

#  define POUTRE_NOEXCEPT_EXPR(Predicate) noexcept((Predicate)) // NOLINT

#  ifndef NDEBUG
#    define POUTRE_NOEXCEPTONLYNDEBUG // NOLINT
#  else
#    define POUTRE_NOEXCEPTONLYNDEBUG POUTRE_NOEXCEPT // NOLINT
#  endif

#  define POUTRE_STATIC_CONSTEXPR static constexpr // NOLINT

#  define POUTRE_CONSTEXPR constexpr // NOLINT

#  define POUTRE_CONSTEXPR constexpr // NOLINT

// POUTRE_STRONG_INLINE is a stronger version of inline directive mostly to enforce inlining for
// MSVC compiler:using
// __forceinline on MSVC/INTEL,but still doesn't use GCC's/clang always_inline.
#  if( defined _MSC_VER ) || (defined(__ICC) || defined(__INTEL_COMPILER))
#    define POUTRE_STRONG_INLINE __forceinline // NOLINT
#  else
#    define POUTRE_STRONG_INLINE inline // NOLINT
#  endif

// POUTRE_ALWAYS_INLINE is the strongest version of inline directive, using __forceinline on
// MSVC/INTEL,and always inline for GCC/clang.
#  if defined(_MSC_VER)
#    define POUTRE_ALWAYS_INLINE __forceinline // NOLINT
#  elif defined(__GNUC__) && __GNUC__ > 3
  // Clang also defines __GNUC__ (as 4)
#    define POUTRE_ALWAYS_INLINE inline __attribute__((__always_inline__)) // NOLINT
#  else
#    define POUTRE_ALWAYS_INLINE inline // NOLINT
#  endif

//   NOINLINE
#if defined(__GNUC__)
#define POUTRE_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define POUTRE_NOINLINE __declspec(noinline)
#else
#define POUTRE_NOINLINE
#endif

#define POUTRE_MAYBE_UNUSED [[maybe_unused]]

#  if defined(_WIN32) || defined(_WIN64)
#    define poutreDeprecated       __declspec(deprecated) // NOLINT
#    define poutreDeprecatedMSG(x) __declspec(deprecated(x)) // NOLINT

#    if defined(_M_X64) || defined(_WIN64)
#      define POUTRE_64BITS
#    endif

#  else
#    define poutreDeprecated       __attribute__((deprecated))
#    define poutreDeprecatedMSG(x) poutreDeprecated

#    if defined(__LP64__)
#      define POUTRE_64BITS
#    endif

#  endif

  // Generic helper definitions for shared library support
#  if defined _WIN32 || defined __CYGWIN__
#    define MODULE_IMPORT __declspec(dllimport) // NOLINT
#    define MODULE_EXPORT __declspec(dllexport) // NOLINT
#    define MODULE_LOCAL
#  else
#    if __GNUC__ >= 4
#      define MODULE_IMPORT __attribute__((visibility("default")))
#      define MODULE_EXPORT __attribute__((visibility("default")))
#      define MODULE_LOCAL  __attribute__((visibility("hidden")))
#    else
#      define MODULE_IMPORT
#      define MODULE_EXPORT
#      define MODULE_LOCAL
#    endif
#  endif

#  define PROJECT_NAME__ Poutre // NOLINT

  // plateform specific section
#  if defined(_WIN32) || defined(_WIN64)

#    ifndef _CPPRTTI
#      error You should activate the RTTI for PROJECT_NAME__ to make it run correctly
#    endif

#    ifndef _CPPUNWIND
#      error You should activate the exception handling for PROJECT_NAME__ to make it run correcly
#    endif

#  endif

namespace poutre
{

// NOLINTNEXTLINE
#  define POUTRE_RUNTIME_ERROR(MSG)                                                                                         \
    throw std::runtime_error(std::format("Throw {} at {} {}", MSG, __FILE__, __LINE__)) //-V1003 //NOLINT
// NOLINTNEXTLINE
#  define POUTRE_NEVER_REACH_HERE                                                                                           \
    POUTRE_RUNTIME_ERROR("Code should never reach here, WTF Oo") // NOLINT
                                                                 // NOLINTNEXTLINE
#  define POUTRE_CHECK(CONDITION, MSG)                                                                                      \
    {                                                                                                                       \
      if( !(CONDITION) )                                                                                                    \
      {                                                                                                                     \
        POUTRE_RUNTIME_ERROR(MSG);                                                                                          \
      }                                                                                                                     \
    }

#  ifdef _DEBUG
#    if defined(_MSC_VER)
#      define POUTRE_ASSERTCHECK(CONDITION, MSG)                                                                            \
        {                                                                                                                   \
          if( !(CONDITION) )                                                                                                \
          {                                                                                                                 \
            __debugbreak();                                                                                                 \
          }                                                                                                                 \
        }
#    else
#      define POUTRE_ASSERTCHECK(CONDITION, MSG)                                                                            \
        {                                                                                                                   \
          if( !(CONDITION) )                                                                                                \
          {                                                                                                                 \
            __builtin_trap();                                                                                               \
          }                                                                                                                 \
        }
#    endif
#  else
#    define POUTRE_ASSERTCHECK(CONDITION, MSG)
#  endif


  using f32 = float;
  using f64 = double;

  #ifdef POUTRE_IS_MSVC
  using u8  = unsigned __int8;
  using i8  = signed   __int8;
  using u16 = unsigned __int16;
  using i16 = signed   __int16;
  using u32 = unsigned __int32;
  using i32 = signed   __int32;
  using u64 = unsigned __int64;
  using i64 = signed   __int64;
#else
  using u8  = unsigned char;
  using i8  = signed   char;
  using u16 = unsigned short;
  using i16 = signed   short;
  using u32 = unsigned int;
  using i32 = signed   int;
  #if POUTRE_WORD_SIZE == 64
    using u64 = unsigned long;
    using i64 = signed long;
  #else
    #if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
      __extension__ typedef __signed__ long long i64;
      __extension__ typedef unsigned long long u64;
    #else
      typedef __signed__ long long i64;
      typedef unsigned long long u64;
    #endif
  #endif
#endif


#ifdef POUTRE_NATIVE_FP16
  using f16 = __fp16;
#elif defined(POUTRE_CUDA_COMPILING_FOR_DEVICE) || \
      defined(POUTRE_ROCM_COMPILING_FOR_DEVICE)
  using f16 = __half;
#elif defined(POUTRE_ONEAPI_COMPILING_FOR_DEVICE)
  using f16 = half;
#else
  using f16 = struct { u16 u; };
#endif


#if POUTRE_CXX >= 2020
  template <typename T> concept base_value_type_c =
      std::is_same_v<std::remove_const_t<T>, u8> || std::is_same_v<std::remove_const_t<T>, i8> ||
      std::is_same_v<std::remove_const_t<T>, u16> || std::is_same_v<std::remove_const_t<T>, i16> ||
      std::is_same_v<std::remove_const_t<T>, u32> || std::is_same_v<std::remove_const_t<T>, i32> ||
      std::is_same_v<std::remove_const_t<T>, u64> || std::is_same_v<std::remove_const_t<T>, i64> ||
      std::is_same_v<std::remove_const_t<T>, f16> || std::is_same_v<std::remove_const_t<T>, f32> ||
      std::is_same_v<std::remove_const_t<T>, f64>;
  #define POUTRE_CONCEPT_BASE_VALUE_TYPE poutre::base_value_type_c

  #  define POUTRE_REQUIRES(cond) requires(cond)
  #else
#  define POUTRE_CONCEPT_VALUE_TYPE         typename
#  define POUTRE_REQUIRES(cond)
#endif
  }
/* clang-format on */
#endif// DOXYGEN_SHOULD_SKIP_THIS
