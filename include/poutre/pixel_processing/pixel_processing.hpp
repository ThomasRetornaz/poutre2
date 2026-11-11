/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_PIXEL_PROCESSING_PIXEL_PROCESSING
#define INCLUDE_POUTRE_PIXEL_PROCESSING_PIXEL_PROCESSING

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_pixel_processing_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define PP_API MODULE_EXPORT
#else
#define PP_API MODULE_IMPORT
#endif// poutre_pixel_processing_EXPORTS
#define PP_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define PP_API
#define PP_LOCAL
#endif// POUTRE_DYNAMIC

namespace poutre {
/**
 * @addtogroup poutre_pixel_processing_group Pixel processing operations on images
 *@{
 */
//! @} doxygroup: poutre_pixel_processing_group
}// namespace poutre


#endif /* INCLUDE_POUTRE_PIXEL_PROCESSING_PIXEL_PROCESSING */
