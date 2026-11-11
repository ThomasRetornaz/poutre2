/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/


#ifndef INCLUDE_POUTRE_GEODESY_GEODESY
#define INCLUDE_POUTRE_GEODESY_GEODESY

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_geodesy_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define GEO_API MODULE_EXPORT
#else
#define GEO_API MODULE_IMPORT
#endif// poutre_geodesy_EXPORTS
#define GEO_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define GEO_API
#define GEO_LOCAL
#endif// POUTRE_DYNAMIC

namespace poutre::geo {
/**
 * @addtogroup poutre_geodesy_group Geodesic operators
 * @ingroup image_processing_group
 *@{
 */

//! @} doxygroup: poutre_geodesy_group
}// namespace poutre::geo


#endif /* INCLUDE_POUTRE_GEODESY_GEODESY */
