
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   poutreSimdAlgorithm.hpp
 * @author Thomas Retornaz
 * @brief  Add STL like algorithm on top of xsimd
 *
 *
 */

#include <poutre/base/details/simd/poutreSimd.hpp>
#include <poutre/base/poutreConfig.hpp>
#include <poutre/base/poutreTypes.hpp>

#include <cstdio>

/**
 * @addtogroup simd_group SIMD facilities
 * @ingroup poutre_base_group
 *@{
 */
namespace poutre::simd {
template<typename T, typename U, typename UnOp>
U *transform(const T *__restrict first, const T *__restrict last, U *__restrict out, UnOp f) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(first, "null ptr");
  POUTRE_ASSERTCHECK(last, "null ptr");
  POUTRE_ASSERTCHECK(out, "null ptr");

  using simd_type_T = typename TypeTraits<T>::simd_type;
  using simd_type_U = typename TypeTraits<U>::simd_type;

  static_assert(simd_type_T::size == simd_type_U::size, "mismatch length between T and U");

  static_assert(sizeof(T) >= alignof(T), "can't generate simd code for this kind of type");

  static_assert(sizeof(U) >= alignof(U), "can't generate simd code for this kind of type");

  const auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;

  // Define loop counter
  const auto simd_size = TypeTraits<T>::simd_loop_step;
  const auto size = std::distance(first, last);
  // note enforce that input is aligned when we start the main simd loop
  const auto range = simd::t_SIMDInputRange(first, last);
  const auto size_prologue_loop = range.first;
  const auto size_simd_loop = range.second;

  auto i = 0;

  //---prologue
  for (; i < size_prologue_loop; ++i) { *out++ = f(*first++); }

  //---main simd loop
  if (simd::IsAligned(out, alignment)) {
    for (; i < size_simd_loop; i += simd_size) {
      auto element = xs::load_aligned(first);
      xs::store_aligned(out, f(element));
      first += simd_size;
      out += simd_size;
    }
  } else {
    for (; i < size_simd_loop; i += simd_size) {
      auto element = xs::load_aligned(first);
      xs::store_unaligned(out, f(element));
      first += simd_size;
      out += simd_size;
    }
  }
  //---epilogue
  for (; i < size; ++i) { *out++ = f(*first++); }
  return out;
}

template<typename T1, typename T2, typename U, typename BinOp>
U *transform(T1 const *__restrict first1,
  T1 const *__restrict last1,
  T2 const *__restrict first2,
  U *__restrict out,
  BinOp f) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(first1, "null ptr");
  POUTRE_ASSERTCHECK(last1, "null ptr");
  POUTRE_ASSERTCHECK(first2, "null ptr");
  POUTRE_ASSERTCHECK(out, "null ptr");

  using simd_type_T1 = typename TypeTraits<T1>::simd_type;
  using simd_type_T2 = typename TypeTraits<T2>::simd_type;
  using simd_type_U = typename TypeTraits<U>::simd_type;

  static_assert(simd_type_T1::size == simd_type_T2::size, "mismatch length between T1 and T2");
  static_assert(simd_type_T1::size == simd_type_U::size, "mismatch length between T1 and U");
  static_assert(simd_type_T2::size == simd_type_U::size, "mismatch length between T2 and U");
  static_assert(sizeof(T1) >= alignof(T1), "can't generate simd code for this kind of type");
  static_assert(sizeof(T2) >= alignof(T2), "can't generate simd code for this kind of type");
  static_assert(sizeof(U) >= alignof(U), "can't generate simd code for this kind of type");
  auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;

  // Define loop counter
  const auto simd_size = TypeTraits<T1>::simd_loop_step;
  const auto size = std::distance(first1, last1);
  const auto range = simd::t_SIMDInputRange(first1, last1);
  const auto size_prologue_loop = range.first;
  const auto size_simd_loop = range.second;

  auto i = 0;

  //---prologue
  for (; i < size_prologue_loop; ++i) { *out++ = f(*first1++, *first2++); }

  //---main simd loop
  if (simd::IsAligned(first2, alignment) && simd::IsAligned(out, alignment)) {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_aligned(first2);
      xs::store_aligned(out, f(element1, element2));
      first1 += simd_size;
      first2 += simd_size;
      out += simd_size;
    }
  } else {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_unaligned(first2);
      xs::store_unaligned(out, f(element1, element2));
      first1 += simd_size;
      first2 += simd_size;
      out += simd_size;
    }
  }
  //---epilogue
  for (; i < size; ++i) { *out++ = f(*first1++, *first2++); }
  return out;
}

template<typename T1, typename T2, typename T3, typename U, typename TerOp>
U *transform(T1 const *__restrict first1,
  T1 const *__restrict last1,
  T2 const *__restrict first2,
  T3 const *__restrict first3,
  U *__restrict out,
  TerOp f) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(first1, "null ptr");
  POUTRE_ASSERTCHECK(last1, "null ptr");
  POUTRE_ASSERTCHECK(first2, "null ptr");
  POUTRE_ASSERTCHECK(first3, "null ptr");
  POUTRE_ASSERTCHECK(out, "null ptr");

  using simd_type_T1 = typename TypeTraits<T1>::simd_type;
  using simd_type_T2 = typename TypeTraits<T2>::simd_type;
  using simd_type_T3 = typename TypeTraits<T3>::simd_type;
  using simd_type_U = typename TypeTraits<U>::simd_type;

  static_assert(simd_type_T1::size == simd_type_T2::size, "mismatch length between T1 and T2");
  static_assert(simd_type_T2::size == simd_type_T3::size, "mismatch length between T2 and T3");
  static_assert(simd_type_T1::size == simd_type_U::size, "mismatch length between T1 and U");
  static_assert(simd_type_T2::size == simd_type_U::size, "mismatch length between T2 and U");
  static_assert(simd_type_T3::size == simd_type_U::size, "mismatch length between T3 and U");
  static_assert(sizeof(T1) >= alignof(T1), "can't generate simd code for this kind of type");
  static_assert(sizeof(T2) >= alignof(T2), "can't generate simd code for this kind of type");
  static_assert(sizeof(T3) >= alignof(T3), "can't generate simd code for this kind of type");
  static_assert(sizeof(U) >= alignof(U), "can't generate simd code for this kind of type");
  auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;

  // Define loop counter
  const auto simd_size = TypeTraits<T1>::simd_loop_step;
  const auto size = std::distance(first1, last1);
  const auto range = simd::t_SIMDInputRange(first1, last1);
  const auto size_prologue_loop = range.first;
  const auto size_simd_loop = range.second;

  auto i = 0;

  //---prologue
  for (; i < size_prologue_loop; ++i) { *out++ = f(*first1++, *first2++, *first3++); }

  //---main simd loop
  if (simd::IsAligned(first2, alignment) && simd::IsAligned(first3, alignment) && simd::IsAligned(out, alignment)) {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_aligned(first2);
      simd_type_T3 element3 = xs::load_aligned(first3);

      xs::store_aligned(out, f(element1, element2, element3));
      first1 += simd_size;
      first2 += simd_size;
      first3 += simd_size;
      out += simd_size;
    }
  } else {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_unaligned(first2);
      simd_type_T3 element3 = xs::load_unaligned(first3);
      xs::store_unaligned(out, f(element1, element2, element3));
      first1 += simd_size;
      first2 += simd_size;
      first3 += simd_size;
      out += simd_size;
    }
  }
  //---epilogue
  for (; i < size; ++i) { *out++ = f(*first1++, *first2++, *first3++); }
  return out;
}

template<typename T1, typename T2, typename T3, typename T4, typename U, typename QuaterOp>
U *transform(T1 const *__restrict first1,
  T1 const *__restrict last1,
  T2 const *__restrict first2,
  T3 const *__restrict first3,
  T4 const *__restrict first4,
  U *__restrict out,
  QuaterOp f) POUTRE_NOEXCEPTONLYNDEBUG
{
  POUTRE_ASSERTCHECK(first1, "null ptr");
  POUTRE_ASSERTCHECK(last1, "null ptr");
  POUTRE_ASSERTCHECK(first2, "null ptr");
  POUTRE_ASSERTCHECK(first3, "null ptr");
  POUTRE_ASSERTCHECK(first4, "null ptr");
  POUTRE_ASSERTCHECK(out, "null ptr");

  using simd_type_T1 = typename TypeTraits<T1>::simd_type;
  using simd_type_T2 = typename TypeTraits<T2>::simd_type;
  using simd_type_T3 = typename TypeTraits<T3>::simd_type;
  using simd_type_T4 = typename TypeTraits<T4>::simd_type;
  using simd_type_U = typename TypeTraits<U>::simd_type;

  static_assert(simd_type_T1::size == simd_type_T2::size, "mismatch length between T1 and T2");
  static_assert(simd_type_T2::size == simd_type_T3::size, "mismatch length between T1 and T3");
  static_assert(simd_type_T4::size == simd_type_T3::size, "mismatch length between T3 and T4");
  static_assert(simd_type_T1::size == simd_type_U::size, "mismatch length between T1 and U");
  static_assert(simd_type_T2::size == simd_type_U::size, "mismatch length between T2 and U");
  static_assert(simd_type_T3::size == simd_type_U::size, "mismatch length between T3 and U");
  static_assert(simd_type_T4::size == simd_type_U::size, "mismatch length between T4 and U");
  static_assert(sizeof(T1) >= alignof(T1), "can't generate simd code for this kind of type");
  static_assert(sizeof(T2) >= alignof(T2), "can't generate simd code for this kind of type");
  static_assert(sizeof(T3) >= alignof(T3), "can't generate simd code for this kind of type");
  static_assert(sizeof(T4) >= alignof(T4), "can't generate simd code for this kind of type");
  static_assert(sizeof(U) >= alignof(U), "can't generate simd code for this kind of type");
  auto alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;

  // Define loop counter
  const auto simd_size = TypeTraits<T1>::simd_loop_step;
  const auto size = std::distance(first1, last1);
  const auto range = simd::t_SIMDInputRange(first1, last1);
  const auto size_prologue_loop = range.first;
  const auto size_simd_loop = range.second;

  auto i = 0;

  //---prologue
  for (; i < size_prologue_loop; ++i) { *out++ = f(*first1++, *first2++, *first3++, *first4++); }

  //---main simd loop
  if (simd::IsAligned(first2, alignment) && simd::IsAligned(first3, alignment) && simd::IsAligned(first4, alignment)
      && simd::IsAligned(out, alignment)) {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_aligned(first2);
      simd_type_T3 element3 = xs::load_aligned(first3);
      simd_type_T4 element4 = xs::load_aligned(first4);

      xs::store_aligned(out, f(element1, element2, element3, element4));
      first1 += simd_size;
      first2 += simd_size;
      first3 += simd_size;
      first4 += simd_size;
      out += simd_size;
    }
  } else {
    for (; i < size_simd_loop; i += simd_size) {
      simd_type_T1 element1 = xs::load_aligned(first1);
      simd_type_T2 element2 = xs::load_unaligned(first2);
      simd_type_T3 element3 = xs::load_unaligned(first3);
      simd_type_T4 element4 = xs::load_unaligned(first4);
      xs::store_unaligned(out, f(element1, element2, element3, element4));
      first1 += simd_size;
      first2 += simd_size;
      first3 += simd_size;
      first4 += simd_size;
      out += simd_size;
    }
  }
  //---epilogue
  for (; i < size; ++i) { *out++ = f(*first1++, *first2++, *first3++, *first4++); }
  return out;
}
}// namespace poutre::simd
 //! @} doxygroup: simd_group
