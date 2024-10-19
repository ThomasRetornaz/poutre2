/*
 * array_view -- https://github.com/wardw/array_view
 *
 * Copyright (c) 2015, Tom Ward - All rights reserved.
 * BSD 2-clause “Simplified” License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * + Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * + Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <poutre/base/poutreConfig.hpp>
#include <type_traits>

/*
template <ptrdiff_t Rank>
class index
{
public:
        // constants and types
        static constexpr ptrdiff_t Rank = Rank;
        using reference              = ptrdiff_t&;
        using const_reference        = const ptrdiff_t&;
        using size_type              = size_t;
        using value_type             = ptrdiff_t;

        static_assert(Rank > 0, "Size of Rank must be greater than 0");

        // construction
        constexpr index() noexcept;
        template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
        constexpr index(value_type v) noexcept;
        constexpr index(std::initializer_list<value_type> il);

        // element access
        constexpr reference       operator[](size_type n);
        constexpr const_reference operator[](size_type n) const;

        // arithmetic
        constexpr index& operator+=(const index& rhs);
        constexpr index& operator-=(const index& rhs);

        template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
        constexpr index& operator++();
        template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
        constexpr index  operator++(int);
        template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
        constexpr index& operator--();
        template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
        constexpr index  operator--(int);

        constexpr index  operator+() const noexcept;
        constexpr index  operator-() const;

        constexpr index& operator*=(value_type v);
        constexpr index& operator/=(value_type v);

private:
        std::array<value_type, rank> index_ = {};
};

// index equality
template <ptrdiff_t Rank>
constexpr bool operator==(const index<Rank>& lhs, const index<Rank>& rhs) noexcept;
template <ptrdiff_t Rank>
constexpr bool operator!=(const index<Rank>& lhs, const index<Rank>& rhs) noexcept;

// index arithmetic
template <ptrdiff_t Rank>
constexpr index<Rank> operator+(const index<Rank>& lhs, const index<Rank>& rhs);
template <ptrdiff_t Rank>
constexpr index<Rank> operator-(const index<Rank>& lhs, const index<Rank>& rhs);
template <ptrdiff_t Rank>
constexpr index<Rank> operator*(const index<Rank>& lhs, ptrdiff_t v);
template <ptrdiff_t Rank>
constexpr index<Rank> operator*(ptrdiff_t v, const index<Rank>& rhs);
template <ptrdiff_t Rank>
constexpr index<Rank> operator/(const index<Rank>& lhs, ptrdiff_t v);


template <ptrdiff_t Rank>
class bounds
{
public:
        // constants and types
        static constexpr ptrdiff_t Rank = Rank;
        using reference              = ptrdiff_t&;
        using const_reference        = const ptrdiff_t&;
        using iterator               = bounds_iterator<Rank>;
        using const_iterator         = bounds_iterator<Rank>;
        using size_type              = size_t;
        using value_type             = ptrdiff_t;

        // construction
        constexpr bounds() noexcept;
        template <size_t R = Rank, typename = std::enable_if_t<R == 1>>
        constexpr bounds(value_type v);
        constexpr bounds(std::initializer_list<value_type> il);

        // observers
        constexpr size_type size() const noexcept;
        constexpr bool      contains(const index<Rank>& idx) const noexcept;

        // iterators
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;

        // element access
        constexpr reference       operator[](size_type n);
        constexpr const_reference operator[](size_type n) const;

        // arithmetic
        constexpr bounds& operator+=(const index<Rank>& rhs);
        constexpr bounds& operator-=(const index<Rank>& rhs);

        constexpr bounds& operator*=(value_type v);
        constexpr bounds& operator/=(value_type v);

private:
        std::array<value_type, rank> bounds_ = {};
};


// bounds equality
template <ptrdiff_t Rank>
constexpr bool operator==(const bounds<Rank>& lhs, const bounds<Rank>& rhs) noexcept;
template <ptrdiff_t Rank>
constexpr bool operator!=(const bounds<Rank>& lhs, const bounds<Rank>& rhs) noexcept;

// bounds arithmetic
template <ptrdiff_t Rank>
constexpr bounds<Rank> operator+(const bounds<Rank>& lhs, const index<Rank>& rhs);
template <ptrdiff_t Rank>
constexpr bounds<Rank> operator+(const index<Rank>& lhs, const bounds<Rank>& rhs);
template <ptrdiff_t Rank>
constexpr bounds<Rank> operator-(const bounds<Rank>& lhs, const index<Rank>& rhs);
template <ptrdiff_t Rank>
constexpr bounds<Rank> operator*(const bounds<Rank>& lhs, ptrdiff_t v);
template <ptrdiff_t Rank>
constexpr bounds<Rank> operator*(ptrdiff_t v, const bounds<Rank>& rhs);
template <ptrdiff_t Rank>
constexpr bounds<Rank> operator/(const bounds<Rank>& lhs, ptrdiff_t v);


template <ptrdiff_t Rank>
class bounds_iterator
{
public:
        using iterator_category = unspecified;
        using value_type        = index<Rank>;
        using difference_type   = ptrdiff_t;
        using pointer           = unspecified;
        using reference         = const index<Rank>;

        bounds_iterator& operator++();
        bounds_iterator  operator++(int);
        bounds_iterator& operator--();
        bounds_iterator  operator--(int);

        bounds_iterator  operator+(difference_type n) const;
        bounds_iterator& operator+=(difference_type n);
        bounds_iterator  operator-(difference_type n) const;
        bounds_iterator& operator-=(difference_type n);

        difference_type  operator-(const bounds_iterator& rhs) const;

        reference operator*() const;
        pointer   operator->() const;
        reference operator[](difference_type n) const;
};

template <typename T, ptrdiff_t Rank = 1>
class array_view
{
public:
        static constexpr ptrdiff_t Rank = Rank;
        using index_type            = index<Rank>;
        using bounds_type            = bounds<Rank>;
        using size_type              = size_t;
        using value_type             = T;
        using pointer                = T*;
        using reference              = T&;

        constexpr array_view() noexcept;

        template <typename Viewable>             // only if Rank == 1
        constexpr array_view(Viewable&& vw);

        template <typename U, size_t R = Rank>   // only if Rank == 1
        constexpr array_view(const array_view<U, R>& rhs) noexcept;

        template <size_t Extent>                 // only if Rank == 1
        constexpr array_view(value_type (&arr)[Extent]) noexcept;

        template <typename U>
        constexpr array_view(const array_view<U, Rank>& rhs) noexcept;

        template <typename Viewable>
        constexpr array_view(Viewable&& vw, bounds_type bounds);

        constexpr array_view(pointer ptr, bounds_type bounds);

        // observers
        constexpr bounds_type bounds() const noexcept;
        constexpr size_type   size()   const noexcept;
        constexpr index_type stride() const noexcept;
        constexpr pointer     data()   const noexcept;

        constexpr reference operator[](const index_type& idx) const;

        // slicing and sectioning
        template <size_t R = Rank>                // only if Rank > 1
        constexpr array_view<T, Rank-1> operator[](ptrdiff_t slice) const;

        constexpr strided_array_view<T, Rank>
        section(const index_type& origin, const bounds_type& section_bounds) const;

        constexpr strided_array_view<T, Rank>
        section(const index_type& origin) const;
};


template <class T, ptrdiff_t Rank = 1>
class strided_array_view
{
public:
        // constants and types
        static constexpr ptrdiff_t Rank = Rank;
        using index_type            = index<Rank>;
        using bounds_type            = bounds<Rank>;
        using size_type              = size_t;
        using value_type             = T;
        using pointer                = T*;
        using reference              = T&;

        // constructors, copy, and assignment
        constexpr strided_array_view() noexcept;

        template <typename U>
        constexpr strided_array_view(const array_view<U, Rank>& rhs) noexcept;

        template <typename U>
        constexpr strided_array_view(const strided_array_view<U, Rank>& rhs) noexcept;

        constexpr strided_array_view(pointer ptr, bounds_type bounds, index_type stride);

        // observers
        constexpr bounds_type bounds() const noexcept;
        constexpr size_type   size()   const noexcept;
        constexpr index_type stride() const noexcept;

        // element access
        constexpr reference operator[](const index_type& idx) const;

        // slicing and sectioning
        template <size_t R = Rank>                // Only if Rank > 1
        constexpr strided_array_view<T, Rank-1> operator[](ptrdiff_t slice) const;

        constexpr strided_array_view<T, Rank>
        section(const index_type& origin, const bounds_type& section_bounds) const;

        constexpr strided_array_view<T, Rank>
        section(const index_type& origin) const;
};
*/
// FIXME tmp disable check anyway i will remove array_view
// NOLINTBEGIN
namespace poutre::details::av {

using size_t = std::size_t;

template<ptrdiff_t Rank> class index;
template<ptrdiff_t Rank> class bounds;
template<ptrdiff_t Rank> class bounds_iterator;
template<typename T, ptrdiff_t Rank> class array_view;
template<typename T, ptrdiff_t Rank> class strided_array_view;

template<ptrdiff_t rank> struct IsRankEqual1
{
  POUTRE_STATIC_CONSTEXPR bool value = false;
};

template<> struct IsRankEqual1<1>
{
  POUTRE_STATIC_CONSTEXPR bool value = true;
};

template<ptrdiff_t rank> struct IsRankEqual2
{
  POUTRE_STATIC_CONSTEXPR bool value = false;
};

template<> struct IsRankEqual2<2>
{
  POUTRE_STATIC_CONSTEXPR bool value = true;
};

template<ptrdiff_t rank1, ptrdiff_t rank2> struct UseSameRank
{
  POUTRE_STATIC_CONSTEXPR bool value = false;
};

template<ptrdiff_t rank> struct UseSameRank<rank, rank>
{
  POUTRE_STATIC_CONSTEXPR bool value = true;
};

template<ptrdiff_t Rank> class index
{
public:
  // constants and types
  static constexpr ptrdiff_t rank = Rank;
  using reference = std::ptrdiff_t &;
  using const_reference = const std::ptrdiff_t &;
  using size_type = ptrdiff_t;
  using value_type = std::ptrdiff_t;

  static_assert(Rank > 0, "Size of Rank must be greater than 0");

  // construction
  constexpr index() noexcept = default;
  // cppcheck-suppress noExplicitConstructor
  template<size_t R = Rank, typename = std::enable_if_t<R == 1>> constexpr index(value_type v) noexcept
  {
    (*this)[0] = v;
  }
  constexpr index(std::initializer_list<value_type> il);

  // element access
  constexpr reference operator[](std::size_t n) { return index_[n]; }
  constexpr const_reference operator[](std::size_t n) const { return index_[n]; }

  // arithmetic
  template<size_t R = Rank, typename = std::enable_if_t<R == 1>> constexpr index &operator++() { return ++(*this)[0]; }
  template<size_t R = Rank, typename = std::enable_if_t<R == 1>> constexpr index operator++(int)
  {
    return index<Rank>{ (*this)[0]++ };
  }
  template<size_t R = Rank, typename = std::enable_if_t<R == 1>> constexpr index &operator--() { return --(*this)[0]; }
  template<size_t R = Rank, typename = std::enable_if_t<R == 1>> constexpr index operator--(int)
  {
    return index<Rank>{ (*this)[0]-- };
  }

  constexpr index &operator+=(const index &rhs);
  constexpr index &operator-=(const index &rhs);

  constexpr index operator+() const noexcept { return *this; }
  constexpr index operator-() const
  {
    index<Rank> copy{ *this };
    // cppcheck-suppress useStlAlgorithm
    for (value_type &elem : copy.index_) { elem *= -1; }
    return copy;
  }

  constexpr index &operator*=(value_type v);
  constexpr index &operator/=(value_type v);

private:
  std::array<value_type, static_cast<size_t>(rank)> index_ = {};
};

template<ptrdiff_t Rank> constexpr index<Rank>::index(std::initializer_list<value_type> il)
{
  // Note `il` is not a constant expression, hence the runtime assert for now
  assert(il.size() == Rank);
  std::copy(il.begin(), il.end(), index_.data());
}

// arithmetic
template<ptrdiff_t Rank> constexpr index<Rank> &index<Rank>::operator+=(const index &rhs)
{
  // cppcheck-suppress useStlAlgorithm
  for (size_type i = 0; i < Rank; ++i) { (*this)[i] += rhs[i]; }
  return *this;
}

template<ptrdiff_t Rank> constexpr index<Rank> &index<Rank>::operator-=(const index &rhs)
{
  // cppcheck-suppress useStlAlgorithm
  for (size_type i = 0; i < Rank; ++i) { (*this)[i] -= rhs[i]; }
  return *this;
}

template<ptrdiff_t Rank> constexpr index<Rank> &index<Rank>::operator*=(value_type v)
{
  // cppcheck-suppress useStlAlgorithm
  for (value_type &elem : index_) { elem *= v; }
  return *this;
}

template<ptrdiff_t Rank> constexpr index<Rank> &index<Rank>::operator/=(value_type v)
{
  // cppcheck-suppress useStlAlgorithm
  for (value_type &elem : index_) { elem /= v; }
  return *this;
}


// Free functions

// index equality
template<ptrdiff_t Rank> constexpr bool operator==(const index<Rank> &lhs, const index<Rank> &rhs) noexcept
{
  for (size_t i = 0; i < Rank; ++i) {
    if (lhs[i] != rhs[i]) return false;
  }
  return true;
}

template<ptrdiff_t Rank> constexpr bool operator!=(const index<Rank> &lhs, const index<Rank> &rhs) noexcept
{
  return !(lhs == rhs);
}

// index arithmetic
template<ptrdiff_t Rank> constexpr index<Rank> operator+(const index<Rank> &lhs, const index<Rank> &rhs)
{
  return index<Rank>{ lhs } += rhs;
}

template<ptrdiff_t Rank> constexpr index<Rank> operator-(const index<Rank> &lhs, const index<Rank> &rhs)
{
  return index<Rank>{ lhs } -= rhs;
}

template<ptrdiff_t Rank> constexpr index<Rank> operator*(const index<Rank> &lhs, std::ptrdiff_t v)
{
  return index<Rank>{ lhs } *= v;
}

template<ptrdiff_t Rank> constexpr index<Rank> operator*(std::ptrdiff_t v, const index<Rank> &rhs)
{
  return index<Rank>{ rhs } *= v;
}

template<ptrdiff_t Rank> constexpr index<Rank> operator/(const index<Rank> &lhs, std::ptrdiff_t v)
{
  return index<Rank>{ lhs } /= v;
}


template<ptrdiff_t Rank> class bounds
{
public:
  // constants and types
  static constexpr ptrdiff_t rank = Rank;
  using reference = std::ptrdiff_t &;
  using const_reference = const std::ptrdiff_t &;
  using iterator = bounds_iterator<rank>;
  using const_iterator = bounds_iterator<rank>;
  using size_type = size_t;
  using value_type = std::ptrdiff_t;

  static_assert(rank > 0, "Size of Rank must be greater than 0");

  // construction
  constexpr bounds() noexcept = default;

  // Question: is there a reason this constructor is not `noexcept` ?
  template<size_t R = Rank, typename = std::enable_if_t<R == 1>> constexpr explicit bounds(value_type v)
  {
    (*this)[0] = v;
    // postcondition();
  }
  constexpr bounds(std::initializer_list<value_type> il);

  // observers
  [[nodiscard]] constexpr size_type size() const noexcept;
  constexpr bool contains(const index<Rank> &idx) const noexcept;

  // iterators
  const_iterator begin() const noexcept { return const_iterator{ *this }; };
  const_iterator end() const noexcept
  {
    iterator iter{ *this };
    return iter._setOffTheEnd();
  }

  // element access
  constexpr reference operator[](std::size_t n) { return bounds_[n]; }
  constexpr const_reference operator[](std::size_t n) const { return bounds_[n]; }

  // arithmetic
  constexpr bounds &operator+=(const index<Rank> &rhs);
  constexpr bounds &operator-=(const index<Rank> &rhs);

  constexpr bounds &operator*=(value_type v);
  constexpr bounds &operator/=(value_type v);

private:
  std::array<value_type, static_cast<size_t>(rank)> bounds_ = {};

  // void postcondition() { /* todo */ };
};

// construction
template<ptrdiff_t Rank> constexpr bounds<Rank>::bounds(const std::initializer_list<value_type> il)
{
  assert(il.size() == Rank);

  std::copy(il.begin(), il.end(), bounds_.data());
  // postcondition();
}

// observers
template<ptrdiff_t Rank> constexpr size_t bounds<Rank>::size() const noexcept
{
  size_type product{ 1 };
  // cppcheck-suppress useStlAlgorithm
  for (const value_type &elem : bounds_) { product *= static_cast<size_type>(elem); }
  return product;
}

template<ptrdiff_t Rank> constexpr bool bounds<Rank>::contains(const index<Rank> &idx) const noexcept
{
  for (size_type i = 0; i < Rank; ++i) {
    if (!(0 <= idx[i] && idx[i] < (*this)[i])) { return false; }
  }
  return true;
}

// iterators
// todo

// arithmetic
template<ptrdiff_t Rank> constexpr bounds<Rank> &bounds<Rank>::operator+=(const index<Rank> &rhs)
{
  // cppcheck-suppress useStlAlgorithm
  for (std::size_t i = 0; i < static_cast<size_t>(Rank); ++i) { bounds_[i] += rhs[i]; }
  // postcondition();
  return *this;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> &bounds<Rank>::operator-=(const index<Rank> &rhs)
{
  // cppcheck-suppress useStlAlgorithm
  for (std::size_t i = 0; i < static_cast<size_t>(Rank); ++i) { bounds_[i] -= rhs[i]; }
  // postcondition();
  return *this;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> &bounds<Rank>::operator*=(value_type v)
{
  // cppcheck-suppress useStlAlgorithm
  for (value_type &elem : bounds_) { elem *= v; }
  // postcondition();
  return *this;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> &bounds<Rank>::operator/=(value_type v)
{
  // cppcheck-suppress useStlAlgorithm
  for (value_type &elem : bounds_) { elem /= v; }
  // postcondition();
  return *this;
}


// Free functions

// bounds equality
template<ptrdiff_t Rank> constexpr bool operator==(const bounds<Rank> &lhs, const bounds<Rank> &rhs) noexcept
{
  for (size_t i = 0; i < Rank; ++i) {
    if (lhs[i] != rhs[i]) return false;
  }
  return true;
}

template<ptrdiff_t Rank> constexpr bool operator!=(const bounds<Rank> &lhs, const bounds<Rank> &rhs) noexcept
{
  return !(lhs == rhs);
}

// bounds arithmetic
template<ptrdiff_t Rank> constexpr bounds<Rank> operator+(const bounds<Rank> &lhs, const index<Rank> &rhs)
{
  return bounds<Rank>{ lhs } += rhs;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> operator+(const index<Rank> &lhs, const bounds<Rank> &rhs)
{
  return bounds<Rank>{ rhs } += lhs;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> operator-(const bounds<Rank> &lhs, const index<Rank> &rhs)
{
  return bounds<Rank>{ lhs } -= rhs;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> operator*(const bounds<Rank> &lhs, std::ptrdiff_t v)
{
  return bounds<Rank>{ lhs } *= v;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> operator*(std::ptrdiff_t v, const bounds<Rank> &rhs)
{
  return bounds<Rank>{ rhs } *= v;
}

template<ptrdiff_t Rank> constexpr bounds<Rank> operator/(const bounds<Rank> &lhs, std::ptrdiff_t v)
{
  return bounds<Rank>{ lhs } /= v;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> begin(const bounds<Rank> &b) noexcept { return b.begin(); }

template<ptrdiff_t Rank> bounds_iterator<Rank> end(const bounds<Rank> &b) noexcept { return b.end(); }


template<ptrdiff_t Rank> class bounds_iterator
{
public:
  using iterator_category = std::random_access_iterator_tag;// unspecified but satisfactory
  using value_type = index<Rank>;
  using difference_type = std::ptrdiff_t;
  using pointer = index<Rank> *;// unspecified but satisfactory (?)
  using reference = const index<Rank>;

  static_assert(Rank > 0, "Size of Rank must be greater than 0");

  explicit bounds_iterator(const bounds<Rank> bounds, index<Rank> off = index<Rank>()) noexcept
    : bounds_(bounds), index_(off)
  {}

  bool operator==(const bounds_iterator &rhs) const
  {
    // Requires *this and rhs are iterators over the same bounds object.
    return index_ == rhs.index_;
  }

  bounds_iterator &operator++();
  bounds_iterator operator++(int);
  bounds_iterator &operator--();
  bounds_iterator operator--(int);

  bounds_iterator operator+(difference_type n) const;
  bounds_iterator &operator+=(difference_type n);
  bounds_iterator operator-(difference_type n) const;
  bounds_iterator &operator-=(difference_type n);

  difference_type operator-(const bounds_iterator &rhs) const;

  // Note this iterator is not a true random access iterator, nor meets N4512
  // + operator* returns a value type (and not a reference)
  // + operator-> returns a pointer to the current value type, which breaks N4512 as this
  //   must be considered invalidated after any subsequent operation on this iterator
  reference operator*() const { return index_; }
  pointer operator->() const { return &index_; }

  reference operator[](difference_type n) const
  {
    bounds_iterator<Rank> iter(*this);
    return (iter += n).index_;
  }

  bounds_iterator &_setOffTheEnd();

private:
  bounds<Rank> bounds_;
  index<Rank> index_;
};

template<ptrdiff_t Rank> bounds_iterator<Rank> bounds_iterator<Rank>::operator++(int)
{
  bounds_iterator tmp(*this);
  ++(*this);
  return tmp;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> &bounds_iterator<Rank>::operator++()
{
  // watchit: dim must be signed in order to fail the condition dim>=0
  for (std::ptrdiff_t dim = (Rank - 1); dim >= 0; --dim) {
    if (++index_[static_cast<std::size_t>(dim)] < bounds_[static_cast<std::size_t>(dim)]) {
      return (*this);
    } else {
      index_[static_cast<std::size_t>(dim)] = 0;
    }
  }

  // off-the-end value
  _setOffTheEnd();

  return *this;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> &bounds_iterator<Rank>::operator--()
{
  // watchit: dim must be signed in order to fail the condition dim>=0
  for (auto dim = Rank - 1; dim >= 0; --dim) {
    if (--index_[static_cast<size_t>(dim)] >= 0) {
      return (*this);
    } else {
      index_[static_cast<size_t>(dim)] = bounds_[static_cast<size_t>(dim)] - 1;
    }
  }

  // before-the-start value
  for (size_t dim = 0; dim < Rank - 1; ++dim) { index_[dim] = 0; }
  index_[Rank - 1] = -1;
  return *this;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> bounds_iterator<Rank>::operator--(int)
{
  bounds_iterator tmp(*this);
  --(*this);
  return tmp;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> &bounds_iterator<Rank>::_setOffTheEnd()
{
  for (ptrdiff_t dim = 0; dim < Rank - 1; ++dim) {
    index_[static_cast<size_t>(dim)] = bounds_[static_cast<size_t>(dim)] - 1;
  }
  index_[Rank - 1] = bounds_[Rank - 1];

  return *this;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> &bounds_iterator<Rank>::operator+=(difference_type n)
{
  for (ptrdiff_t dim = (Rank - 1); dim >= 0; --dim) {
    difference_type remainder = (n + index_[static_cast<size_t>(dim)]) % bounds_[static_cast<size_t>(dim)];
    n = (n + index_[static_cast<size_t>(dim)]) / bounds_[static_cast<size_t>(dim)];
    index_[static_cast<size_t>(dim)] = remainder;
  }
  assert(n == 0);// no overflow
  return *this;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> bounds_iterator<Rank>::operator+(difference_type n) const
{
  bounds_iterator<Rank> iter(*this);
  return iter += n;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> &bounds_iterator<Rank>::operator-=(difference_type n)
{
  // take (diminished) radix compliment
  auto diminishedRadixComplement = [&]() {
    for (int dim = (Rank - 1); dim >= 0; --dim) { index_[dim] = bounds_[dim] - index_[dim]; }
  };

  diminishedRadixComplement();
  *this += n;
  diminishedRadixComplement();

  return *this;
}

template<ptrdiff_t Rank> bounds_iterator<Rank> bounds_iterator<Rank>::operator-(difference_type n) const
{
  bounds_iterator<Rank> iter(*this);
  return iter -= n;
}

// Free functions

template<ptrdiff_t Rank> bool operator==(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs)
{
  return lhs.operator==(rhs);
}

template<ptrdiff_t Rank> bool operator!=(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs)
{
  return !lhs.operator==(rhs);
}

template<ptrdiff_t Rank> bool operator<(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs)
{
  return rhs - lhs > 0;
}

template<ptrdiff_t Rank> bool operator<=(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs)
{
  return !(lhs > rhs);
}

template<ptrdiff_t Rank> bool operator>(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs)
{
  return rhs < lhs;
}

template<ptrdiff_t Rank> bool operator>=(const bounds_iterator<Rank> &lhs, const bounds_iterator<Rank> &rhs)
{
  return !(lhs < rhs);
}

template<ptrdiff_t Rank>
bounds_iterator<Rank> operator+(typename bounds_iterator<Rank>::difference_type n, const bounds_iterator<Rank> &rhs);

namespace {

  template<typename Viewable, typename U, typename View = std::remove_reference_t<Viewable>>
  using is_viewable_on_u = std::integral_constant<bool,
    // std::is_convertible<typename View::size_type, std::ptrdiff_t>::value
    //&&
    std::is_convertible<typename View::value_type *, std::add_pointer_t<U>>::value
      && std::is_same<std::remove_cv_t<typename View::value_type>, std::remove_cv_t<U>>::value

    >;

  template<typename T, typename U>
  using is_viewable_value = std::integral_constant<bool,
    std::is_convertible<std::add_pointer_t<T>, std::add_pointer_t<U>>::value
      && std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value>;

  template<typename T, ptrdiff_t Rank>
  constexpr T &view_access(T *data, const index<Rank> &idx, const index<Rank> &stride)
  {
    std::ptrdiff_t off{};
    for (std::size_t i = 0; i < Rank; ++i) { off += idx[i] * stride[i]; }
    return data[off];
  }

}// namespace


template<typename T, ptrdiff_t Rank = 1> class array_view
{
public:
  static constexpr size_t rank = Rank;
  using index_type = index<Rank>;
  using bounds_type = bounds<Rank>;
  using size_type = size_t;
  using value_type = T;
  using pointer = T *;
  using reference = T &;

  static_assert(rank > 0, "Size of Rank must be greater than 0");

  constexpr array_view() noexcept : data_(nullptr) {}

  template<typename Viewable,
    ptrdiff_t R = Rank,
    typename = std::enable_if_t<R == 1 && is_viewable_on_u<Viewable, value_type>::value
      // todo: && decay_t<Viewable> is not a specialization of array_view
      >>
  // todo: assert static_cast<U*>(vw.data()) points to contigious data of at least vw.size()

  // cppcheck-suppress noExplicitConstructor
  constexpr array_view(Viewable &&vw) : data_(vw.data()), bounds_(static_cast<ptrdiff_t>(vw.size()))
  {}

  template<typename U, size_t R = Rank, typename = std::enable_if_t<R == 1 && is_viewable_value<U, value_type>::value>>
  // cppcheck-suppress noExplicitConstructor
  constexpr array_view(const array_view<U, static_cast<ptrdiff_t>(R)> &rhs) noexcept
    : data_(rhs.data()), bounds_(rhs.bounds())
  {}


  template<size_t Extent, ptrdiff_t rank = Rank, class = typename std::enable_if<IsRankEqual1<rank>::value>::type>
  // cppcheck-suppress noExplicitConstructor
  constexpr array_view(value_type (&arr)[Extent]) noexcept : data_(arr), bounds_(Extent)
  {}


  template<typename U, typename = std::enable_if_t<is_viewable_value<U, value_type>::value>>
  // cppcheck-suppress noExplicitConstructor
  constexpr array_view(const array_view<U, Rank> &rhs) noexcept : data_(rhs.data()), bounds_(rhs.bounds())
  {}

  template<typename Viewable, typename = std::enable_if_t<is_viewable_on_u<Viewable, value_type>::value>>
  // cppcheck-suppress noExplicitConstructor
  constexpr array_view(Viewable &&vwi, bounds_type bounds) : data_(vwi.data()), bounds_(bounds)
  {
    assert(bounds.size() <= vwi.size());
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr array_view(pointer ptr, bounds_type bounds) : data_(ptr), bounds_(bounds) {}

  // observers
  constexpr bounds<Rank> bound() const noexcept { return bounds_; }

  [[nodiscard]] constexpr size_type size() const noexcept { return bound().size(); }

  constexpr index_type stride() const noexcept;

  constexpr pointer data() const noexcept { return data_; }

  constexpr reference operator[](const index_type &idx) const
  {
    assert(bound().contains(idx) == true);
    return view_access(data_, idx, stride());
  }

  // slicing and sectioning
  template<size_t R = Rank, typename = std::enable_if_t<R >= 2>>
  constexpr array_view<T, Rank - 1> operator[](std::ptrdiff_t slice) const
  {
    assert(0 <= slice && slice < bound()[0]);

    av::bounds<Rank - 1> new_bounds{};
    for (size_t i = 0; i < rank - 1; ++i) { new_bounds[i] = bound()[i + 1]; }

    std::ptrdiff_t off = slice * stride()[0];

    return array_view<T, Rank - 1>(data_ + off, new_bounds);
  }

  constexpr strided_array_view<T, Rank> section(const index_type &origin, const bounds_type &section_bounds) const
  {
    // todo: requirement is for any idx in section_bounds (boundary fail)
    // assert(bounds().contains(origin + section_bounds) == true);
    return strided_array_view<T, Rank>(&(*this)[origin], section_bounds, stride());
  }

  constexpr strided_array_view<T, Rank> section(const index_type &origin) const
  {
    // todo: requires checking for any idx in bounds() - origin
    // assert(bounds().contains(bounds()) == true);
    return strided_array_view<T, Rank>(&(*this)[origin], bound() - origin, stride());
  }

private:
  pointer data_;
  bounds_type bounds_;
};

template<typename T, ptrdiff_t Rank>
constexpr typename array_view<T, Rank>::index_type array_view<T, Rank>::stride() const noexcept
{
  index_type stride{};
  stride[Rank - 1] = 1;
  for (ptrdiff_t dim = Rank - 2; dim >= 0; --dim) {
    stride[static_cast<size_t>(dim)] = stride[static_cast<size_t>(dim) + 1] * bound()[static_cast<size_t>(dim) + 1];
  }
  return stride;
}


template<class T, ptrdiff_t Rank = 1> class strided_array_view
{
public:
  // constants and types
  static constexpr ptrdiff_t rank = Rank;
  using index_type = index<Rank>;
  using bounds_type = bounds<Rank>;
  using size_type = size_t;
  using value_type = T;
  using pointer = T *;
  using reference = T &;

  // constructors, copy, and assignment
  constexpr strided_array_view() noexcept : data_{ nullptr }, bounds_{}, stride_{} {}

  template<typename U, typename = std::enable_if_t<is_viewable_value<U, value_type>::value>>
  constexpr explicit strided_array_view(const array_view<U, Rank> &rhs) noexcept
    : data_{ rhs.data() }, bounds_{ rhs.bound() }, stride_{ rhs.stride() }
  {}
  template<typename U, typename = std::enable_if_t<is_viewable_value<U, value_type>::value>>
  constexpr explicit strided_array_view(const strided_array_view<U, Rank> &rhs) noexcept
    : data_{ rhs.data_ }, bounds_{ rhs.bound() }, stride_{ rhs.stride() }
  {}

  constexpr strided_array_view(pointer ptr, bounds_type bound, index_type stride)
    : data_(ptr), bounds_(bound), stride_(stride)
  {
    // todo: assert that sum(idx[i] * stride[i]) fits in std::ptrdiff_t
  }

  // observers
  constexpr bounds_type bound() const noexcept { return bounds_; }
  [[nodiscard]] constexpr size_type size() const noexcept { return bounds_.size(); }
  constexpr index_type stride() const noexcept { return stride_; }

  // element access
  constexpr reference operator[](const index_type &idx) const
  {
    assert(bound().contains(idx) == true);
    return view_access(data_, idx, stride_);
  }

  // slicing and sectioning
  template<size_t R = Rank, typename = std::enable_if_t<R >= 2>>
  constexpr strided_array_view<T, Rank - 1> operator[](std::ptrdiff_t slice) const
  {
    assert(0 <= slice && slice < bound()[0]);

    av::bounds<Rank - 1> new_bounds{};
    for (size_t i = 0; i < static_cast<size_t>(rank - 1); ++i) { new_bounds[i] = bound()[i + 1]; }

    av::index<Rank - 1> new_stride{};
    for (size_t i = 0; i < rank - 1; ++i) { new_stride[i] = stride()[i + 1]; }

    std::ptrdiff_t off = slice * stride()[0];

    return strided_array_view<T, Rank - 1>(data_ + off, new_bounds, new_stride);
  }

  constexpr strided_array_view<T, Rank> section(const index_type &origin, const bounds_type &section_bounds) const
  {
    // todo: requirement is for any idx in section_bounds (boundary fail)
    // assert(bounds().contains(origin + section_bounds) == true);
    return strided_array_view<T, Rank>(&(*this)[origin], section_bounds, stride());
  }

  constexpr strided_array_view<T, Rank> section(const index_type &origin) const
  {
    // todo: requires checking for any idx in bounds() - origin
    // assert(bounds().contains(bounds()) == true);
    return strided_array_view<T, Rank>(&(*this)[origin], bound() - origin, stride());
  }

private:
  pointer data_;
  bounds_type bounds_;
  index_type stride_;
};

// Default to false
template<class ViewType> struct is_strided : std::false_type
{
};

template<typename T, ptrdiff_t Rank> struct is_strided<strided_array_view<T, Rank>> : std::true_type
{
};

template<class ViewType1, class ViewType2> struct is_same_view_type : std::false_type
{
};

template<class ViewType> struct is_same_view_type<ViewType, ViewType> : std::true_type
{
};


using bd1d = bounds<1>;//! alias bounds 1D
using bd2d = bounds<2>;//! alias bounds 2D
using bd3d = bounds<3>;//! alias bounds 3D
using bd4d = bounds<4>;//! alias bounds 4D

using idx1d = index<1>;//! alias index 1D
using idx2d = index<2>;//! alias index 2D
using idx3d = index<3>;//! alias index 3D
using idx4d = index<4>;//! alias index 4D

}// namespace poutre::details::av
// NOLINTEND