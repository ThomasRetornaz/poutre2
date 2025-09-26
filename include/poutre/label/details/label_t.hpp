
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   label_t.hpp
 * @author Thomas Retornaz
 * @brief  Template labelling operators
 *
 *
 */

#include <algorithm>
#include <deque>
#include <filesystem>
#include <type_traits>
#include <numeric>
#include <poutre/base/config.hpp>
#include <poutre/base/details/data_structures/array_view.hpp>
// #include <poutre/geodesy/geodesy.hpp>
// #include <poutre/low_level_morpho/details/ero_dil_static_se_t.hpp>
#include <iostream>
#include <poutre/structuring_element/details/neighbor_list_static_se_t.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <queue>
#include <vector>

namespace poutre::label::details {
/**
 * @addtogroup poutre_label_group
 *@{
*/


template <class T>
struct not_background
{
  bool operator()(const T pixel_value) const noexcept {return pixel_value != T(0);}
};

template <>
struct not_background<pFLOAT>
{
  bool operator()(const pFLOAT pixel_value) const noexcept {return std::abs(pixel_value) > 1E-10F;}
};

template <>
struct not_background<pDOUBLE>
{
  bool operator()(const pDOUBLE pixel_value) const noexcept {return std::abs(pixel_value) > 1E-10;}
};

template <class T>
struct always_accept
{
  bool operator()( POUTRE_MAYBE_UNUSED const T pixel_value ) const noexcept
  {
    return true;
  }
  bool operator()(POUTRE_MAYBE_UNUSED const T pixel_value_1, POUTRE_MAYBE_UNUSED const T pixel_value_2) const noexcept
  {
    return true;
  }
};

template <class T>
struct sup_zero
{
  bool operator()( POUTRE_MAYBE_UNUSED const T pixel_value ) const noexcept
  {
    return pixel_value>T(0);
  }
  bool operator()(POUTRE_MAYBE_UNUSED const T pixel_value_1, POUTRE_MAYBE_UNUSED const T pixel_value_2) const noexcept
  {
    return pixel_value_2>T(0);
  }
};

template <typename T> struct label_binary_accept    {using type = not_background<T>;};
template <typename T> struct label_binary_neighbor_relation  {using type = sup_zero<T>;};

template <typename T> struct label_flat_zones_accept    {using type = always_accept<T>;};
template <typename T> struct label_flat_zones_neighbor_relation  {using type = std::equal_to<T>;};

template <typename IdxT = std::size_t>
struct disjoint_min_sets
{
  static_assert(std::is_unsigned_v<IdxT>,
        "IdxT should be an unsigned type");
  static_assert(std::is_integral_v<IdxT>,
      "IdxT should be an integral type");

public:
  using IndexType = IdxT;
  std::vector<IndexType> parents;
  //std::vector<IndexType> sizes;
  explicit disjoint_min_sets(std::size_t size) noexcept : parents()  //, sizes(size, 1)
  {
    parents.reserve(size);
  }
  IndexType get_new_label()
  {
    const auto new_vertex  = static_cast<IndexType>(parents.size());
    POUTRE_ASSERTCHECK(new_vertex<=std::numeric_limits<IndexType>::max(),"labels exhausted, use larger type in disjoint_min_sets");
    parents.push_back(new_vertex);
    return new_vertex;
  }

  IndexType find_root(const IndexType vertex) noexcept
  {
    IndexType parent = parents[vertex];
    if (parents[parent] != parent) {
      parents[vertex] = parent = find_root(parent);
    }
    return parent;
  }

  IndexType union_sets(const IndexType vertex1, const IndexType vertex2) noexcept
  {
    auto vertex1_parent = find_root(vertex1);
    auto vertex2_parent = find_root(vertex2);
    if (vertex1_parent <vertex2_parent) {
      parents[vertex2_parent] = vertex1_parent;
      return vertex1_parent;
    }
    else if (vertex2_parent < vertex1_parent) {
      parents[vertex1_parent] = vertex2_parent;
      return vertex2_parent;
    }
    return vertex1_parent;
  }

  std::pair<std::vector<IndexType>,size_t> min_label() noexcept
  {
    std::vector<IndexType> out(parents.size());
    std::size_t count = 0;
    for (size_t i = 0; i< parents.size(); ++i) {
      if (parents[i] == i) {
        count+=1;
      }
      out[i] = static_cast<IndexType>(count);
    }
    return std::make_pair(out, count);
  }
};

template< poutre::se::Common_NL_SE nl_static,
          typename Tin,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewIn,
          template<typename, ptrdiff_t>
          class ViewOut,
          class accep_t = typename label_binary_accept<Tin>::type,
          class nl_relation_t = typename label_binary_neighbor_relation<Tin>::type>
struct t_label_helper
{
  static_assert(Rank == poutre::se::details::static_se_traits<nl_static>::rank, "SE and view have not the same Rank");
  std::size_t operator()(const ViewIn<const Tin, Rank> &i_vin,
                  const ViewOut<Tout, Rank> &o_vout) const
  {
    POUTRE_CHECK(i_vin.size() == o_vout.size(), "Incompatible views size");

    // More check
    auto vInbound   = i_vin.bound();
    auto vOutbound  = o_vout.bound();
    auto stridevIn  = i_vin.stride();
    auto stridevOut = o_vout.stride();
    POUTRE_CHECK(vOutbound==vInbound, "Incompatible bound");
    POUTRE_CHECK(stridevIn==stridevOut, "Incompatible stride");

    // init output
    std::fill(o_vout.data(),o_vout.data()+o_vout.size(),static_cast<Tout>(0));

    auto [nl_upper, _ ]   = poutre::se::details::static_se_traits<nl_static>::split_coordinates_upper_lower();

    const auto sentinel = std::numeric_limits<Tout>::max();
    disjoint_min_sets sets(static_cast<std::size_t>(std::floor(std::sqrt(o_vout.size()))));
    const auto accept = accep_t();
    const auto neighbor_relation = nl_relation_t();

    {
      auto beg1 = begin(vInbound);
      auto end1 = end(vInbound);
      for( ; beg1 != end1; ++beg1 ) {
        auto curr_center_val = i_vin[*beg1];
        if (!accept(curr_center_val)) {
          continue;
        }
        auto label = sentinel;
        for( const auto &idx_nl_upper : nl_upper )
        {
          const auto delta_nl_idx = *beg1 + idx_nl_upper;
          if( !vInbound.contains(delta_nl_idx) ) {continue;}
          if (neighbor_relation(curr_center_val, i_vin[delta_nl_idx])) {
            const auto new_label = o_vout[delta_nl_idx];
            if (label == sentinel || label == new_label) ///????
              {
              label = new_label;
              }
            else {
              label = static_cast<Tout>(sets.union_sets(static_cast<std::size_t>(label), static_cast<std::size_t>(new_label)));
            }
          }
        }

        if (label==sentinel) {
          label = static_cast<Tout>(sets.get_new_label());
        }
      o_vout[*beg1] = label;
      }
    }

    // temp display
    // {
    //   std::cout<<"before min label \n";
    //   std::cout<<"out \n";
    //   {
    //     auto begit = begin(vInbound);
    //     auto endit = end(vInbound);
    //     for( ; begit != endit; ++begit ) {
    //       std::cout << static_cast<int>(o_vout[*begit]) << " ";
    //     }
    //     std::cout<<"\n";
    //   }
    // }

    // in order to enumerate label along scanning
    const auto [new_labels, nb_labels] = sets.min_label();
    //
    // // temp display
    // {
    //   std::cout<<"min label \n";
    //   {
    //
    //     for( const auto&val:new_labels ) {
    //       std::cout << static_cast<int>(val) << " ";
    //     }
    //     std::cout<<"\n";
    //   }
    // }

    // flatten pass
    {
      auto beg1 = begin(vInbound);
      auto end1 = end(vInbound);
      for( ; beg1 != end1; ++beg1 ) {
        if (accept(i_vin[*beg1]))
        {
          o_vout[*beg1] =static_cast<Tout>(new_labels[sets.find_root(static_cast<std::size_t>(o_vout[*beg1]))]);
        }
      }
    }
    return nb_labels;
  }
};

template< poutre::se::Common_NL_SE nl_static,
          typename Tin,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewIn,
          template<typename, ptrdiff_t>
          class ViewOut>
struct t_label_binaryHelper
{
  using t_label_operator = t_label_helper<nl_static,Tin,Tout,Rank,ViewIn,ViewOut,
                                          typename label_binary_accept<Tin>::type,
                                          typename label_binary_neighbor_relation<Tin>::type>;

  size_t operator()(const ViewIn<const Tin, Rank> &i_vin,
                  const ViewOut<Tout, Rank> &o_vout) const
  {
      return t_label_operator()(i_vin,o_vout);
  }
};

template< poutre::se::Common_NL_SE nl_static,
          typename Tin,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewIn,
          template<typename, ptrdiff_t>
          class ViewOut>
struct t_label_flat_zones_Helper
{
  using t_label_operator = t_label_helper<nl_static,Tin,Tout,Rank,ViewIn,ViewOut,
                                          typename label_flat_zones_accept<Tin>::type,
                                          typename label_flat_zones_neighbor_relation<Tin>::type>;

  size_t operator()(const ViewIn<const Tin, Rank> &i_vin,
                  const ViewOut<Tout, Rank> &o_vout) const
  {
    return t_label_operator()(i_vin,o_vout);
  }
};

template< typename Tin,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewIn,
          template<typename, ptrdiff_t>
          class ViewOut>
size_t t_label_binaryDispatch(
    const ViewIn<const Tin, Rank> &i_vin,
    const poutre::se::Common_NL_SE nl_static,
    const ViewOut<Tout, Rank> &o_vout)
{
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "t_label_binaryDispatch Incompatible views size");

  if constexpr (Rank == 1) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX1D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESegmentX1D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_label_binaryDispatch unsupported nl_static");}
    }
    //return 0;
  }
  if constexpr (Rank == 2) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESquare2D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESquare2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SECross2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentX2D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESegmentX2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY2D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESegmentY2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_label_binaryDispatch unsupported nl_static");}
    }
  }
  if constexpr (Rank == 3) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SECross3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESquare3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentX3D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESegmentX3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY3D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESegmentY3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentZ3D: {
      t_label_binaryHelper<poutre::se::Common_NL_SE::SESegmentZ3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_label_binaryDispatch unsupported nl_static");}
    }
  }
  POUTRE_RUNTIME_ERROR("t_label_binaryDispatch unsupported nb dims");
  // cppcheck-suppress missingReturn
  //return 0; //NOLINT
}

template<typename Tin, typename Tout, ptrdiff_t Rank>
size_t t_label_binary(
    const poutre::details::image_t<Tin, Rank> &i_img,
    poutre::se::Common_NL_SE nl_static,
    poutre::details::image_t<Tout, Rank> &o_img)
{
 AssertSizesCompatible(i_img, o_img, "t_label_binary incompatible size");
  AssertImagesAreDifferent(i_img, o_img, "t_label_binary output must be != than input images");

 auto viewIn = view(i_img);
 auto viewOut = view(o_img);
 return t_label_binaryDispatch(viewIn, nl_static, viewOut);
}

template< typename Tin,
          typename Tout,
          ptrdiff_t Rank,
          template<typename, ptrdiff_t>
          class ViewIn,
          template<typename, ptrdiff_t>
          class ViewOut>
size_t t_label_flat_zonesDispatch(
    const ViewIn<const Tin, Rank> &i_vin,
    const poutre::se::Common_NL_SE nl_static,
    const ViewOut<Tout, Rank> &o_vout)
{
  POUTRE_CHECK(i_vin.size() == o_vout.size(), "t_label_flat_zonesDispatch Incompatible views size");

  if constexpr (Rank == 1) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESegmentX1D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESegmentX1D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_label_flat_zonesDispatch unsupported nl_static");}
    }
    //return 0;
  }
  if constexpr (Rank == 2) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SESquare2D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESquare2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SECross2D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SECross2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentX2D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESegmentX2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY2D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESegmentY2D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_label_flat_zonesDispatch unsupported nl_static");}
    }
  }
  if constexpr (Rank == 3) {
    switch(nl_static) {
    case poutre::se::Common_NL_SE::SECross3D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SECross3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESquare3D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESquare3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentX3D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESegmentX3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentY3D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESegmentY3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    case poutre::se::Common_NL_SE::SESegmentZ3D: {
      t_label_flat_zones_Helper<poutre::se::Common_NL_SE::SESegmentZ3D, Tin, Tout, Rank, ViewIn, ViewOut> op;
      return op(i_vin, o_vout);
    } break;
    default:
    {
      POUTRE_RUNTIME_ERROR("t_label_flat_zonesDispatch unsupported nl_static");}
    }
  }
  // cppcheck-suppress missingReturn
  POUTRE_RUNTIME_ERROR("t_label_flat_zonesDispatch unsupported nb dims");
  //return 0; //NOLINT
}

template<typename Tin, typename Tout, ptrdiff_t Rank>
size_t t_label_flat_zones(
    const poutre::details::image_t<Tin, Rank> &i_img,
    poutre::se::Common_NL_SE nl_static,
    poutre::details::image_t<Tout, Rank> &o_img)
{
 AssertSizesCompatible(i_img, o_img, "t_label_flat_zones incompatible size");
  AssertImagesAreDifferent(i_img, o_img, "t_label_flat_zones output must be != than input images");

 auto viewIn = view(i_img);
 auto viewOut = view(o_img);
 return t_label_flat_zonesDispatch(viewIn, nl_static, viewOut);
}
//! @} doxygroup: poutre_label_group
}//poutre::label::details