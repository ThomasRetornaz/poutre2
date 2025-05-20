//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file image_t.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Define template image class
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <poutre/base/base.hpp>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
#include <poutre/base/details/simd/simd_helpers.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
#include <poutre/base/types_traits.hpp>
#include <utility>
#include <vector>

// #include <mdspan>

namespace poutre::details {

/**
 * @addtogroup image_processing_container_group Image Processing Template Container
 * @ingroup image_processing_group
 *@{
 */

template<class valuetype, std::ptrdiff_t Rank = 2>  class BASE_API image_t : public IInterface
{
  static_assert(Rank > 0, "Rank must be >0");

public:
  using self_type = image_t<valuetype, Rank>;
  using parent_interface = IInterface;
  using aligned_allocator =
    xs::aligned_allocator<typename poutre::TypeTraits<valuetype>::storage_type, SIMD_IDEAL_MAX_ALIGN_BYTES>;
  using value_type = valuetype;// typename
                               // TypeTraits<ptype>::storage_type;
  using const_value_type = std::add_const_t<value_type>;
  using pointer = std::add_pointer_t<value_type>;
  using reference = std::add_lvalue_reference_t<value_type>;
  using const_pointer = std::add_pointer_t<const_value_type>;
  using const_reference = std::add_lvalue_reference_t<const_value_type>;

  using difference_type = std::ptrdiff_t;

  using index = offset;
  using size_type = std::size_t;

  using coordinate_type = av::bounds<Rank>;
  using index_type = av::index<Rank>;
  using storage_type = std::vector<value_type, aligned_allocator>;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using reverse_iterator = typename storage_type::reverse_iterator;
  using const_reverse_iterator = typename storage_type::const_reverse_iterator;
  
  // static const std::ptrdiff_t m_numdims = NumDims;
  static const PType m_ptype = TypeTraits<value_type>::p_type;
  static const CompoundType m_ctype = TypeTraits<value_type>::c_type;
  static const std::ptrdiff_t m_numdims = Rank;

  [[nodiscard]] CompoundType GetCType() const noexcept override { return m_ctype; }

  [[nodiscard]] PType GetPType() const noexcept override { return m_ptype; }

  [[nodiscard]] std::vector<std::size_t> GetShape() const override
  {
    std::vector<std::size_t> out(this->m_numdims);
    // use normal traversal
    for (size_t i = 0; i < this->m_numdims; ++i) { out[i] = static_cast<std::size_t>(this->m_coordinnates[i]); }
    return out;
  }
  //! Get num of dimensions
  [[nodiscard]] std::size_t GetRank() const override { return m_numdims; }

  //! Get total number of elements
  [[nodiscard]] constexpr size_type size() const noexcept { return m_numelement; }

  //! no const direct access to underlying storage @Warning
  // cppcheck-suppress functionConst
  [[nodiscard]] pointer data() noexcept { return m_storage.data(); }

  //! const direct access to underlying storage
  [[nodiscard]] const_pointer data() const noexcept { return m_storage.data(); }

  iterator begin() noexcept {return m_storage.begin(); }

  [[nodiscard]] const_iterator cbegin() const noexcept { return m_storage.cbegin(); }

  iterator end() noexcept { return m_storage.end(); }

  [[nodiscard]] const_iterator cend() const noexcept { return m_storage.cend();; }

  reverse_iterator rbegin() noexcept { return m_storage.rbegin(); }

  [[nodiscard]] const_reverse_iterator crbegin() const noexcept
  {
    return m_storage.crbegin();
  }

  //! assign value to all elements
  void fill(const value_type &val) { std::fill(m_storage.begin(), m_storage.end(), val); }

  [[nodiscard]] std::string str() const noexcept override
  {
    std::ostringstream out;
    out << "Image" << std::endl;
    out << "\tCtype: " << this->GetCType() << std::endl;
    out << "\tPtype: " << this->GetPType() << std::endl;
    const auto &numDims = this->GetRank();
    out << "\tNumdim: " << numDims << std::endl;
    const auto &coords = this->GetShape();
    out << "\tcoord: (";
    for (size_t i = 0; i < numDims - 1; i++) { out << coords[i] << ", "; }
    if (static_cast<ptrdiff_t>(numDims) - 1 >= 0) { out << coords[numDims - 1]; }
    out << ")" << std::endl;
    return out.str();
  }

  constexpr explicit image_t(const std::vector<size_t> &dims) : m_storage(), m_coordinnates(), m_numelement(0)
  {
    if (dims.size() != m_numdims)
      POUTRE_RUNTIME_ERROR("Invalid input initializer Mismatch between Rank and dims.size()");
    for (size_t i = 0; i < this->m_numdims; ++i) { this->m_coordinnates[i] = static_cast<ptrdiff_t>(dims[i]); }
    if (m_numdims > 0) {
      m_numelement = static_cast<std::size_t>(m_coordinnates[0]);
      for (size_t i = 1; i < static_cast<std::size_t>(m_numdims); i++) {
        m_numelement *= (static_cast<std::size_t>(m_coordinnates[i]));
      }
      m_storage.resize(m_numelement);
    }
  }

  constexpr explicit image_t(const std::initializer_list<size_t> &dims) : m_storage(), m_coordinnates(), m_numelement(0)
  {
    if (dims.size() != m_numdims)
      POUTRE_RUNTIME_ERROR("Invalid input initializer Mismatch between Rank and dims.size()");
    auto it = dims.begin();
    for (size_t i = 0; i < this->m_numdims; ++i, ++it) { this->m_coordinnates[i] = static_cast<std::ptrdiff_t>(*it); }
    // compute full array size
    if (m_numdims > 0) {
      m_numelement = static_cast<std::size_t>(m_coordinnates[0]);
      for (size_t i = 1; i < static_cast<std::size_t>(m_numdims); i++) {
        m_numelement *= (static_cast<std::size_t>(m_coordinnates[i]));
      }
      m_storage.resize(m_numelement);
    }
  }

  // see this later
  image_t(const image_t &rhs) = default;
  image_t &operator=(const image_t &rhs) = default;
  // see this later
  image_t(image_t &&other) = default;
  image_t &operator=(image_t &&other) = default;
  ~image_t() = default;

  constexpr const coordinate_type shape() const noexcept { return m_coordinnates; }


  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  constexpr scoord GetXSize() const noexcept
  {
    return this->m_coordinnates[1];
  }

  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  constexpr scoord GetYSize() const noexcept
  {
    return this->m_coordinnates[0];
  }

  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  POUTRE_CONSTEXPR const_pointer GetLineBuffer(scoord y) const POUTRE_NOEXCEPTONLYNDEBUG
  {
    POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
    POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
    return &m_storage[static_cast<std::size_t>(y) * static_cast<std::size_t>(m_coordinnates[0])];
  }


  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  // cppcheck-suppress functionConst
  pointer GetLineBuffer(scoord y) POUTRE_NOEXCEPTONLYNDEBUG
  {
    POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
    POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
    return &m_storage[static_cast<std::size_t>(y) * static_cast<std::size_t>(m_coordinnates[0])];
  }

  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  void SetPixel(scoord x, scoord y, value_type value) POUTRE_NOEXCEPTONLYNDEBUG
  {
    POUTRE_ASSERTCHECK(x < this->m_coordinnates[1], "Access out of bound");
    POUTRE_ASSERTCHECK(x >= 0, "Access out of bound");
    POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
    POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
    m_storage[static_cast<std::size_t>(y) * static_cast<std::size_t>(m_coordinnates[0]) + static_cast<std::size_t>(x)] = value;
  }

  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  void SetPixel(av::idx2d idx, value_type value) POUTRE_NOEXCEPTONLYNDEBUG
  {
    this->SetPixel(idx[1], idx[0], value);
  }

  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  constexpr value_type GetPixel(scoord x, scoord y) const POUTRE_NOEXCEPTONLYNDEBUG
  {
    POUTRE_ASSERTCHECK(x < this->m_coordinnates[1], "Access out of bound");
    POUTRE_ASSERTCHECK(x >= 0, "Access out of bound");
    POUTRE_ASSERTCHECK(y < this->m_coordinnates[0], "Access out of bound");
    POUTRE_ASSERTCHECK(y >= 0, "Access out of bound");
    return m_storage[static_cast<std::size_t>(y) * static_cast<std::size_t>(m_coordinnates[0]) + static_cast<std::size_t>(x)];
  }

  template<size_t R = Rank, typename = std::enable_if_t<R == 2>>
  POUTRE_CONSTEXPR value_type GetPixel(av::idx2d idx) const POUTRE_NOEXCEPTONLYNDEBUG
  {
    return this->GetPixel(idx[1], idx[0]);
  }

  void swap(self_type &rhs) noexcept
  {
    if (this != &rhs) {
      using std::swap;
      swap(this->m_storage, rhs.m_storage);// nothrow
      swap(this->m_coordinnates,
        rhs.m_coordinnates);// nothrow
      swap(this->m_numelement, rhs.m_numelement);
    }
  }

private:
  storage_type m_storage;
  coordinate_type m_coordinnates;
  size_type m_numelement;
};

template<class valuetype, std::ptrdiff_t Rank>
  constexpr poutre::details::av::array_view<valuetype, Rank> view(image_t<valuetype, Rank> &i_img)
  {
    return poutre::details::av::array_view<valuetype, Rank>(i_img.data(), i_img.shape());
  }

template<class valuetype, std::ptrdiff_t Rank>
  constexpr poutre::details::av::array_view<const valuetype, Rank> view(const image_t<valuetype, Rank> &i_img)
  {
    return poutre::details::av::array_view<const valuetype, Rank>(i_img.data(), i_img.shape());
  }

#if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
extern template class BASE_API image_t<pUINT8, 1>;
extern template class BASE_API image_t<pINT32, 1>;
extern template class BASE_API image_t<pFLOAT, 1>;
extern template class BASE_API image_t<pINT64, 1>;
extern template class BASE_API image_t<pDOUBLE, 1>;

extern template class BASE_API image_t<pUINT8, 2>;
extern template class BASE_API image_t<pINT32, 2>;
extern template class BASE_API image_t<pFLOAT, 2>;
extern template class BASE_API image_t<pINT64, 2>;
extern template class BASE_API image_t<pDOUBLE, 2>;

extern template class BASE_API image_t<compound_type<pUINT8, 3>, 2>;
extern template class BASE_API image_t<compound_type<pINT32, 3>, 2>;
extern template class BASE_API image_t<compound_type<pFLOAT, 3>, 2>;
extern template class BASE_API image_t<compound_type<pINT64, 3>, 2>;
extern template class BASE_API image_t<compound_type<pDOUBLE, 3>, 2>;

extern template class BASE_API image_t<compound_type<pUINT8, 4>, 2>;
extern template class BASE_API image_t<compound_type<pINT32, 4>, 2>;
extern template class BASE_API image_t<compound_type<pFLOAT, 4>, 2>;
extern template class BASE_API image_t<compound_type<pINT64, 4>, 2>;
extern template class BASE_API image_t<compound_type<pDOUBLE, 4>, 2>;

extern template class BASE_API image_t<pUINT8, 3>;
extern template class BASE_API image_t<pINT32, 3>;
extern template class BASE_API image_t<pFLOAT, 3>;
extern template class BASE_API image_t<pINT64, 3>;
extern template class BASE_API image_t<pDOUBLE, 3>;

extern template class BASE_API image_t<pUINT8, 4>;
extern template class BASE_API image_t<pINT32, 4>;
extern template class BASE_API image_t<pFLOAT, 4>;
extern template class BASE_API image_t<pINT64, 4>;
extern template class BASE_API image_t<pDOUBLE, 4>;
#endif

//! @} doxygroup: image_processing_container_group
}// namespace poutre::details
