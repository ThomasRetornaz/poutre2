// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <cstring>
#include <json/value.h>
#include <poutre/base/json.hpp>
#include <string>
#include <vector>

class TestClassA : public poutre::InterfaceJsonSerializable// NOLINT
{
public:
  TestClassA();
  ~TestClassA() override;
  void Serialize(Json::Value &root) override;
  void Deserialize(Json::Value &root) override;

private:
  int m_nTestInt{};
  double m_fTestFloat{};
  std::string m_TestString;
  bool m_bTestBool{};
  std::vector<float> m_array;
};

TestClassA::TestClassA()
  : m_nTestInt(42), m_fTestFloat(3.14159), m_TestString("foo"), m_bTestBool(true),// NOLINT
    m_array({ 1.1f, 2.1f, 3.1f })// NOLINT
{}
TestClassA::~TestClassA() = default;

void TestClassA::Deserialize(Json::Value &root)
{
  // deserialize primitives
  m_nTestInt = root.get("m_nTestInt", 0).asInt();
  m_fTestFloat = root.get("m_fTestFloat", 0.0).asDouble();
  m_TestString = root.get("m_TestString", "").asString();
  m_bTestBool = root.get("m_bTestBool", false).asBool();
  const Json::Value &jsonarray = root.get("m_array", Json::Value(Json::arrayValue));
  m_array.clear();
  m_array.reserve(jsonarray.size());
  // cppcheck-suppress useStlAlgorithm
  for (const auto &val : jsonarray) { m_array.emplace_back(val.asFloat()); }
}

void TestClassA::Serialize(Json::Value &root)
{
  // serialize primitives
  root["m_nTestInt"] = m_nTestInt;
  root["m_fTestFloat"] = m_fTestFloat;
  root["m_TestString"] = m_TestString;
  root["m_bTestBool"] = m_bTestBool;
  // const Json::Value& array = root["m_array"];
  for (auto var : m_array) { root["m_array"].append(static_cast<double>(var)).asFloat(); }
}

TEST_CASE("serializer", "[json]")
{
  TestClassA testClass;
  std::string output;
  poutre::JsonSerializer::Serialize(&testClass, output);
  std::string expected =
    "\
{\n\
\"m_TestString\" : \"foo\",\n\
\"m_array\" : [\n1.1000000238418579,\n2.0999999046325684,\n3.0999999046325684\n],\n\
\"m_bTestBool\" : true,\n\
\"m_fTestFloat\" : 3.1415899999999999,\n\
\"m_nTestInt\" : 42\n\
}\n";
  auto rm_space = [](const char character) { return isspace(character); };

  const auto [first, last] = std::ranges::remove_if(output, rm_space);
  output.erase(first, last);
  const auto [first_exp, last_exp] = std::ranges::remove_if(expected, rm_space);
  expected.erase(first_exp, last_exp);

  REQUIRE(std::strcmp(expected.c_str(), output.c_str()) == 0);
}
