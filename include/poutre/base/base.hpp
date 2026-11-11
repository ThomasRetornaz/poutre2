/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef INCLUDE_POUTRE_BASE_BASE
#define INCLUDE_POUTRE_BASE_BASE

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_base_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define BASE_API MODULE_EXPORT
#define EXPIMP_TEMPLATE
#else
#define BASE_API MODULE_IMPORT
#define EXPIMP_TEMPLATE extern
#endif// poutre_base_EXPORTS
#define BASE_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define BASE_API
#define BASE_LOCAL
#endif// POUTRE_DYNAMIC

namespace poutre {
/**
 * @addtogroup poutre_base_group Base/Common objects and types
 *@{
 */
//! @} doxygroup: poutre_base_group
}// namespace poutre


#endif /* INCLUDE_POUTRE_BASE_BASE */
