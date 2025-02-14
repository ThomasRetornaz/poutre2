
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   poutreStaticContainer.hpp
 * @author Thomas Retornaz
 * @brief Define something similar to std::array but enforce
 * constexpr, inlining and optimisations  for small array size.
 */

#include <poutre/base/poutreConfig.hpp>
#include <poutre/base/poutreContainerHelper.hpp>
#include <poutre/base/poutreDenseIterator.hpp>
#include <poutre/base/poutreTypes.hpp>

#include <array>
#include <iterator>
#include <sstream>
#include <stdexcept>

#ifndef __clang__
#include <compare>
#endif

#define HAVE_THREE_WAY_OPERATOR __cpp_lib_three_way_comparison &&__cpp_lib_concepts && !_MSC_VER

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4425)// 4425 improper support constexpr
#endif

// #include <amp.h>

namespace poutre {
/*!
 * @defgroup static_array_group Static Array
 * @ingroup poutre_base_group
 * @{
 */

/**
 *@brief Something similar to std::array but we enforce here constexpr
 * capabilities, inlining and specializations for small array size.
 *@ingroup static_array_group
 */
template<class valuetype, ptrdiff_t Rank> class static_array_base// NOLINT
{
  // Static assertion
  static_assert(Rank > 0, "static_array_base requires a Rank>0");
  static_assert(std::is_arithmetic<valuetype>::value, "static_array_base only support arithmetic type");

protected:
  // valuetype m_array[Rank]; //! actual storage has static sized buffer TODO ALIGNEMENT
  // typename std::aligned_storage<sizeof(valuetype), alignof(valuetype)>::type
  // m_array[Rank];
  std::array<valuetype, Rank> m_array;

public:
  POUTRE_STATIC_CONSTEXPR ptrdiff_t rank = Rank;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

  using size_type = std::size_t;
  using value_type = valuetype;
  using const_value_type = typename std::add_const<value_type>::type;
  using pointer = typename std::add_pointer<value_type>::type;
  using reference = typename std::add_lvalue_reference<value_type>::type;
  using const_pointer = typename std::add_pointer<const_value_type>::type;
  using const_reference = typename std::add_lvalue_reference<const_value_type>::type;
  using difference_type = std::ptrdiff_t;
  using self_type = static_array_base<valuetype, rank>;

#endif// #ifndef DOXYGEN_SHOULD_SKIP_THIS

  /** @name Construction and Assignment
   */

  /**@{*/
  // FIXME
  POUTRE_CONSTEXPR
  static_array_base() : m_array{ 0 } /*POUTRE_NOEXCEPT*/ {}
  // POUTRE_CONSTEXPR
  //    static_array_base() POUTRE_NOEXCEPT {}
  // template <typename other=valuetype,
  //	 class = typename std::enable_if<std::is_convertible<other,
  // value_type>::value> && typename std::enable_if<Rank==1>::type >
  // POUTRE_CONSTEXPR explicit static_array_base( other a)z POUTRE_NOEXCEPT
  //    {
  //    m_array[0] = value_type(a);
  //    }

  // template <typename other,
  //     typename std::enable_if<Rank==2>::type* = nullptr>
  // POUTRE_CONSTEXPR static_array_base(typename
  // std::enable_if<std::is_convertible<other, value_type>::value && NumElmnt ==
  // 2, value_type>::type a0, other a1) POUTRE_NOEXCEPT
  //{
  // m_array[0] = value_type(a0);
  // m_array[1] = value_type(a1);
  //}
  //
  // template <typename other,
  //             typename std::enable_if<Rank==1>::type* = nullptr>
  // POUTRE_CONSTEXPR static_array_base(typename
  // std::enable_if<std::is_convertible<other, value_type>::value && NumElmnt ==
  // 3, value_type>::type a0, other a1, value_type a2) POUTRE_NOEXCEPT
  //{
  // m_array[0] = a0;
  // m_array[1] = a1;
  // m_array[2] = a2;
  //}
  //
  //

  POUTRE_CONSTEXPR explicit static_array_base(value_type a) POUTRE_NOEXCEPT : m_array()
  {
    static_assert(Rank == 1,
      "static_array_base(value_type) is only supported "
      "on static_array_base<T, 1>");
    m_array[0] = value_type(a);
  }

  POUTRE_CONSTEXPR explicit static_array_base(value_type a0, value_type a1) POUTRE_NOEXCEPT : m_array()
  {
    static_assert(Rank == 2,
      "static_array_base(value_type,value_type) is only "
      "supported on static_array_base<T, 2>");
    m_array[0] = value_type(a0);
    m_array[1] = value_type(a1);
  }

  POUTRE_CONSTEXPR explicit static_array_base(value_type a0, value_type a1, value_type a2) POUTRE_NOEXCEPT : m_array()
  {
    static_assert(Rank == 3,
      "static_array_base(value_type,value_type,value_"
      "type) is only supported on static_array_base<T, "
      "3>");
    m_array[0] = a0;
    m_array[1] = a1;
    m_array[2] = a2;
  }

  POUTRE_CONSTEXPR explicit static_array_base(value_type a0, value_type a1, value_type a2, value_type a3)
    POUTRE_NOEXCEPT : m_array()
  {
    static_assert(Rank == 4,//-V112
      "static_array_base(value_type,value_type,value_"
      "type,value_type) is only supported on "
      "static_array_base<T, 4>");
    m_array[0] = a0;
    m_array[1] = a1;
    m_array[2] = a2;
    m_array[3] = a3;
  }

  POUTRE_CONSTEXPR
  static_array_base(const std::initializer_list<value_type> &rhs) : m_array()
  {
    POUTRE_CHECK(rhs.size() == rank, "Ill formed initializer list: rhs.size() must equal Rank");
    auto it = std::begin(rhs);
    auto itend = std::end(rhs);
    for (size_t i = 0; i < Rank && it != itend; i++, ++it) { m_array[i] = *it; }
    // safe but silly behavior if no static assertion
    // std::copy_n(rhs.begin( ), std::min<ptrdiff_t>((ptrdiff_t)rank, rhs.size(
    // )), m_size_list.begin( ));
    // std::copy(rhs.begin(), rhs.end(), &m_array[0]);
  }

  POUTRE_CONSTEXPR explicit static_array_base(const value_type (&rhs)[Rank]) POUTRE_NOEXCEPT : m_array()
  {
    details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
  }

  POUTRE_CONSTEXPR
  static_array_base(const self_type &rhs) POUTRE_NOEXCEPT : m_array()
  {
    details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this);
  }

  POUTRE_CONSTEXPR self_type &operator=(const self_type &rhs) POUTRE_NOEXCEPT
  {
    if (this != &rhs) { details::helper_assign_container_op<self_type, AssignOpType::AssignOp, Rank>::op(rhs, *this); }
    return *this;
  }

  /**@}*/

  /**@}*/
  /** @name Comparison Operators
   */

  /**@{*/
#if HAVE_THREE_WAY_OPERATOR
  POUTRE_CONSTEXPR auto operator<=>(const static_array_base<valuetype, Rank> &rhs) const POUTRE_NOEXCEPT = default;
#else
  POUTRE_CONSTEXPR bool operator==(const self_type &rhs) const POUTRE_NOEXCEPT
  {
    return details::helper_comp_equal_container_op<self_type, Rank>::op(rhs, *this);
  }

  POUTRE_CONSTEXPR bool operator!=(const self_type &rhs) const POUTRE_NOEXCEPT
  {
    return !details::helper_comp_equal_container_op<self_type, Rank>::op(rhs, *this);
  }
#endif
  /**@}*/

  std::string str() const
  {
    std::ostringstream out;
    out << "[";
    size_t i = 0;
    if (!this->empty()) {
      for (i = 0; i < rank - 1; ++i) {
        out << m_array[i];
        out << ",";
      }
    }
    out << m_array[size() - 1];
    out << "]";
    return out.str();
  }

  /** @name Arithmetic Operators
   * Apply vector like operation on static_array_base with index aka translate
   */

  /**@{*/
  POUTRE_CONSTEXPR static_array_base &operator+=(const value_type &rhs) POUTRE_NOEXCEPT
  {
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpAdd, Rank>::op(rhs, *this);
    return *this;
  }

  POUTRE_CONSTEXPR static_array_base &operator-=(const value_type &rhs) POUTRE_NOEXCEPT
  {
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMinus, Rank>::op(rhs, *this);
    return *this;
  }

  /**@}*/

  /** @name Scaling Operators
   */

  /**@{*/
  template<typename ArithmeticType,
    typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
  POUTRE_CONSTEXPR static_array_base operator*(ArithmeticType v) const POUTRE_NOEXCEPT
  {
    self_type tmp(*this);// return a fresh new object
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMul, Rank>::op(v, tmp);
    return tmp;
  }

  template<typename ArithmeticType,
    typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
  POUTRE_CONSTEXPR static_array_base operator/(ArithmeticType v) const POUTRE_NOEXCEPT
  {
    self_type tmp(*this);// return a fresh new object
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpDiv, Rank>::op(v, tmp);
    return tmp;
  }

  template<typename ArithmeticType,
    typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
  POUTRE_CONSTEXPR static_array_base operator%(ArithmeticType v) POUTRE_NOEXCEPT
  {
    self_type tmp(*this);// return a fresh new object
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMod, Rank>::op(v, tmp);
    return tmp;
  }

  template<typename ArithmeticType,
    typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
  POUTRE_CONSTEXPR static_array_base &operator*=(ArithmeticType v) POUTRE_NOEXCEPT
  {
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMul, Rank>::op(v, *this);
    return *this;
  }

  template<typename ArithmeticType,
    typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
  POUTRE_CONSTEXPR static_array_base &operator/=(ArithmeticType v) POUTRE_NOEXCEPT
  {
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpDiv, Rank>::op(v, *this);
    return *this;
  }

  template<typename ArithmeticType,
    typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
  POUTRE_CONSTEXPR static_array_base &operator%=(ArithmeticType v) POUTRE_NOEXCEPT
  {
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOpMod, Rank>::op(v, *this);
    return *this;
  }

  /**@}*/

  // std::array like interface

  /** @name Capacity
   */
  /**@{*/

  //! Return length of sequence

  POUTRE_CONSTEXPR size_type size() const POUTRE_NOEXCEPT { return rank; }

  //! Return maximum possible length of sequence

  POUTRE_CONSTEXPR size_type max_size() const POUTRE_NOEXCEPT { return rank; }

  //! Is sequence empty?.Always false numelmnt>0

  POUTRE_CONSTEXPR bool empty() const POUTRE_NOEXCEPT { return false; }
  /**@}*/

  //! Assign value to all elements

  void assign(const valuetype &value) POUTRE_NOEXCEPT
  {
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOp, Rank>::op(value, *this);
  }

  //! Fill all elements with provided value

  void fill(const valuetype &value) POUTRE_NOEXCEPT
  {
    details::helper_assign_container_valueop<self_type, AssignOpType::AssignOp, Rank>::op(value, *this);
  }

  //! swap

  void swap(self_type &rhs) POUTRE_NOEXCEPT { std::swap<value_type, rank>(m_array, rhs.m_array); }

  /** @name Accessing Components
   */

  /**@{*/
  //! Access element
  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR reference operator[](difference_type n) POUTRE_NOEXCEPTONLYNDEBUG
  {
    POUTRE_ASSERTCHECK(n < rank, "static_array_base[n] n must be in [0,rank)");
    POUTRE_ASSERTCHECK(n >= 0, "static_array_base[n] n must be in [0,rank)");
    return m_array[n];
  }

  //! Access element
  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR const_reference operator[](difference_type n) const POUTRE_NOEXCEPTONLYNDEBUG
  {
    POUTRE_ASSERTCHECK(n < rank, "static_array_base[n] n must be in [0,rank)");
    POUTRE_ASSERTCHECK(n >= 0, "static_array_base[n] n must be in [0,rank)");
    return m_array[n];
  }

  //! Access element with bound checking

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR reference at(difference_type n)
  {
    POUTRE_CHECK(n < rank, "static_array_base.at(n) n must be in [0,rank)");
    POUTRE_CHECK(n >= 0, "static_array_base.at(n) n must be in [0,rank)");
    return m_array[n];
  }

  //! Access const element with bound checking

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR const_reference at(difference_type n) const
  {
    POUTRE_ASSERTCHECK(n < rank, "static_array_base.at(n) n must be in [0,rank)");
    POUTRE_ASSERTCHECK(n >= 0, "static_array_base.at(n) n must be in [0,rank)");
    return m_array[n];
  }

  //! Return first element of mutable sequence

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR reference front() POUTRE_NOEXCEPT { return m_array[0]; }

  //! Return first element of nonmutable sequence

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR const_reference front() const POUTRE_NOEXCEPT { return m_array[0]; }

  //! Return last element of mutable sequence

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR reference back() POUTRE_NOEXCEPT { return m_array[rank - 1]; }

  //! Return last element of nonmutable sequence

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR const_reference back() const POUTRE_NOEXCEPT { return m_array[rank - 1]; }

  //! Return pointer to mutable data array

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR pointer data() POUTRE_NOEXCEPT { return m_array.data(); }

  //! Return pointer to nonmutable data array

  POUTRE_ALWAYS_INLINE POUTRE_CONSTEXPR const_pointer data() const POUTRE_NOEXCEPT { return m_array.data(); }
  /**@}*/
};

template<typename value_type,
  ptrdiff_t size,
  typename ArithmeticType,
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
POUTRE_CONSTEXPR static_array_base<value_type, size> operator*(ArithmeticType v,
  const static_array_base<value_type, size> &rhs) POUTRE_NOEXCEPT
{
  static_array_base<value_type, size> tmp(rhs);
  return (tmp * v);
}

template<typename value_type,
  ptrdiff_t size,
  typename ArithmeticType,
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
POUTRE_CONSTEXPR static_array_base<value_type, size> operator/(ArithmeticType v,
  const static_array_base<value_type, size> &rhs) POUTRE_NOEXCEPT
{
  static_array_base<value_type, size> tmp(rhs);
  return (tmp / v);
}

template<typename value_type,
  ptrdiff_t size,
  typename ArithmeticType,
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type * = nullptr>
POUTRE_CONSTEXPR static_array_base<value_type, size> operator%(ArithmeticType v,
  const static_array_base<value_type, size> &rhs) POUTRE_NOEXCEPT
{
  static_array_base<value_type, size> tmp(rhs);
  return (tmp % v);
}

template<typename value_type, ptrdiff_t size>
POUTRE_CONSTEXPR void swap(static_array_base<value_type, size> &lhs,
  static_array_base<value_type, size> &rhs) POUTRE_NOEXCEPT// POUTRE_NOEXCEPT_IF(lhs.swap(rhs))
{
  (lhs.swap(rhs));
}

#if HAVE_THREE_WAY_OPERATOR
#else
template<typename value_type, ptrdiff_t size>
POUTRE_CONSTEXPR bool operator==(const static_array_base<value_type, size> &lhs,
  const static_array_base<value_type, size> &rhs) POUTRE_NOEXCEPT
{
  return lhs.operator==(rhs);
}

template<typename value_type, ptrdiff_t size>
POUTRE_CONSTEXPR bool operator!=(const static_array_base<value_type, size> &lhs,
  const static_array_base<value_type, size> &rhs) POUTRE_NOEXCEPT
{
  return lhs.operator!=(rhs);
}
#endif

template<typename value_type, ptrdiff_t size>
std::ostream &operator<<(std::ostream &os, const static_array_base<value_type, size> &rhs)
{
  os << rhs.str();
  return os;
}

template<class valuetype, ptrdiff_t Rank
  // TODO ADD ALIGNEMENT CAPABILITIES ?
  >
class static_array : public static_array_base<valuetype, Rank>
{
public:
  // inherit from parent
  using value_type = valuetype;

  using parent = static_array_base<value_type, Rank>;
  using self_type = static_array<value_type, Rank>;
#if (defined _MSC_VER && _MSC_VER <= 1800)
  using static_array_base<value_type, Rank>::static_array_base<value_type, Rank>;
#else
  using parent::parent;
#endif
  using typename parent::const_pointer;
  using typename parent::const_reference;
  using typename parent::difference_type;
  using typename parent::pointer;
  using typename parent::reference;

  using parent::assign;
  using parent::at;
  using parent::back;
  using parent::data;
  using parent::empty;
  using parent::fill;
  using parent::front;
  using parent::max_size;
  using parent::rank;
  using parent::size;
  using parent::swap;

  using parent::operator=;
#if HAVE_THREE_WAY_OPERATOR
#else
  using parent::operator==;
  using parent::operator!=;
#endif
  using parent::operator/=;
  using parent::operator*=;
  using parent::operator%=;
  using parent::operator+=;
  using parent::operator-=;
  using parent::operator/;
  using parent::operator*;
  using parent::operator%;

  // Default iteration capabilities
  using iterator = pdense_iterator<value_type>;
  using const_iterator = pdense_iterator<const value_type>;
  using reverse_iterator = pdense_reverse_iterator<value_type>;
  using const_reverse_iterator = pdense_reverse_iterator<const value_type>;

protected:
  using parent::m_array;

public:
  /** @name Construction and Assignment
   */

  /**@{*/
  POUTRE_CONSTEXPR static_array() POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent())) : parent() {}

  POUTRE_CONSTEXPR explicit static_array(value_type a) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a))) : parent(a)
  {}

  POUTRE_CONSTEXPR static_array(value_type a0, value_type a1) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1)))
    : parent(a0, a1)
  {}

  POUTRE_CONSTEXPR static_array(value_type a0, value_type a1, value_type a2)
    POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1, a2)))
    : parent(a0, a1, a2)
  {}

  POUTRE_CONSTEXPR static_array(value_type a0, value_type a1, value_type a2, value_type a3)
    POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(a0, a1, a2, a3)))
    : parent(a0, a1, a2, a3)
  {}

  POUTRE_CONSTEXPR
  static_array(const std::initializer_list<value_type> &rhs) : parent(rhs) {}

  POUTRE_CONSTEXPR explicit static_array(const value_type (&rhs)[Rank])
    POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(rhs)))
    : parent(rhs)
  {}

  POUTRE_CONSTEXPR static_array(const parent &rhs) POUTRE_NOEXCEPT_IF(POUTRE_NOEXCEPT_EXPR(parent(rhs))) : parent(rhs)
  {}
  /**@}*/

  /** @name Iteration
   */
  /**@{*/

  //! Return iterator at beginning of mutable sequence

  iterator begin() POUTRE_NOEXCEPT { return (iterator(std::addressof(m_array[0]), std::addressof(m_array[0]))); }

  //! Return iterator at endding of mutable sequence

  iterator end() POUTRE_NOEXCEPT { return (iterator(std::addressof(m_array[0]) + rank, std::addressof(m_array[0]))); }

  //! Return iterator at beginning of reversed mutable sequence

  reverse_iterator rbegin() POUTRE_NOEXCEPT
  {
    return (reverse_iterator(std::addressof(m_array[0]) + rank - 1, std::addressof(m_array[0])));
  }

  //! Return iterator at end of reversed mutable sequence

  reverse_iterator rend() POUTRE_NOEXCEPT
  {
    return (reverse_iterator(std::addressof(m_array[0]) - 1, std::addressof(m_array[0])));
  }

  //! Return iterator at beginning of nonmutable sequence

  const_iterator begin() const POUTRE_NOEXCEPT
  {
    return (const_iterator(std::addressof(m_array[0]), std::addressof(m_array[0])));
  }

  //! Return iterator at endding of mutable sequence

  const_iterator end() const POUTRE_NOEXCEPT
  {
    return (const_iterator(std::addressof(m_array[0]) + rank, std::addressof(m_array[0])));
  }

  //! Return iterator at beginning of reversed mutable sequence

  const_reverse_iterator rbegin() const POUTRE_NOEXCEPT
  {
    return (const_reverse_iterator(std::addressof(m_array[0]) + rank - 1, std::addressof(m_array[0])));
  }

  //! Return iterator at end of reversed mutable sequence

  const_reverse_iterator rend() const POUTRE_NOEXCEPT
  {
    return (const_reverse_iterator(std::addressof(m_array[0]) - 1, std::addressof(m_array[0])));
  }

  //! Return iterator at beginning of nonmutable sequence

  const_iterator cbegin() const POUTRE_NOEXCEPT { return static_cast<const self_type *>(this)->begin(); }

  //! Return iterator at endding of nonmutable sequence

  const_iterator cend() const POUTRE_NOEXCEPT { return static_cast<const self_type *>(this)->end(); }

  //! Return iterator at beginning of reversed nonmutable sequence

  const_reverse_iterator crbegin() const POUTRE_NOEXCEPT { return static_cast<const self_type *>((this))->rbegin(); }

  //! Return iterator at endding of reversed nonmutable sequence

  const_reverse_iterator crend() const POUTRE_NOEXCEPT { return static_cast<const self_type *>(this)->rend(); }

  /**@}*/
};

}// namespace poutre
