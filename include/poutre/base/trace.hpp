
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   trace.hpp
 * @author Thomas Retornaz
 * @brief  Trace capabilities
 *
 *
 */

#include <poutre/base/base.hpp>
#include <poutre/base/config.hpp>

#include <fstream>
#include <ostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/common.h>


namespace poutre {
/**
 * @addtogroup trace_group Trace facilities
 * @ingroup poutre_base_group
 *@{
 */
class BASE_API GlobalLogger
{
public:
  enum class LoggerLevel {
    trace,
    debug,
    info,
    warn,
    err,
    critical
    /* off ,
     n_levels*/
  };
  static GlobalLogger &get()
  {
    static GlobalLogger instance;
    // Instantiated on first use.
    return instance;
  }

  GlobalLogger(const GlobalLogger &) = delete;
  GlobalLogger &operator=(const GlobalLogger &) = delete;
  GlobalLogger(GlobalLogger &&other) = delete;
  GlobalLogger &operator=(GlobalLogger &&other) = delete;
  ~GlobalLogger();

  void setLoggerLevel(LoggerLevel l)
  {
    switch (l) {
    case LoggerLevel::debug:
      (*m_innerlogger).set_level(spdlog::level::debug);
      break;
    case LoggerLevel::info:
      (*m_innerlogger).set_level(spdlog::level::info);
      break;
    case LoggerLevel::critical:
      (*m_innerlogger).set_level(spdlog::level::critical);
      break;
    case LoggerLevel::err:
      (*m_innerlogger).set_level(spdlog::level::err);
      break;
    case LoggerLevel::trace:
      (*m_innerlogger).set_level(spdlog::level::trace);
      break;
    case LoggerLevel::warn:
      (*m_innerlogger).set_level(spdlog::level::warn);
      break;
    default:
      POUTRE_RUNTIME_ERROR("Logger unsupported enum case");
    }
  }
  void call(const char *MSG, LoggerLevel l = LoggerLevel::debug)
  {
    switch (l) {
    case LoggerLevel::debug:
      m_innerlogger->debug(MSG);
      break;
    case LoggerLevel::info:
      m_innerlogger->info(MSG);
      break;
    case LoggerLevel::critical:
      m_innerlogger->critical(MSG);
      break;
    case LoggerLevel::err:
      m_innerlogger->error(MSG);
      break;
    case LoggerLevel::trace:
      m_innerlogger->trace(MSG);
      break;
    case LoggerLevel::warn:
      m_innerlogger->warn(MSG);
      break;
    default:
      POUTRE_RUNTIME_ERROR("Logger unsupported enum case");
    }
  }

private:
  std::shared_ptr<spdlog::logger> m_innerlogger;
  //! Private ctor
  GlobalLogger();
};

//! operator<< for GlobalLogger::LoggerLevel
BASE_API std::ostream &operator<<(std::ostream &, GlobalLogger::LoggerLevel level);

/** @name Global logger macros
 */
/**@{*/
// NOLINTNEXTLINE
#define POUTRE_DEBUG(MSG)           \
  poutre::GlobalLogger::get().call( \
    std::format("DEBUG: {} at {} {}", MSG, __FILE__, __LINE__).c_str(), GlobalLogger::LoggerLevel::debug)
// NOLINTNEXTLINE
#define POUTRE_INFO(MSG)            \
  poutre::GlobalLogger::get().call( \
    std::format("INFO: {} at {} {}", MSG, __FILE__, __LINE__).c_str(), GlobalLogger::LoggerLevel::info)
// NOLINTNEXTLINE
#define POUTRE_CRITICAL(MSG)        \
  poutre::GlobalLogger::get().call( \
    std::format("CRITICAL: {} at {} {}", MSG, __FILE__, __LINE__).c_str(), GlobalLogger::LoggerLevel::critical)
// NOLINTNEXTLINE
#define POUTRE_ERROR(MSG)           \
  poutre::GlobalLogger::get().call( \
    std::format("ERROR: {} at {} {}", MSG, __FILE__, __LINE__).c_str(), GlobalLogger::LoggerLevel::err)
// NOLINTNEXTLINE
#define POUTRE_TRACE(MSG)           \
  poutre::GlobalLogger::get().call( \
    std::format("TRACE: {} at {} {}", MSG, __FILE__, __LINE__).c_str(), GlobalLogger::LoggerLevel::trace)
// NOLINTNEXTLINE
#define POUTRE_WARN(MSG)            \
  poutre::GlobalLogger::get().call( \
    std::format("WARNING: {} at {} {}", MSG, __FILE__, __LINE__).c_str(), GlobalLogger::LoggerLevel::warn)
// NOLINTNEXTLINE
#define POUTRE_ENTERING(MSG)        \
  poutre::GlobalLogger::get().call( \
    std::format("Entering: {} at {} {}", MSG, __FILE__, __LINE__).c_str(), GlobalLogger::LoggerLevel::trace)
/**@}*/

// !@} doxygroup: trace_group

}// namespace poutre
