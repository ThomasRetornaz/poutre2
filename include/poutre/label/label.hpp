/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_LABEL_LABEL
#define INCLUDE_POUTRE_LABEL_LABEL

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_label_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define LAB_API MODULE_EXPORT
#else
#define LAB_API MODULE_IMPORT
#endif// poutre_label_EXPORTS
#define LAB_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define LAB_API
#define LAB_LOCAL
#endif// POUTRE_DYNAMIC

namespace poutre::label {
/**
 * @addtogroup poutre_label_group Labelling operators
 * @ingroup image_processing_group
 *@{
 */

//! @} doxygroup: poutre_label_group
}// namespace poutre::label


#endif /* INCLUDE_POUTRE_LABEL_LABEL */
