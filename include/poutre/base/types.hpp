
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   poutreTypes.hpp
 * @author Thomas Retornaz
 * @brief  Common type alias and helpers
 *
 *
 */

#include <poutre/base/poutreBase.hpp>

#include <format>
#include <iosfwd>

namespace poutre {
/*!
 * @defgroup types_group Define type alias
 * @ingroup poutre_base_group
 * @{
 */
using pbool = bool;//! boolean type
using pUINT8 = u8;//! UINT8 type
// using  pINT8 = signed char; //!INT8 type
using pUINT32 = u32;//! UINT32 type
using pINT32 = i32;//! INT32 type
using pFLOAT = f32;//! Float/Real type (default real type)
using pDOUBLE = f64;//! Float/Real type
using pINT64 = i64;//! INT64 type
using pUINT64 = u64;//! UINT64 type

using scoord = std::ptrdiff_t;//! scalar coordinate
using rcoord = pFLOAT;//! real coordinate

using offset = std::ptrdiff_t;//! offset

using str_type = std::string;//! string type
using prealtype = pFLOAT;//! real type @see @c pFLOAT

/*!
 * @brief Compare Operator
 */
enum class CompOpType {
  CompOpUndef = 0,//!< Undefined type
  CompOpEqual = 1 << 0,//! p1==p2
  CompOpDiff = 1 << 1,//! p1!=p2
  CompOpSup = 1 << 2,//! p1>p2
  CompOpSupEqual = 1 << 3,//! p1>=p2
  CompOpInf = 1 << 4,//! p1<p2
  CompOpInfEqual = 1 << 5,//! p1<=p2
  CompOp_Max = 1 << 5// sync with the max value
};

//! operator<< for CompOpType
BASE_API std::ostream &operator<<(std::ostream &, CompOpType);

//! operator>> for CompOpType
BASE_API std::istream &operator>>(std::istream &, CompOpType &);

/*!
 * @brief Arithmetic Operator
 */
enum class ArithOpType {
  ArithOpUndef = 0,//!< Undefined type
  ArithOpPlus = 1 << 1,//! r=a0+a1
  ArithOpMinus = 1 << 2,//! r=a0-a1
  ArithOpMul = 1 << 3,//! r=a0*a1
  ArithOpDiv = 1 << 4,//! r=a0/a1
  ArithOpMod = 1 << 5,//! r=a0%a1
  ArithOp_Max = 1 << 5// sync with the max value
};

//! operator<< for ArithOpType
BASE_API std::ostream &operator<<(std::ostream &, ArithOpType);

//! operator>> for ArithOpType
BASE_API std::istream &operator>>(std::istream &, ArithOpType &);

/*!
 * @brief Assignment Operator
 */
enum class AssignOpType {
  AssignOpUndef = 0,//!< Undefined type
  AssignOp = 1 << 0,//! r=a0
  AssignOpAdd = 1 << 1,//! r+=a0
  AssignOpMinus = 1 << 2,//! r-=a0
  AssignOpMul = 1 << 3,//! r*=a0
  AssignOpDiv = 1 << 4,//! r/=a0
  AssignOpMod = 1 << 5,//! r/=a0
  AssignOpNegate = 1 << 6,//! r/=-a0
  AssignOpType_Max = 1 << 6// sync with the max value
};

//! operator<< for AssignOpType
BASE_API std::ostream &operator<<(std::ostream &, AssignOpType);

//! operator>> for AssignOpType
BASE_API std::istream &operator>>(std::istream &, AssignOpType &);
// !@} doxygroup: types_group
}// namespace poutre

namespace std {
template<> struct formatter<poutre::CompOpType> : std::formatter<const char *>
{
  template<typename Context> auto format(const poutre::CompOpType state, Context &context) const
  {
    switch (state) {
    case poutre::CompOpType::CompOpDiff:
      return formatter<const char *>::format("!=", context);
    case poutre::CompOpType::CompOpEqual:
      return formatter<const char *>::format("==", context);
    case poutre::CompOpType::CompOpInf:
      return formatter<const char *>::format("<", context);
    case poutre::CompOpType::CompOpInfEqual:
      return formatter<const char *>::format("<=", context);
    case poutre::CompOpType::CompOpSup:
      return formatter<const char *>::format(">", context);
    case poutre::CompOpType::CompOpSupEqual:
      return formatter<const char *>::format(">=", context);
    default:
      return formatter<const char *>::format("Unknown", context);
    }

    // unreachable
    return context.out();
  }
};
}// namespace std
