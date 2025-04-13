
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file   registrar.hpp
 * @author Thomas Retornaz
 * @brief Define static objetc which store map to factories
 *
 *
 */

#include <poutre/base/base.hpp>

// #ifndef POUTRE_TRACE__HPP__
// #include <poutreBase/poutreTrace.hpp>
// #endif

#include <functional>
#include <string>
#include <unordered_map>

namespace poutre {
/*!
 * @defgroup register_group Register factories of image operations
 * @ingroup poutre_base_group
 * @{
 */
/**
 * @brief Allow to register a set of factories regarding provided type
 * @note use singleton pattern with lazy construct
 */
template<class Interface> class Registrar
{
public:
  using KeyFactoryMap = std::unordered_map<std::string, std::function<Interface *(void)>>;
  // typedef std::unordered_map< std::string,
  // std::function<std::unique_ptr<Interface>(void)>>
  // KeyFactoryMap;

  /** @name Singleton interface
   */
  //! Get concrete instance (lazy construct)
  static Registrar &getInstance()
  {
    static Registrar instance;// Guaranteed to be destroyed. AND
                              // http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
    // Instantiated on first use.
    return instance;
  }

  Registrar(const Registrar &) = delete;
  Registrar &operator=(const Registrar &) = delete;
  Registrar(Registrar &&other) = delete;
  Registrar &operator=(Registrar &&other) = delete;

  ~Registrar() = default;

private:
  //! Private ctor
  Registrar() = default;
  /**@}*/

  KeyFactoryMap m_keyfactory_map;//! container which store factories
public:
  // void Register(const std::string& key,
  // std::function<std::unique_ptr<Interface>(void)>
  // ImplFactoryFunction)

  /**
   * @brief Register a factory
   *
   * @param key name associated with factory
   * @param ImplFactoryFunction factory has ptr to function
   * @throw runtime_error if key already used
   */
  void Register(const std::string &key, std::function<Interface *(void)> ImplFactoryFunction)
  {
    auto iter = m_keyfactory_map.find(key);
    if (iter != m_keyfactory_map.end()) { POUTRE_RUNTIME_ERROR("Registrar::Add key already used"); }
    m_keyfactory_map[key] = ImplFactoryFunction;
  }
};
/**
//! @} doxygroup: register_group
*/
}// namespace poutre
