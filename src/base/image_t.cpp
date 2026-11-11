/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/


#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/types.hpp>

namespace poutre::details {
#if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
template class image_t< poutre::pUINT8, 1 >;
template class image_t< poutre::pINT32, 1 >;
template class image_t< poutre::pFLOAT, 1 >;
template class image_t< poutre::pINT64, 1 >;
template class image_t< poutre::pDOUBLE, 1 >;

template class image_t< poutre::pUINT8, 2 >;
template class image_t< poutre::pINT32, 2 >;
template class image_t< poutre::pFLOAT, 2 >;
template class image_t< poutre::pINT64, 2 >;
template class image_t< poutre::pDOUBLE, 2 >;

template class image_t< compound_type< poutre::pUINT8, 3 >, 2 >;
template class image_t< compound_type< poutre::pINT32, 3 >, 2 >;
template class image_t< compound_type< poutre::pFLOAT, 3 >, 2 >;
template class image_t< compound_type< poutre::pINT64, 3 >, 2 >;
template class image_t< compound_type< poutre::pDOUBLE, 3 >, 2 >;

template class image_t< compound_type< poutre::pUINT8, 4 >, 2 >;
template class image_t< compound_type< poutre::pINT32, 4 >, 2 >;
template class image_t< compound_type< poutre::pFLOAT, 4 >, 2 >;
template class image_t< compound_type< poutre::pINT64, 4 >, 2 >;
template class image_t< compound_type< poutre::pDOUBLE, 4 >, 2 >;

template class image_t< poutre::pUINT8, 3 >;
template class image_t< poutre::pINT32, 3 >;
template class image_t< poutre::pFLOAT, 3 >;
template class image_t< poutre::pINT64, 3 >;
template class image_t< poutre::pDOUBLE, 3 >;

template class image_t< pUINT8, 4 >;
template class image_t< pINT32, 4 >;
template class image_t< pFLOAT, 4 >;
template class image_t< pINT64, 4 >;
template class image_t< pDOUBLE, 4 >;
#else

template class image_t< poutre::pUINT8, 1 >;
template class image_t< poutre::pINT32, 1 >;
template class image_t< poutre::pFLOAT, 1 >;
template class image_t< poutre::pINT64, 1 >;
template class image_t< poutre::pDOUBLE, 1 >;

template class image_t< poutre::pUINT8, 2 >;
template class image_t< poutre::pINT32, 2 >;
template class image_t< poutre::pFLOAT, 2 >;
template class image_t< poutre::pINT64, 2 >;
template class image_t< poutre::pDOUBLE, 2 >;

template class image_t< compound_type< poutre::pUINT8, 3 >, 2 >;
template class image_t< compound_type< poutre::pINT32, 3 >, 2 >;
template class image_t< compound_type< poutre::pFLOAT, 3 >, 2 >;
template class image_t< compound_type< poutre::pINT64, 3 >, 2 >;
template class image_t< compound_type< poutre::pDOUBLE, 3 >, 2 >;

template class image_t< compound_type< poutre::pUINT8, 4 >, 2 >;
template class image_t< compound_type< poutre::pINT32, 4 >, 2 >;
template class image_t< compound_type< poutre::pFLOAT, 4 >, 2 >;
template class image_t< compound_type< poutre::pINT64, 4 >, 2 >;
template class image_t< compound_type< poutre::pDOUBLE, 4 >, 2 >;

template class image_t< poutre::pUINT8, 3 >;
template class image_t< poutre::pINT32, 3 >;
template class image_t< poutre::pFLOAT, 3 >;
template class image_t< poutre::pINT64, 3 >;
template class image_t< poutre::pDOUBLE, 3 >;

template class image_t< pUINT8, 4 >;
template class image_t< pINT32, 4 >;
template class image_t< pFLOAT, 4 >;
template class image_t< pINT64, 4 >;
template class image_t< pDOUBLE, 4 >;
#endif

}// namespace poutre::details