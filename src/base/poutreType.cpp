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
#include <poutre/base/poutreConfig.hpp>
#include <poutre/base/poutreTypes.hpp>
#include <string>

namespace poutre {
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

}// namespace poutre
