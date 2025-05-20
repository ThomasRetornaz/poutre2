//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   types.hpp
 * @author Thomas Retornaz
 * @brief  Common type alias and helpers
 *
 *
 */

#include "config.hpp"
#include <array>
#include <cstddef>
#include <type_traits>

#ifndef __clang__
#include <compare>
#endif

#include <format>
#include <initializer_list>
#include <poutre/base/base.hpp>
#include <sstream>
#include <string>
#include <variant>

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


template<class valuetype, std::size_t Rank> class compound_type;
// template<class valuetype> class compound_type<valuetype, 3>;


template<class valuetype, std::size_t Rank> class compound_type// NOLINT
{
  // Static assertion
  static_assert(Rank > 0, "compound_type requires a Rank>0");
  static_assert(std::is_arithmetic_v<valuetype>, "compound_type only support arithmetic type");// or use concept ?
#ifndef DOXYGEN_SHOULD_SKIP_THIS

  using size_type = size_t;
  using value_type = valuetype;
  using const_value_type = std::add_const_t<value_type>;
  using pointer = std::add_pointer_t<value_type>;
  using reference = std::add_lvalue_reference_t<value_type>;
  using const_pointer = std::add_pointer_t<const_value_type>;
  using const_reference = std::add_lvalue_reference_t<const_value_type>;
  using difference_type = std::ptrdiff_t;
  using self_type = compound_type<valuetype, Rank>;
#endif

public:
  static constexpr std::size_t rank = Rank;
  constexpr compound_type() : m_array{ 0 } {}

  // template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  // constexpr explicit compound_type(value_type a) noexcept : m_array()
  // {
  //   m_array[0] = value_type(a);
  // }

  // template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  // constexpr explicit compound_type(value_type a0, value_type a1) noexcept : m_array()
  // {
  //   m_array[0] = value_type(a0);
  //   m_array[1] = value_type(a1);
  // }

  // template<size_t R = Rank, typename = std::enable_if_t<R == 3>>
  // constexpr explicit compound_type(value_type a0, value_type a1, value_type a2) noexcept : m_array()
  // {
  //   static_assert(Rank == 3,
  //     "static_array_base(value_type,value_type,value_"
  //     "type) is only supported on static_array_base<T, "
  //     "3>");
  //   m_array[0] = a0;
  //   m_array[1] = a1;
  //   m_array[2] = a2;
  // }

  template<size_t R = Rank, typename = std::enable_if_t<R == 4>>
  constexpr explicit compound_type(valuetype a0, valuetype a1, valuetype a2, valuetype a3) POUTRE_NOEXCEPT : m_array()
  {
    m_array[0] = a0;
    m_array[1] = a1;
    m_array[2] = a2;
    m_array[3] = a3;
  }
  constexpr compound_type(std::initializer_list<valuetype> rhs) : m_array()
  {
    POUTRE_CHECK(rhs.size() == rank, "Ill formed initializer list: rhs.size() must equal Rank");
    auto it = std::cbegin(rhs);
    auto itend = std::cend(rhs);
    for (size_t i = 0; i < Rank && it != itend; i++, ++it) { m_array[i] = *it; }
    // safe but silly behavior if no static assertion
    // std::copy_n(rhs.begin( ), std::min<ptrdiff_t>((ptrdiff_t)rank, rhs.size(
    // )), m_size_list.begin( ));
    // std::copy(rhs.begin(), rhs.end(), &m_array[0]);
  }

  constexpr explicit compound_type(const valuetype (&rhs)[Rank]) noexcept : m_array()
  {
    for (size_t i = 0; i < Rank; i++) { m_array[i] = rhs[i]; }
  }

  constexpr compound_type(const compound_type &rhs) = default;

  constexpr compound_type &operator=(const compound_type &rhs) = default;

  constexpr compound_type(compound_type &&rhs) = default;

  constexpr compound_type &operator=(compound_type &&rhs) = default;

  //! Return length of sequence

  static constexpr size_type size() noexcept { return rank; }

  //! Return maximum possible length of sequence

  static constexpr size_type max_size() noexcept { return rank; }

  //! Is sequence empty?.Always false numelmnt>0

  static constexpr bool empty() noexcept { return false; }

  /**@}*/

  //! Fill all elements with provided value
  void fill(const valuetype &value) POUTRE_NOEXCEPT { std::fill(begin(m_array), end(m_array), value); }

  //! swap
  void swap(self_type &rhs) POUTRE_NOEXCEPT { std::swap<value_type, rank>(m_array, rhs.m_array); }

  //! Strict equality operator
  bool operator==(const self_type &other) const noexcept
  {
    for (std::size_t i = 0; i < rank; ++i) {
      if (m_array[i] != other.m_array[i]) { return false; }
    }
    return true;
  }
  //! Inequality operator
  bool operator!=(const self_type &other) const noexcept
  {
    for (std::size_t i = 0; i < rank; ++i) {
      if (m_array[i] != other.m_array[i]) { return true; }
    }
    return false;
  }

  /** @name Accessing Components
   */

  /**@{*/
  //! Access element
  valuetype &operator[](difference_type n) noexcept { return m_array[static_cast<size_t>(n)]; }

  //! Access element
  const_reference operator[](difference_type n) const noexcept { return m_array[static_cast<size_t>(n)]; }

  [[nodiscard]] std::string to_string() const
  {
    std::ostringstream out;
    out << "[";
    size_t i = 0;

    for (i = 0; i < rank - 1; ++i) {
      out << m_array[i];
      out << ",";
    }
    out << m_array[size() - 1];
    out << "]";
    return out.str();
  }

private:
  std::array<valuetype, Rank> m_array;
};

template<typename value_type, std::size_t size>
constexpr bool operator==(const compound_type<value_type, size> &lhs,
  const compound_type<value_type, size> &rhs) noexcept
{
  return lhs.operator==(rhs);
}

template<typename value_type, std::size_t size>
constexpr bool operator!=(const compound_type<value_type, size> &lhs,
  const compound_type<value_type, size> &rhs) noexcept
{
  return lhs.operator!=(rhs);
}


//! Helper to retrieve dim off pixel storage type
template<class value_type> struct get_dim
{
  static constexpr size_t dim = 1;
};

//! Helper to retrieve dim off compound
template<class value_type, std::size_t Rank> struct get_dim<compound_type<value_type, Rank>>
{
  static constexpr size_t dim = Rank;
};

// specialization compound 3

template<class valuetype> class BASE_API compound_type<valuetype, 3>
{
public:
  static constexpr std::ptrdiff_t rank = 3;
  using self_type = compound_type<valuetype, 3>;
  using value_type = valuetype;

public:
  value_type m_pix1, m_pix2, m_pix3;

  //! Default constructor
  compound_type() = default;

  //! Constant assignment constructor
  compound_type(const value_type &pix1, const value_type &pix2, const value_type &pix3)
    : m_pix1(pix1), m_pix2(pix2), m_pix3(pix3)
  {}
  //! Constant assignment constructor
  constexpr explicit compound_type(const value_type &value) : m_pix1(value), m_pix2(value), m_pix3(value) {}

  template<class U, typename = std::enable_if_t<std::is_convertible_v<valuetype, U>>>
  constexpr compound_type(const compound_type<U, rank> &other)
    : m_pix1(static_cast<valuetype>(other.m_pix1)), m_pix2(static_cast<valuetype>(other.m_pix2)),
      m_pix3(static_cast<valuetype>(other.m_pix3))
  {}

  constexpr compound_type(const compound_type &rhs) = default;

  constexpr compound_type &operator=(const compound_type &rhs) = default;

  constexpr compound_type(compound_type &&rhs) = default;

  constexpr compound_type &operator=(compound_type &&rhs) = default;

  valuetype &operator[](ptrdiff_t idx) POUTRE_NOEXCEPTONLYNDEBUG
  {
    if (idx == 0) {
      return m_pix1;
    } else if (idx == 1) {
      return m_pix2;
    }
    POUTRE_ASSERTCHECK(
      idx == 2, "Unsupported dimension : " + std::to_string(i) + " > #dimension=" + std::to_string(dimension::value));
    return m_pix3;
  }

  const valuetype &operator[](ptrdiff_t idx) const POUTRE_NOEXCEPTONLYNDEBUG
  {
    if (idx == 0) {
      return m_pix1;
    } else if (idx == 1) {
      return m_pix2;
    }
    POUTRE_ASSERTCHECK(
      idx == 2, "Unsupported dimension : " + std::to_string(i) + " > #dimension=" + std::to_string(dimension::value));
    return m_pix3;
  }

  //! Strict equality operator
  bool operator==(const self_type &other) const noexcept
  {
    return (m_pix1 == other.m_pix1) && (m_pix2 == other.m_pix2) && (m_pix3 == other.m_pix3);
  }
  //! Inequality operator
  bool operator!=(const self_type &other) const noexcept
  {
    return (m_pix1 != other.m_pix1) || (m_pix2 != other.m_pix2) || (m_pix3 != other.m_pix3);
  }

  [[nodiscard]] std::string to_string() const
  {
    std::ostringstream out;
    out << "[";
    out << m_pix1;
    out << ",";
    out << m_pix2;
    out << ",";
    out << m_pix3;
    out << "]";
    return out.str();
  }

  //! Fill all elements with provided value
  void fill(const valuetype &value) POUTRE_NOEXCEPT
  {
    m_pix1 = value;
    m_pix2 = value;
    m_pix3 = value;
  }
};


// specialization compound 4

template<class valuetype> class BASE_API compound_type<valuetype, 4>
{
public:
  static constexpr std::ptrdiff_t rank = 4;
  using self_type = compound_type<valuetype, 4>;
  using value_type = valuetype;

public:
  value_type m_pix1, m_pix2, m_pix3, m_pix4;

  //! Default constructor
  compound_type() = default;
  //! Constant assignment constructor
  compound_type(const value_type &pix1, const value_type &pix2, const value_type &pix3, const value_type &pix4)
    : m_pix1(pix1), m_pix2(pix2), m_pix3(pix3), m_pix4(pix4)
  {}
  //! Constant assignment constructor
  constexpr explicit compound_type(const value_type &value) : m_pix1(value), m_pix2(value), m_pix3(value), m_pix4(value)
  {}

  template<class U, typename = std::enable_if_t<std::is_convertible_v<valuetype, U>>>
  // cppcheck-suppress missingMemberCopy
  constexpr compound_type(const compound_type<U, 4> &other)
    : m_pix1(static_cast<valuetype>(other.m_pix1)), m_pix2(static_cast<valuetype>(other.m_pix2)), m_pix3(static_cast<valuetype>(other.m_pix3)), m_pix4(static_cast<valuetype>(other.m_pix4))
  {}

  constexpr compound_type(const compound_type<valuetype, 4> &rhs) = default;

  constexpr compound_type<valuetype, 4>  &operator=(const compound_type<valuetype, 4> &rhs) = default;

  constexpr compound_type(compound_type<valuetype, 4> &&rhs) = default;

  constexpr compound_type<valuetype, 4>  &operator=(compound_type<valuetype, 4> &&rhs) = default;

  valuetype &operator[](ptrdiff_t idx) POUTRE_NOEXCEPTONLYNDEBUG
  {
    if (idx == 0) {
      return m_pix1;
    } else if (idx == 1) {
      return m_pix2;
    } else if (idx == 2) {
      return m_pix3;
    }
    POUTRE_ASSERTCHECK(
      idx == 3, "Unsupported dimension : " + std::to_string(i) + " > #dimension=" + std::to_string(dimension::value));
    return m_pix4;
  }

  const valuetype &operator[](ptrdiff_t idx) const POUTRE_NOEXCEPTONLYNDEBUG
  {
    if (idx == 0) {
      return m_pix1;
    } else if (idx == 1) {
      return m_pix2;
    } else if (idx == 2) {
      return m_pix3;
    }
    POUTRE_ASSERTCHECK(
      idx == 3, "Unsupported dimension : " + std::to_string(i) + " > #dimension=" + std::to_string(dimension::value));
    return m_pix4;
  }

  //! Strict equality operator
  bool operator==(const self_type &other) const noexcept
  {
    return (m_pix1 == other.m_pix1) && (m_pix2 == other.m_pix2) && (m_pix3 == other.m_pix3) && (m_pix4 == other.m_pix4);
  }
  //! Inequality operator
  bool operator!=(const self_type &other) const noexcept
  {
    return (m_pix1 != other.m_pix1) || (m_pix2 != other.m_pix2) || (m_pix3 != other.m_pix3) || (m_pix4 != other.m_pix4);
  }

  [[nodiscard]] std::string to_string() const
  {
    std::ostringstream out;
    out << "[";
    out << m_pix1;
    out << ",";
    out << m_pix2;
    out << ",";
    out << m_pix3;
    out << ",";
    out << m_pix4;
    out << "]";
    return out.str();
  }

  //! Fill all elements with provided value
  void fill(const valuetype &value) POUTRE_NOEXCEPT
  {
    m_pix1 = value;
    m_pix2 = value;
    m_pix3 = value;
    m_pix4 = value;
  }
};

using c3pUINT8 = compound_type<pUINT8, 3>;
using c3pINT32 = compound_type<pINT32, 3>;
using c3pFLOAT = compound_type<pFLOAT, 3>;
using c3pINT64 = compound_type<pINT64, 3>;
using c3pDOUBLE = compound_type<pDOUBLE, 3>;

using c4pUINT8 = compound_type<pUINT8, 4>;
using c4pINT32 = compound_type<pINT32, 4>;
using c4pFLOAT = compound_type<pFLOAT, 4>;
using c4pINT64 = compound_type<pINT64, 4>;
using c4pDOUBLE = compound_type<pDOUBLE, 4>;

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


enum class CompoundType {
  CompoundType_Undef = 0,//!< Undefined
  CompoundType_Scalar = 1 << 0,//!< scalar
  CompoundType_3Planes = 1 << 1,//!< 3 channels
  CompoundType_4Planes = 1 << 2,//!< 4 channels
                                // CompoundType_Container, //!< generic container //LATER
};

//! operator<< for CompoundType
BASE_API std::ostream &operator<<(std::ostream &, CompoundType);

//! operator>> for CompoundType
BASE_API std::istream &operator>>(std::istream &, CompoundType &);

enum class PType {
  PType_Undef = 0,//!< Undefined type
  // PType_Bin = 1 << 0,    //!< data {0,1} encode in  [0,255]
  // PType_BinPack  =  1 << 1,  //!< data {0,1} binary packed
  PType_GrayUINT8 = 1 << 2,//!< 8 bits per pixel, unsigned, grayscale data in [0,255]
  PType_GrayINT32 = 1 << 3,//!< 32 bits per pixel, signed, grayscale data in ]-2^31,+2^31[
  PType_F32 = 1 << 4,//!< Floating-point pixel, single precision (float)
  PType_GrayINT64 = 1 << 5,//!< Int64 pixel, think about integrale image
  PType_D64 = 1 << 6,//!< Floating-point pixel, double precision (long double)
  _PixelType_Max = 1 << 6// keep sync with the max value
};

using ScalarTypeVariant = std::variant<pUINT8, pINT32, pINT64, pFLOAT, pDOUBLE>;

//! operator<< for PType
BASE_API std::ostream &operator<<(std::ostream &, PType);

//! operator>> for PType
BASE_API std::istream &operator>>(std::istream &, PType &);

#if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
extern template class BASE_API compound_type<pUINT8, 3>;
extern template class BASE_API compound_type<pINT32, 3>;
extern template class BASE_API compound_type<pFLOAT, 3>;
extern template class BASE_API compound_type<pINT64, 3>;
extern template class BASE_API compound_type<pDOUBLE, 3>;

extern template class BASE_API compound_type<pUINT8, 4>;
extern template class BASE_API compound_type<pINT32, 4>;
extern template class BASE_API compound_type<pFLOAT, 4>;
extern template class BASE_API compound_type<pINT64, 4>;
extern template class BASE_API compound_type<pDOUBLE, 4>;
#endif

}// namespace poutre

namespace std {

template<> struct formatter<poutre::CompoundType> : std::formatter<const char *>
{
  template<typename Context> auto format(const poutre::CompoundType state, Context &context) const
  {
    switch (state) {
    case poutre::CompoundType::CompoundType_Scalar:
      return formatter<const char *>::format("Scalar", context);
    case poutre::CompoundType::CompoundType_3Planes:
      return formatter<const char *>::format("3Planes", context);
    case poutre::CompoundType::CompoundType_4Planes:
      return formatter<const char *>::format("4Planes", context);
    default:
      return formatter<const char *>::format("Unknown", context);
    }
    // unreachable
    return context.out();
  }
};
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

template<> struct formatter<poutre::PType> : std::formatter<const char *>
{
  template<typename Context> auto format(const poutre::PType state, Context &context) const
  {
    switch (state) {
    case poutre::PType::PType_GrayUINT8:
      return formatter<const char *>::format("GUINT8", context);
    case poutre::PType::PType_GrayINT32:
      return formatter<const char *>::format("GINT32", context);
    case poutre::PType::PType_F32:
      return formatter<const char *>::format("F32", context);
    case poutre::PType::PType_GrayINT64:
      return formatter<const char *>::format("GINT64", context);
    case poutre::PType::PType_D64:
      return formatter<const char *>::format("D64", context);
    default:
      return formatter<const char *>::format("Unknown", context);
    }
    // unreachable
    return context.out();
  }
};
}// namespace std