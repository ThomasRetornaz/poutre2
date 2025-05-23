//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   types_traits.hpp
 * @author Thomas Retornaz
 * @brief  Define type traits for common types
 *
 *
 */

#include "types.hpp"
#include <poutre/base/config.hpp>
#include <poutre/base/details/simd/simd_helpers.hpp>
#include <poutre/base/types.hpp>

#include <limits>
#include <stdexcept>
#include <type_traits>
#include <xsimd/config/xsimd_arch.hpp>

namespace poutre {


//! Define converter from enum to type
template<CompoundType compound, PType ptype> struct enum_to_type
{
};

template<> struct enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_GrayUINT8>
{
  using type = pUINT8;
};

template<> struct enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_GrayINT32>
{
  using type = pINT32;
};

template<> struct enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_GrayINT64>
{
  using type = pINT64;
};

template<> struct enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_F32>
{
  using type = pFLOAT;
};

template<> struct enum_to_type<CompoundType::CompoundType_Scalar, PType::PType_D64>
{
  using type = pDOUBLE;
};

template<> struct enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_GrayUINT8>
{
  using type = compound_type<pUINT8, 3>;
};

template<> struct enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_GrayINT32>
{
  using type = compound_type<pINT32, 3>;
};

template<> struct enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_GrayINT64>
{
  using type = compound_type<pINT64, 3>;
};

template<> struct enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_F32>
{
  using type = compound_type<pFLOAT, 3>;
};

template<> struct enum_to_type<CompoundType::CompoundType_3Planes, PType::PType_D64>
{
  using type = compound_type<pDOUBLE, 3>;
};

template<> struct enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_GrayUINT8>
{
  using type = compound_type<pUINT8, 4>;
};

template<> struct enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_GrayINT32>
{
  using type = compound_type<pINT32, 4>;
};

template<> struct enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_GrayINT64>
{
  using type = compound_type<pINT64, 4>;
};

template<> struct enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_F32>
{
  using type = compound_type<pFLOAT, 4>;
};

template<> struct enum_to_type<CompoundType::CompoundType_4Planes, PType::PType_D64>
{
  using type = compound_type<pDOUBLE, 4>;
};
//! Define TypeTraits
template<class valuetype> struct TypeTraits
{
};

//! TypeTraits pUINT8
template<> struct TypeTraits<pUINT8>
{
  using storage_type = pUINT8;
  using safe_signed_type = pINT32;
  using str_type = pUINT32;
  using accu_type = pINT64;
  POUTRE_STATIC_CONSTEXPR auto p_type = PType::PType_GrayUINT8;
  POUTRE_STATIC_CONSTEXPR auto c_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = xsimd::batch<storage_type, xsimd::default_arch>::size;
  using simd_type = typename xs::batch<storage_type>;
  using simd_mask_type = typename xs::batch_bool<storage_type>;
  POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(storage_type) * 8;

  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::lowest();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::min();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::max();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf() { return min(); }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup() { return max(); }
};

template<> struct TypeTraits<const pUINT8> : public TypeTraits<pUINT8>
{
};

//! TypeTraits pINT32
template<> struct TypeTraits<pINT32>
{
  using storage_type = pINT32;
  using safe_signed_type = pINT64;
  using str_type = pINT32;
  using accu_type = pINT64;

  static constexpr auto p_type = PType::PType_GrayINT32;
  static constexpr auto c_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step =  xsimd::batch<storage_type, xsimd::default_arch>::size;
  using simd_type = typename xs::batch<storage_type>;
  using simd_mask_type = typename xs::batch_bool<storage_type>;
  POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(storage_type) * 8;

  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::lowest();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::min();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::max();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf() { return min(); }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup() { return max(); }
};

template<> struct TypeTraits<const pINT32> : public TypeTraits<pINT32>
{
};

//! TypeTraits pFLOAT
template<> struct TypeTraits<pFLOAT>
{
  using storage_type = pFLOAT;
  using safe_signed_type = pFLOAT;
  using str_type = pFLOAT;
  using accu_type = pDOUBLE;

  static constexpr auto p_type = PType::PType_F32;
  static constexpr auto c_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = xsimd::batch<storage_type, xsimd::default_arch>::size;
  using simd_type = typename xs::batch<storage_type>;
  using simd_mask_type = typename xs::batch_bool<storage_type>;

  POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(pFLOAT) * 8;

  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::lowest();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::min();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::max();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf() { return min(); }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup() { return max(); }
};

template<> struct TypeTraits<const pFLOAT> : public TypeTraits<pFLOAT>
{
};

//! TypeTraits pDOUBLE
template<> struct TypeTraits<pDOUBLE>
{
  using storage_type = pDOUBLE;
  using safe_signed_type = pDOUBLE;
  using str_type = pDOUBLE;
  using accu_type = pDOUBLE;

  static constexpr auto p_type = PType::PType_D64;
  static constexpr auto c_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = xsimd::batch<storage_type, xsimd::default_arch>::size;
  using simd_type = typename xs::batch<storage_type>;
  using simd_mask_type = typename xs::batch_bool<storage_type>;

  POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(pDOUBLE) * 8;

  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::lowest();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::min();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::max();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf() { return min(); }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup() { return max(); }
};

template<> struct TypeTraits<const pDOUBLE> : public TypeTraits<pDOUBLE>
{
};

//! TypeTraits pINT64
template<> struct TypeTraits<pINT64>
{
  using storage_type = pINT64;
  using safe_signed_type = pINT64;
  using str_type = pINT64;
  using accu_type = pINT64;

  static constexpr auto p_type = PType::PType_GrayINT64;
  static constexpr auto c_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = xsimd::batch<storage_type, xsimd::default_arch>::size;
  POUTRE_STATIC_CONSTEXPR size_t quant = sizeof(pINT64) * 8;
  using simd_type = typename xs::batch<storage_type>;
  using simd_mask_type = typename xs::batch_bool<storage_type>;

  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::lowest();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::min();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
  {
    return std::numeric_limits<storage_type>::max();
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf() { return min(); }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup() { return max(); }
};

template<> struct TypeTraits<const pINT64> : public TypeTraits<pINT64>
{
};

//! TypeTraits compound
template<class value_type, std::size_t Rank> struct TypeTraits<compound_type<value_type, Rank>>
{
};

template<class value_type> struct TypeTraits<compound_type<value_type, 3>>
{
  using storage_type = compound_type<value_type, 3>;
  using safe_signed_type = compound_type<typename TypeTraits<value_type>::safe_signed_type, 3>;
  using str_type = compound_type<typename TypeTraits<value_type>::str_type, 3>;
  using accu_type = compound_type<typename TypeTraits<value_type>::accu_type, 3>;

  static constexpr auto p_type = TypeTraits<value_type>::p_type;
  static constexpr auto c_type = CompoundType::CompoundType_3Planes;

  POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

  // todo decltype
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
  {
    return poutre::compound_type<value_type, 3>(std::numeric_limits<value_type>::lowest(),
      std::numeric_limits<value_type>::lowest(),
      std::numeric_limits<value_type>::lowest());
  }

  // todo decltype
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
  {
    return poutre::compound_type<value_type, 3>(std::numeric_limits<value_type>::min(),
      std::numeric_limits<value_type>::min(),
      std::numeric_limits<value_type>::min());
  }

  // todo decltype
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
  {
    return poutre::compound_type<value_type, 3>(std::numeric_limits<value_type>::max(),
      std::numeric_limits<value_type>::max(),
      std::numeric_limits<value_type>::max());
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf() { return min(); }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup() { return max(); }
};

template<class value_type> struct TypeTraits<compound_type<value_type, 4>>
{
  using storage_type = compound_type<value_type, 4>;
  using safe_signed_type = compound_type<typename TypeTraits<value_type>::safe_signed_type, 4>;
  using str_type = compound_type<typename TypeTraits<value_type>::str_type, 4>;
  using accu_type = compound_type<typename TypeTraits<value_type>::accu_type, 4>;

  static constexpr auto p_type = TypeTraits<value_type>::p_type;
  static constexpr auto c_type = CompoundType::CompoundType_4Planes;

  POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

  // todo decltype
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
  {
    return poutre::compound_type<value_type, 4>(std::numeric_limits<value_type>::lowest(),
      std::numeric_limits<value_type>::lowest(),
      std::numeric_limits<value_type>::lowest(),std::numeric_limits<value_type>::lowest());
  }

  // todo decltype
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
  {
    return poutre::compound_type<value_type, 4>(std::numeric_limits<value_type>::min(),
      std::numeric_limits<value_type>::min(),
      std::numeric_limits<value_type>::min(),std::numeric_limits<value_type>::min());
  }

  // todo decltype
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
  {
    return poutre::compound_type<value_type, 4>(std::numeric_limits<value_type>::max(),
      std::numeric_limits<value_type>::max(),
      std::numeric_limits<value_type>::max(),std::numeric_limits<value_type>::max());
  }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf() { return min(); }
  POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup() { return max(); }
};
}// namespace poutre