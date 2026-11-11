/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/


#ifndef INCLUDE_POUTRE_GEODESY_EXTREMA
#define INCLUDE_POUTRE_GEODESY_EXTREMA

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>//for ScalarTypeVariant
#include <poutre/geodesy/geodesy.hpp>
#include <poutre/structuring_element/se_types_and_tags.hpp>

namespace poutre::geo {
/**
 * @addtogroup poutre_geodesy_group
 *@{
 */

//! The h-minima transformation suppresses all minima whose depth is greater than a given threshold pvalue.
GEO_API void h_minima(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static,
                      IInterface &o_img);

//! The h-maxima transformation suppresses all maxima whose depth is greater than a given threshold pvalue.
GEO_API void h_maxima(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static,
                      IInterface &o_img);

/*!@brief TopHat White with contrast criteria
 * @code
 * ABS(imin - the h-maxima transformation).
 * @endcode
 */
GEO_API void h_convex(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static,
                      IInterface &o_img);

/*!@brief TopHat Black with contrast criteria
 * @code
 * ABS(the h-minima transformation - imin).
 * @endcode
 */
GEO_API void h_concave(const IInterface &i_img, const ScalarTypeVariant &pvalue, se::Common_NL_SE nl_static,
                       IInterface &o_img);

/*!@brief Pseudo dynamic opening
 *
 * The pseudo-dynamic opening transform suppresses all maxima whose depth is under a given threshold level pvalue
 * similar to H-maxima transformation but the remaining maxima don't lose their dynamic.
 */
GEO_API void dynamic_pseudo_opening(const IInterface &i_img, const ScalarTypeVariant &pvalue,
                                    se::Common_NL_SE nl_static, IInterface &o_img);

/*!@brief Pseudo dynamic closing
 *
 * The pseudo-dynamic closing transform suppresses all minima whose depth is under a given threshold level pvalue
 * similar to H-maxima transformation but the remaining maxima don't lose their dynamic.
 */
GEO_API void dynamic_pseudo_closing(const IInterface &i_img, const ScalarTypeVariant &pvalue,
                                    se::Common_NL_SE nl_static, IInterface &o_img);

//! @} doxygroup: poutre_geodesy_group
}// namespace poutre::geo


#endif /* INCLUDE_POUTRE_GEODESY_EXTREMA */
