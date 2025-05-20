// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <istream>
#include <ostream>
#include <string>

#include <poutre/base/config.hpp>
#include <poutre/base/types.hpp>

namespace poutre {

template class compound_type<pUINT8, 3>;
template class compound_type<pINT32, 3>;
template class compound_type<pFLOAT, 3>;
template class compound_type<pINT64, 3>;
template class compound_type<pDOUBLE, 3>;

template class compound_type<pUINT8, 4>;
template class compound_type<pINT32, 4>;
template class compound_type<pFLOAT, 4>;
template class compound_type<pINT64, 4>;
template class compound_type<pDOUBLE, 4>;

// NOTE JUST FOR CONVENIENCE AT INTERFACE LAYER TO USE STRING INSTEAD OF VERBOSE ENUM

std::ostream &operator<<(std::ostream &ost, CompOpType compOpType)// NOLINT(misc-unused-parameters)
{
  switch (compOpType) {
  case CompOpType::CompOpDiff:
    ost << "!=";
    break;
  case CompOpType::CompOpEqual:
    ost << "==";
    break;
  case CompOpType::CompOpInf:
    ost << "<";
    break;
  case CompOpType::CompOpInfEqual:
    ost << "<=";
    break;
  case CompOpType::CompOpSup:
    ost << ">";
    break;
  case CompOpType::CompOpSupEqual:
    ost << ">=";
    break;
  default://  CompOpType::CompOpUndef;
    ost << "Unknown CompOpType";
    break;
  }
  return ost;
}

std::istream &operator>>(std::istream &ist, CompOpType &compOpType)// NOLINT(misc-unused-parameters)
{
  compOpType = CompOpType::CompOpUndef;

  if (!ist.good()) { return ist; }

  std::string strType;
  ist >> strType;
  if (ist.bad()) { return ist; }

  if (strType == "!=") {
    compOpType = CompOpType::CompOpDiff;
  } else if (strType == "==") {
    compOpType = CompOpType::CompOpEqual;
  } else if (strType == "<") {
    compOpType = CompOpType::CompOpInf;
  } else if (strType == ">") {
    compOpType = CompOpType::CompOpSup;
  } else if (strType == "<=") {
    compOpType = CompOpType::CompOpInfEqual;
  } else if (strType == ">=") {
    compOpType = CompOpType::CompOpSupEqual;
  } else {
    POUTRE_RUNTIME_ERROR("Unable to read CompOpType from stream");
  }
  return ist;
}

std::ostream &operator<<(std::ostream &ost, ArithOpType arithOpType)// NOLINT(misc-unused-parameters)
{
  switch (arithOpType) {
  case ArithOpType::ArithOpPlus:
    ost << "+";
    break;
  case ArithOpType::ArithOpMinus:
    ost << "-";
    break;
  case ArithOpType::ArithOpMul:
    ost << "*";
    break;
  case ArithOpType::ArithOpDiv:
    ost << "/";
    break;
  case ArithOpType::ArithOpMod:
    ost << "%";
    break;
  default://  ArithOpType::;
    ost << "Unknown ArithOpType";
    break;
  }
  return ost;
}

std::istream &operator>>(std::istream &ist, ArithOpType &arithOpType)// NOLINT(misc-unused-parameters)
{
  arithOpType = ArithOpType::ArithOpUndef;

  if (!ist.good()) { return ist; }

  std::string strType;
  ist >> strType;
  if (ist.bad()) { return ist; }

  if (strType == "+") {
    arithOpType = ArithOpType::ArithOpPlus;
  } else if (strType == "-") {
    arithOpType = ArithOpType::ArithOpMinus;
  } else if (strType == "/") {
    arithOpType = ArithOpType::ArithOpDiv;
  } else if (strType == "%") {
    arithOpType = ArithOpType::ArithOpMod;
  } else if (strType == "*") {
    arithOpType = ArithOpType::ArithOpMul;
  } else {
    POUTRE_RUNTIME_ERROR("Unable to read ArithOpType from stream");
  }
  return ist;
}

std::ostream &operator<<(std::ostream &ost, AssignOpType assignOpType)// NOLINT(misc-unused-parameters)
{
  switch (assignOpType) {
  case AssignOpType::AssignOpAdd:
    ost << "+=";
    break;
  case AssignOpType::AssignOpMinus:
    ost << "-=";
    break;
  case AssignOpType::AssignOpMul:
    ost << "*=";
    break;
  case AssignOpType::AssignOpDiv:
    ost << "/=";
    break;
  case AssignOpType::AssignOpMod:
    ost << "%=";
    break;
  case AssignOpType::AssignOp:
    ost << "=";
    break;
  default://  AssignOpType::;
    ost << "Unknown AssignOpType";
    break;
  }
  return ost;
}

std::istream &operator>>(std::istream &ist, AssignOpType &assignOpType)// NOLINT(misc-unused-parameters)
{
  assignOpType = AssignOpType::AssignOpUndef;

  if (!ist.good()) { return ist; }

  std::string strType;
  ist >> strType;
  if (ist.bad()) { return ist; }

  if (strType == "+=") {
    assignOpType = AssignOpType::AssignOpAdd;
  } else if (strType == "-=") {
    assignOpType = AssignOpType::AssignOpMinus;
  } else if (strType == "*=") {
    assignOpType = AssignOpType::AssignOpMul;
  } else if (strType == "/=") {
    assignOpType = AssignOpType::AssignOpDiv;
  } else if (strType == "%=") {
    assignOpType = AssignOpType::AssignOpMod;
  } else if (strType == "=") {
    assignOpType = AssignOpType::AssignOp;
  } else {
    POUTRE_RUNTIME_ERROR("Unable to read AssignOpType from stream");
  }
  return ist;
}

std::ostream &operator<<(std::ostream &ost, CompoundType ctype)
{
  switch (ctype) {
  case CompoundType::CompoundType_Scalar: {
    ost << "Scalar";
  } break;
  case CompoundType::CompoundType_3Planes: {
    ost << "3Planes";
  } break;
  case CompoundType::CompoundType_4Planes: {
    ost << "4Planes";
  } break;
    /* case CompoundType::CompoundType_Container:
       {
       os << "Container";
       } break;*/
  default://  CompoundType::CompoundType_Undef;
    ost << "Unknown PType";
  }// switch
  return ost;
}

std::istream &operator>>(std::istream &ist, CompoundType &ctype)// NOLINT(misc-unused-parameters)
{
  ctype = CompoundType::CompoundType_Undef;
  if (!ist.good()) { return ist; }

  std::string strType{};
  ist >> strType;
  if (ist.bad()) {
    return ist;
  }

  else if (strType == "Scalar") {
    ctype = CompoundType::CompoundType_Scalar;
  } else if (strType == "3Planes") {
    ctype = CompoundType::CompoundType_3Planes;
  } else if (strType == "4Planes") {
    ctype = CompoundType::CompoundType_4Planes;
  }
  /* else if (strType == "Container")
     ct = CompoundType::CompoundType_Container;*/
  else {
    POUTRE_RUNTIME_ERROR("Unable to read CompoundType from stream");
  }
  return ist;
}

// std::ostream &operator<<(std::ostream &ost, ImgType imgType)
// {
//   switch (imgType) {
//   case ImgType::ImgType_Dense:
//     ost << "Dense";
//     break;
//   case ImgType::ImgType_Sparse:
//     ost << "Sparse";
//     break;
//   default://  ImgType::ImgType_Undef;
//     ost << "Unknown ImgType";
//     break;
//   }
//   return ost;
// }

// std::istream &operator>>(std::istream &ist, ImgType &imgType)// NOLINT(misc-unused-parameters)
// {
//   imgType = ImgType::ImgType_Undef;

//   if (!ist.good()) { return ist; }

//   std::string strType{};
//   ist >> strType;
//   if (ist.bad()) {
//     return ist;
//   }

//   else if (strType == "Dense") {
//     imgType = ImgType::ImgType_Dense;
//   } else if (strType == "Sparse") {
//     imgType = ImgType::ImgType_Sparse;
//   } else {
//     POUTRE_RUNTIME_ERROR("Unable to read ImgType from stream");
//   }
//   return ist;
// }

std::ostream &operator<<(std::ostream &ost, PType ptype)// NOLINT(misc-unused-parameters)
{
  switch (ptype) {
    /* case PType::PType_Bin:
       os << "Bin";
       break;*/
    /*      case PType_BinPack: // break;*/// TODO ?
  case PType::PType_GrayUINT8:
    ost << "GUINT8";
    break;
  case PType::PType_GrayINT32:
    ost << "GINT32";
    break;
  case PType::PType_GrayINT64:
    ost << "GINT64";
    break;
  case PType::PType_F32:
    ost << "F32";
    break;
  case PType::PType_D64:
    ost << "D64";
    break;
  default://  PType::PType_Undef;
    ost << "Unknown PType";
    break;
  }

  return ost;
}

std::istream &operator>>(std::istream &ist, PType &ptype)// NOLINT(misc-unused-parameters)
{
  ptype = PType::PType_Undef;

  if (!ist.good()) { return ist; }

  std::string strType;
  ist >> strType;
  if (ist.bad()) {
    return ist;
  }

  /* if (strType == "Bin")
     p = PType::PType_Bin;*/
  /*      case PType_BinPack: // break;*/// TODO ?
  else if (strType == "GUINT8") {
    ptype = PType::PType_GrayUINT8;
  } else if (strType == "GINT32") {
    ptype = PType::PType_GrayINT32;
  } else if (strType == "GINT64") {
    ptype = PType::PType_GrayINT64;
  } else if (strType == "F32") {
    ptype = PType::PType_F32;
  } else if (strType == "D64") {
    ptype = PType::PType_D64;
  } else {
    POUTRE_RUNTIME_ERROR("Unable to read PType from stream");
  }
  return ist;
}
}// namespace poutre
