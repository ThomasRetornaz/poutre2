
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   mreconstruct.hpp
 * @author Thomas Retornaz
 * @brief  Geodesic reconstructions
 *
 *
 */

#include <poutre/base/config.hpp>
#include <poutre/base/types.hpp> //for ScalarTypeVariant
#include <poutre/base/image_interface.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>
#include <poutre/geodesy/geodesy.hpp>

namespace poutre::geo {
/**
 * @addtogroup poutre_geodesy_group
 *@{
 */

//! The h-minima transformation suppresses all minima whose depth is greater than a given threshold pvalue.
GEO_API void h_minima(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img);

//! The h-maxima transformation suppresses all maxima whose depth is greater than a given threshold pvalue.
GEO_API void h_maxima(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief TopHat White with contrast criteria
 * @code
 * ABS(imin - the h-maxima transformation).
 * @endcode
 */
GEO_API void h_convex(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief TopHat Black with contrast criteria
 * @code
 * ABS(the h-minima transformation - imin).
 * @endcode
 */
GEO_API void h_concave(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief Pseudo dynamic opening
*
* The pseudo-dynamic opening transform suppresses all maxima whose depth is under a given threshold level pvalue
* similar to H-maxima transformation but the remaining maxima don't lose their dynamic.
*/
GEO_API void dynamic_pseudo_opening(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief Pseudo dynamic closing
*
* The pseudo-dynamic closing transform suppresses all minima whose depth is under a given threshold level pvalue
* similar to H-maxima transformation but the remaining maxima don't lose their dynamic.
*/
GEO_API void dynamic_pseudo_closing(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static, IInterface &o_img);

//! @} doxygroup: poutre_geodesy_group
}
