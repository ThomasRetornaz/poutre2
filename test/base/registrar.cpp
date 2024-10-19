// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <catch2/catch_test_macros.hpp>
#include <poutre/base/poutreRegistrar.hpp>
#include <string>

class Base
{
public:
  Base() = default;
  virtual ~Base() = default;

  Base(const Base &) = default;
  Base &operator=(const Base &) = default;
  Base(const Base &&) = delete;
  Base &operator=(const Base &&) = delete;

  // static std::unique_ptr<Base> Create() { return std::make_unique<Base>(); }
  static Base *Create() { return new Base(); }// NOLINT

  virtual std::string WhoIam() { return "Base"; }
};

class Derived : Base
{
public:
  Derived() = default;
  ~Derived() override = default;
  Derived(const Derived &) = default;
  Derived &operator=(const Derived &) = default;
  Derived(const Derived &&) = delete;
  Derived &operator=(const Derived &&) = delete;

  // static std::unique_ptr<Base> Create() { return std::make_unique<Derived>(); }
  // cppcheck-suppress duplInheritedMember
  static Base *Create() { return new Derived(); }// NOLINT
  std::string WhoIam() override { return "Derived"; }
};

TEST_CASE("default", "[registrar]")
{
  poutre::Registrar<Base>::getInstance().Register("Base", Base::Create);
  poutre::Registrar<Base>::getInstance().Register("Derived", Derived::Create);
}
