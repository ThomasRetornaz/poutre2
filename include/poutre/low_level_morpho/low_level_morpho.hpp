/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef INCLUDE_POUTRE_LOW_LEVEL_MORPHO_LOW_LEVEL_MORPHO
#define INCLUDE_POUTRE_LOW_LEVEL_MORPHO_LOW_LEVEL_MORPHO

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_low_level_morpho_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define LLM_API MODULE_EXPORT
#define LLM_TEMPLATE
#else
#define LLM_API MODULE_IMPORT
#define LLM_TEMPLATE extern
#endif// poutre_low_level_morpho_EXPORTS
#define LLM_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define LLM_API
#define LLM_LOCAL
#endif// POUTRE_DYNAMIC

namespace poutre {
/**
 * @addtogroup poutre_llm_group Basics low level operator eg erode, dilate, open, close, ...
 *@{
 */
//! @} doxygroup: poutre_llm_group
}// namespace poutre


#endif /* INCLUDE_POUTRE_LOW_LEVEL_MORPHO_LOW_LEVEL_MORPHO */
