/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_DISTANCE_DISTANCE
#define INCLUDE_POUTRE_DISTANCE_DISTANCE

/**
 * @file   distance.hpp
 * @author Thomas Retornaz
 * @brief  Define import/export for shared libraries
 *
 *
 */

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_distance_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define DIST_API MODULE_EXPORT
#else
#define DIST_API MODULE_IMPORT
#endif// POUTRE_DLL_EXPORTS
#define DIST_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define DIST_API
#define DIST_LOCAL
#endif// POUTRE_DLL

namespace poutre::dist {
/**
 * @addtogroup poutre_distance_group Distance transform
 * @ingroup image_processing_group
 *@{
 */

//! @} doxygroup: poutre_distance_group
}// namespace poutre::dist


#endif /* INCLUDE_POUTRE_DISTANCE_DISTANCE */
