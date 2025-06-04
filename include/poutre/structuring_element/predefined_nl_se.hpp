
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   predefined_nl_se.hpp
 * @author Thomas Retornaz
 * @brief  Some predefined useful neighbor list se
 *
 *
 */

#include <poutre/base/config.hpp>
#include <poutre/structuring_element/structuring_element.hpp>
#include <poutre/structuring_element/details/neighbor_list_se_t.hpp>

namespace poutre::se {
/**
 * @addtogroup poutre_se_nl_predefine_group Some predefined Neighbor list Structuring Element
 * * @ingroup se_group
 *@{
 */


enum class Predefined_NL_SE
{
  Undef,             //!< Undefined predefined nl
  SESegmentX1D,      //!< 1D segment SE
  SESquare2D,        //!< 2D Square SE
  SECross2D,         //!< 2D Cross SE
  SESegmentX2D,      //!< 2D Horizontal SE
  SESegmentY2D,      //!< 2D Vertical SE
  SESegmentX3D,      //!< 3D SE along X
  SESegmentY3D,      //!< 3D SE along Y
  SESegmentZ3D,      //!< 3D SE along Z
  SECross3D,         //!< 3D SE (C6)
  SESquare3D,        //!< 3D SE (C26)
};

/*!@brief Square structuring element (2D)
 * Square rigid structuring element (2D)
 *
 * x x x  @n
 * x x x  @n
 * x x x  @n
 *
 * @remark This se is the unit ball for \f$L_\infty\f$ distance on 2D
 */
SE_API extern const neighbor_list_t< 2 > SESquare2D;

/*!@brief Cross structuring element (2D)
 * Cross rigid structuring element (2D)
 *
 * . x .   @n
 * x x x   @n
 * . x .   @n
 *
 * @remark This se is the unit ball for \f$L_1\f$ distance on 2D
 */
SE_API extern const neighbor_list_t< 2 > SECross2D;

/*! Line structuring element / segment of unit size along X (1D)
 *
 * x x x
 *
 */
SE_API extern const neighbor_list_t< 1 > SESegmentX1D;

/*! Line structuring element / segment of unit size along X (2D)
 * . . .
 * x x x
 * . . .
 */
SE_API extern const neighbor_list_t< 2 > SESegmentX2D;

/*! Line structuring element / segment of unit size along Y (2D)
 *  . x . @n
 *  . x . @n
 *  . x . @n
 */
SE_API extern const neighbor_list_t< 2 > SESegmentY2D;

/*! Line structuring element / segment of unit size along X (3D)
 */
SE_API extern const neighbor_list_t< 3 > SESegmentX3D;

/*! Line structuring element / segment of unit size along Y (3D)
 */
SE_API extern const neighbor_list_t< 3 > SESegmentY3D;

/*! Line structuring element / segment of unit size along Z (3D)
 */
SE_API extern const neighbor_list_t< 3 > SESegmentZ3D;

/*!@brief Square structuring element (3D)
 * Square rigid structuring element (3D), having a thickness of 3 and is a SESquare2D along z axis
 *
 * @remark This se is the unit ball for \f$L_\infty\f$ distance on 3D
 */
SE_API extern const neighbor_list_t< 3 > SESquare3D;

/*!@brief Cross structuring element (3D)
 * Cross rigid structuring element (3D), having a thickness of 3 and is a SECross2D along z axis
 *
 * @remark This se is the unit ball for \f$L_1\f$ distance on 3D
 */
SE_API extern const neighbor_list_t< 3 > SECross3D;

//! @} doxygroup: poutre_se_nl_predefine_group
}// namespace poutre