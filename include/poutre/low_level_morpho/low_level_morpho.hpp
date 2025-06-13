
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   low_level_morpho.hpp
 * @author Thomas Retornaz
 * @brief  Common low level operators : Erode, dilate, open, close
 *
 *
 */

#include <poutre/base/config.hpp>

#ifdef POUTRE_DYNAMIC// defined if POUTRE is compiled as a DLL
#ifdef poutre_low_level_morpho_EXPORTS// defined if we are building the POUTRE DLL (instead of using it)
#define LLM_API MODULE_EXPORT
#define LLM_TEMPLATE
#else
#define LLM_API MODULE_IMPORT
#define LLM_TEMPLATE extern
#endif// POUTRE_DLL_EXPORTS
#define LLM_LOCAL MODULE_LOCAL
#else// POUTRE_DLL is not defined: this means POUTRE is a static lib.
#define LLM_API
#define LLM_LOCAL
#endif// POUTRE_DLL

namespace poutre {
/**
 * @addtogroup poutre_llm_group Basics low level operator eg erode, dilate, open, close, ...
 *@{
 */
//! @} doxygroup: poutre_llm_group
}// namespace poutre

