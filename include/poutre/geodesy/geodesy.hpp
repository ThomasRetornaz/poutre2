
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   geodesy.hpp
 * @author Thomas Retornaz
 * @brief  Define import/export for shared libraries
 *
 *
 */

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_geodesy_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define GEO_API MODULE_EXPORT
#else
#define GEO_API MODULE_IMPORT
#endif// POUTRE_DLL_EXPORTS
#define GEO_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define GEO_API
#define GEO_LOCAL
#endif// POUTRE_DLL

namespace poutre::geo {
/**
 * @addtogroup poutre_geodesy_group Geodesic operators
 * @ingroup image_processing_group
 *@{
 */

//! @} doxygroup: poutre_geodesy_group
}// namespace poutre

