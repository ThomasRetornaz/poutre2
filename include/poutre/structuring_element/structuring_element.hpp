/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
 
#ifndef INCLUDE_POUTRE_STRUCTURING_ELEMENT_STRUCTURING_ELEMENT
#define INCLUDE_POUTRE_STRUCTURING_ELEMENT_STRUCTURING_ELEMENT

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_structuring_element_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define SE_API MODULE_EXPORT
#else
#define SE_API MODULE_IMPORT
#endif// poutre_structuring_element_EXPORTS
#define SE_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define SE_API
#define SE_LOCAL
#endif// POUTRE_DYNAMIC

namespace poutre {
/**
 * @addtogroup poutre_se_group Structuring Element definitions and helpers
 *@{
 */
//! @} doxygroup: poutre_se_group
}// namespace poutre


#endif /* INCLUDE_POUTRE_STRUCTURING_ELEMENT_STRUCTURING_ELEMENT */
