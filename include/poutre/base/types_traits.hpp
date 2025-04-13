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

#include <poutre/base/details/simd/simd_helpers.hpp>
#include <poutre/base/config.hpp>
#include <poutre/base/types.hpp>

#include <limits>
#include <stdexcept>
#include <type_traits>

 namespace poutre
 {
//! Define TypeTraits
template<class valuetype> struct TypeTraits
{
};

//! TypeTraits pUINT8
template<> struct TypeTraits<pUINT8>
{
  using storage_type                      = pUINT8;
  using safe_signed_type                  = pINT32;
  using str_type                          = pUINT32;
  using accu_type                         = pINT64;
  static const PType        pixel_type    = PType::PType_GrayUINT8;
  static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment      = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_INT8_SIZE;
  using simd_type                               = typename xs::batch<storage_type>;
  using simd_mask_type                          = typename xs::batch_bool<storage_type>;
  POUTRE_STATIC_CONSTEXPR size_t quant          = sizeof(storage_type) * 8;

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
  using storage_type     = pINT32;
  using safe_signed_type = pINT64;
  using str_type         = pINT32;
  using accu_type        = pINT64;

  static const PType        pixel_type    = PType::PType_GrayINT32;
  static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment      = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_INT32_SIZE;
  using simd_type                               = typename xs::batch<storage_type>;
  using simd_mask_type                          = typename xs::batch_bool<storage_type>;
  POUTRE_STATIC_CONSTEXPR size_t quant          = sizeof(storage_type) * 8;

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
  using storage_type     = pFLOAT;
  using safe_signed_type = pFLOAT;
  using str_type         = pFLOAT;
  using accu_type        = pDOUBLE;

  static const PType        pixel_type    = PType::PType_F32;
  static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment      = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_FLOAT_SIZE;
  using simd_type                               = typename xs::batch<storage_type>;
  using simd_mask_type                          = typename xs::batch_bool<storage_type>;

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
  using storage_type     = pDOUBLE;
  using safe_signed_type = pDOUBLE;
  using str_type         = pDOUBLE;
  using accu_type        = pDOUBLE;

  static const PType        pixel_type    = PType::PType_D64;
  static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment      = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_DOUBLE_SIZE;
  using simd_type                               = typename xs::batch<storage_type>;
  using simd_mask_type                          = typename xs::batch_bool<storage_type>;

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
  using storage_type     = pINT64;
  using safe_signed_type = pINT64;
  using str_type         = pINT64;
  using accu_type        = pINT64;

  static const PType        pixel_type    = PType::PType_GrayINT64;
  static const CompoundType compound_type = CompoundType::CompoundType_Scalar;

  POUTRE_STATIC_CONSTEXPR size_t alignment      = SIMD_IDEAL_MAX_ALIGN_BYTES;
  POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = SIMD_BATCH_INT64_SIZE;
  POUTRE_STATIC_CONSTEXPR size_t quant          = sizeof(pINT64) * 8;
  using simd_type                               = typename xs::batch<storage_type>;
  using simd_mask_type                          = typename xs::batch_bool<storage_type>;

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
template<class valuetype, std::ptrdiff_t Rank> struct TypeTraits<compound_type<valuetype, Rank>>
{
};

  template <class valuetype> struct TypeTraits<compound_type<valuetype, 3>>
  {
      using storage_type = compound_type<valuetype, 3>;
      using safe_signed_type = compound_type<typename TypeTraits<valuetype>::safe_signed_type, 3>;
      using str_type = compound_type<typename TypeTraits<valuetype>::str_type, 3>;
      using accu_type = compound_type<typename TypeTraits<valuetype>::accu_type, 3>;

      static const PType pixel_type = TypeTraits<valuetype>::pixel_type;
      static const CompoundType compound_type = CompoundType::CompoundType_3Planes;

      POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
      POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

      // todo decltype
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
      {
          return poutre::compound_type<valuetype, 3>(std::numeric_limits<valuetype>::lowest(),
                                              std::numeric_limits<valuetype>::lowest(),
                                              std::numeric_limits<valuetype>::lowest());
      }

      // todo decltype
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
      {
          return poutre::compound_type<valuetype, 3>(std::numeric_limits<valuetype>::min(),
                                              std::numeric_limits<valuetype>::min(),
                                              std::numeric_limits<valuetype>::min());
      }

      // todo decltype
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
      {
          return poutre::compound_type<valuetype, 3>(std::numeric_limits<valuetype>::max(),
                                              std::numeric_limits<valuetype>::max(),
                                              std::numeric_limits<valuetype>::max());
      }
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
      {
          return min();
      }
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
      {
          return max();
      }
  };

  template <class valuetype> struct TypeTraits<compound_type<valuetype, 4>>
  {
      using storage_type = compound_type<valuetype, 4>;
      using safe_signed_type = compound_type<typename TypeTraits<valuetype>::safe_signed_type, 4>;
      using str_type = compound_type<typename TypeTraits<valuetype>::str_type, 4>;
      using accu_type = compound_type<typename TypeTraits<valuetype>::accu_type, 4>;

      static const PType pixel_type = TypeTraits<valuetype>::pixel_type;
      static const CompoundType compound_type = CompoundType::CompoundType_4Planes;

      POUTRE_STATIC_CONSTEXPR size_t alignment = 1;
      POUTRE_STATIC_CONSTEXPR size_t simd_loop_step = 1;

      // todo decltype
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type lowest() POUTRE_NOEXCEPT
      {
          return poutre::compound_type<valuetype, 3>(std::numeric_limits<valuetype>::lowest(),
                                              std::numeric_limits<valuetype>::lowest(),
                                              std::numeric_limits<valuetype>::lowest());
      }

      // todo decltype
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type min() POUTRE_NOEXCEPT
      {
          return poutre::compound_type<valuetype, 3>(std::numeric_limits<valuetype>::min(),
                                              std::numeric_limits<valuetype>::min(),
                                              std::numeric_limits<valuetype>::min());
      }

      // todo decltype
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type max() POUTRE_NOEXCEPT
      {
          return poutre::compound_type<valuetype, 3>(std::numeric_limits<valuetype>::max(),
                                              std::numeric_limits<valuetype>::max(),
                                              std::numeric_limits<valuetype>::max());
      }
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type inf()
      {
          return min();
      }
      POUTRE_ALWAYS_INLINE POUTRE_STATIC_CONSTEXPR storage_type sup()
      {
          return max();
      }
  };
} // namespace poutre