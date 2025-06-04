
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   structuring_element.hpp
 * @author Thomas Retornaz
 * @brief  Define import/export for shared libraries
 *
 *
 */

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_structuring_element_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define SE_API MODULE_EXPORT
#define EXPIMP_TEMPLATE
#else
#define SE_API MODULE_IMPORT
#define EXPIMP_TEMPLATE extern
#endif// POUTRE_DLL_EXPORTS
#define SE_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define SE_API
#define SE_LOCAL
#endif// POUTRE_DLL

namespace poutre {
/**
 * @addtogroup poutre_se_group Structuring Element definitions and helpers
 *@{
 */
//! @} doxygroup: poutre_se_group
}// namespace poutre
