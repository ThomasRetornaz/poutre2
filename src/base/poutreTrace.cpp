// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/poutreTrace.hpp>

#include <ostream>

namespace poutre {
std::ostream &operator<<(std::ostream &ost, GlobalLogger::LoggerLevel level)// NOLINT(misc-unused-parameters)
{
  switch (level) {
  case GlobalLogger::LoggerLevel::trace:
    ost << "trace";
    break;
  case GlobalLogger::LoggerLevel::critical:
    ost << "critical";
    break;
  case GlobalLogger::LoggerLevel::debug:
    ost << "debug";
    break;
  case GlobalLogger::LoggerLevel::err:
    ost << "err";
    break;
  case GlobalLogger::LoggerLevel::info:
    ost << "info";
    break;
  case GlobalLogger::LoggerLevel::warn:
    ost << "warn";
    break;
  default:
    ost << "Unknown GlobalLogger::LoggerLevel";
    break;
  }
  return ost;
}
}// namespace poutre