/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <catch2/catch_test_macros.hpp>
#include <poutre/base/registrar.hpp>
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

class Derived : public Base
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
