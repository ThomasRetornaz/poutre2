
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   pixel_processing.hpp
 * @author Thomas Retornaz
 * @brief  Define import/export for shared libraries
 *
 *
 */

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_pixel_processing_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define PP_API MODULE_EXPORT
#else
#define PP_API MODULE_IMPORT
#endif// POUTRE_DLL_EXPORTS
#define PP_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define PP_API
#define PP_LOCAL
#endif// POUTRE_DLL

namespace poutre {
/**
 * @addtogroup poutre_pixel_processing_group Pixel processing operations on images
 *@{
 */
//! @} doxygroup: poutre_pixel_processing_group
}// namespace poutre