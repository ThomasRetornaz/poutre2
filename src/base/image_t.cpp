// This is an open source non-commercial project. Dear PVS-Studio, please check
// it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================


#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/types.hpp>

namespace poutre::details
{

template class image_t<poutre::pUINT8, 1>;
template class image_t<poutre::pINT32, 1>;
template class image_t<poutre::pFLOAT, 1>;
template class image_t<poutre::pINT64, 1>;
template class image_t<poutre::pDOUBLE, 1>;

template class image_t<poutre::pUINT8, 2>;
template class image_t<poutre::pINT32, 2>;
template class image_t<poutre::pFLOAT, 2>;
template class image_t<poutre::pINT64, 2>;
template class image_t<poutre::pDOUBLE, 2>;

template class image_t<compound_type<poutre::pUINT8, 3>, 2>;
template class image_t<compound_type<poutre::pINT32, 3>, 2>;
template class image_t<compound_type<poutre::pFLOAT, 3>, 2>;
template class image_t<compound_type<poutre::pINT64, 3>, 2>;
template class image_t<compound_type<poutre::pDOUBLE, 3>, 2>;

template class image_t<compound_type<poutre::pUINT8, 4>, 2>;
template class image_t<compound_type<poutre::pINT32, 4>, 2>;
template class image_t<compound_type<poutre::pFLOAT, 4>, 2>;
template class image_t<compound_type<poutre::pINT64, 4>, 2>;
template class image_t<compound_type<poutre::pDOUBLE, 4>, 2>;

template class image_t<poutre::pUINT8, 3>;
template class image_t<poutre::pINT32, 3>;
template class image_t<poutre::pFLOAT, 3>;
template class image_t<poutre::pINT64, 3>;
template class image_t<poutre::pDOUBLE, 3>;

template class image_t<pUINT8, 4>;
template class image_t<pINT32, 4>;
template class image_t<pFLOAT, 4>;
template class image_t<pINT64, 4>;
template class image_t<pDOUBLE, 4>;

}