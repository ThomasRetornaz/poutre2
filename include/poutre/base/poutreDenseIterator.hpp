
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   poutreDenseIterator.hpp
 * @author Thomas Retornaz
 * @brief  Define iterator capabilities over contiguous allocated contener
 *
 *
 */

#include <poutre/base/poutreConfig.hpp>
#include <poutre/base/poutreTypes.hpp>

#include <iterator>

namespace poutre {
/*!
 * @defgroup iterator_group Define iterators
 * @ingroup poutre_base_group
 * @{
 */
struct strided_iterator_tag : std::random_access_iterator_tag
{
};

struct contiguous_iterator_tag : std::random_access_iterator_tag
{
};

//! Raw dense iterator with random access
template<typename DataType, bool Const = false> class pdense_iterator
{
public:
  using value_type = DataType;
  /* deduce const qualifier from bool Const parameter */
  using reference = typename std::conditional_t<Const, DataType const &, DataType &>;
  using pointer = typename std::conditional_t<Const, DataType const *, DataType *>;
  using self_type = pdense_iterator<value_type, Const>;

  using difference_type = std::ptrdiff_t;
  using iterator_category = contiguous_iterator_tag;

  explicit pdense_iterator(DataType *ptr = nullptr, DataType *ptrorig = nullptr)// NOLINT
    : m_ptr(ptr), m_ptrorig(ptrorig)
  {}

  pdense_iterator(const self_type &rhs) = default;
  pdense_iterator &operator=(const self_type &rhs) = default;

  pdense_iterator(self_type &&rhs) noexcept = default;
  pdense_iterator &operator=(self_type &&rhs) noexcept = default;

  ~pdense_iterator() = default;

  POUTRE_CONSTEXPR explicit operator bool() const { return static_cast<bool>((m_ptr) && (m_ptrorig)); }

  POUTRE_CONSTEXPR bool operator==(const self_type &rhs) const
  {
    return (m_ptr == rhs.m_ptr && m_ptrorig == rhs.m_ptrorig);
  }
  POUTRE_CONSTEXPR bool operator!=(const self_type &rhs) const
  {
    return (m_ptr != rhs.m_ptr || m_ptrorig != rhs.m_ptrorig);
  }

  POUTRE_CONSTEXPR pdense_iterator<DataType> &operator+=(const std::ptrdiff_t &movement)
  {
    m_ptr += movement;
    return (*this);
  }
  POUTRE_CONSTEXPR pdense_iterator<DataType> &operator-=(const std::ptrdiff_t &movement)
  {
    m_ptr -= movement;
    return (*this);
  }
  POUTRE_CONSTEXPR pdense_iterator<DataType> &operator++()
  {
    ++m_ptr;
    return (*this);
  }
  POUTRE_CONSTEXPR pdense_iterator<DataType> &operator--()
  {
    --m_ptr;
    return (*this);
  }

  POUTRE_CONSTEXPR const pdense_iterator<DataType> operator++(int)
  {
    auto temp(*this);
    ++m_ptr;
    return temp;
  }
  POUTRE_CONSTEXPR const pdense_iterator<DataType> operator--(int)
  {
    auto temp(*this);
    --m_ptr;
    return temp;
  }

  POUTRE_CONSTEXPR ptrdiff_t operator-(const self_type &rawIterator)
  {
    return std::distance(rawIterator.m_ptr, this->m_ptr);
  }

  template<bool _Const = Const> std::enable_if_t<!Const, reference> POUTRE_CONSTEXPR operator*() { return *m_ptr; }

  template<bool _Const = Const> std::enable_if_t<_Const, reference> POUTRE_CONSTEXPR operator*() const
  {
    return *m_ptr;
  }

  template<bool _Const = Const> std::enable_if_t<_Const, pointer> POUTRE_CONSTEXPR operator->() const { return m_ptr; }

  template<bool _Const = Const> std::enable_if_t<!_Const, pointer> POUTRE_CONSTEXPR operator->() { return m_ptr; }

  [[nodiscard]] POUTRE_CONSTEXPR offset getOffset() const { return m_ptr - m_ptrorig; }

protected:
  pointer m_ptr;
  pointer m_ptrorig;
};

//! Raw reversed dense iterator with random access
template<typename DataType, bool Const = false> class pdense_reverse_iterator : public pdense_iterator<DataType, Const>
{
public:
  using parent = pdense_iterator<DataType>;
  using value_type = typename parent::value_type;
  using reference = typename std::conditional_t<Const, DataType const &, DataType &>;
  using pointer = typename std::conditional_t<Const, DataType const *, DataType *>;
  using difference_type = std::ptrdiff_t;

  using parent::m_ptr;
  using parent::m_ptrorig;
  using self_type = pdense_reverse_iterator<DataType, Const>;

  explicit pdense_reverse_iterator(DataType *ptr = nullptr, DataType *ptrorig = nullptr) : parent(ptr, ptrorig) {}
  // pdense_reverse_iterator(const self_type& rhs)
  //  {
  //  this->m_ptr = rhs.getPtr( ); this->m_ptrorig = rhs.getOrigPtr( );
  //  }

  pdense_reverse_iterator(const pdense_reverse_iterator &rhs) = default;
  pdense_reverse_iterator &operator=(const pdense_reverse_iterator &rawReverseIterator) = default;
  pdense_reverse_iterator(pdense_reverse_iterator &&rhs) noexcept = default;
  pdense_reverse_iterator &operator=(self_type &&rhs) noexcept = default;

  ~pdense_reverse_iterator() = default;

  // self_type&           operator=(const self_type& rawIterator)
  //  {
  //  this->m_ptr = rawIterator.getPtr( ); this->m_ptrorig =
  //  rawIterator.getOrigPtr( ); return (*this);
  //  }

  POUTRE_CONSTEXPR self_type &operator+=(const std::ptrdiff_t &movement)
  {
    this->m_ptr -= movement;
    return (*this);
  }
  POUTRE_CONSTEXPR self_type &operator-=(const std::ptrdiff_t &movement)
  {
    this->m_ptr += movement;
    return (*this);
  }
  POUTRE_CONSTEXPR self_type &operator++()
  {
    --this->m_ptr;
    return (*this);
  }
  POUTRE_CONSTEXPR self_type &operator--()
  {
    ++this->m_ptr;
    return (*this);
  }
  POUTRE_CONSTEXPR const self_type operator++(int) /*const*/
  {
    auto temp(*this);
    --this->m_ptr;
    return temp;
  }
  POUTRE_CONSTEXPR const self_type operator--(int) /*const*/
  {
    auto temp(*this);
    ++this->m_ptr;
    return temp;
  }

  POUTRE_CONSTEXPR ptrdiff_t operator-(const self_type &rawReverseIterator)
  { /*todo assert same orig?*/
    return std::distance(this->getPtr(), rawReverseIterator.getPtr());
  }

  POUTRE_CONSTEXPR offset getOffset() const { return m_ptr - m_ptrorig; }

  POUTRE_CONSTEXPR pdense_iterator<DataType> base()
  {
    pdense_iterator<DataType> forwardIterator(this->m_ptr, this->m_ptrorig);
    ++forwardIterator;
    return forwardIterator;
  }
};
/**
//! @} doxygroup: iterator_group
*/
}// namespace poutre
