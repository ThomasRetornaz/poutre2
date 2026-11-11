/****************************************************************************
 * Copyright (c) Thomas Retornaz                                            *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <cstring>
//#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

struct TestClassA
{
  TestClassA();
  int m_nTestInt{};
  double m_fTestFloat{};
  std::string m_TestString;
  bool m_bTestBool{};
  std::vector<double> m_array;
};

void to_json(json& jso, const TestClassA& test) {
  jso = json{
    {"int", test.m_nTestInt},
    {"double",test.m_fTestFloat},
    {"string",test.m_TestString},
    {"bool", test.m_bTestBool},
    {"array", test.m_array} };
}

  void from_json(const json& jvalue, TestClassA& test) {
  jvalue.at("int").get_to(test.m_nTestInt);
  jvalue.at("double").get_to(test.m_fTestFloat);
  jvalue.at("string").get_to(test.m_TestString);
  jvalue.at("bool").get_to(test.m_bTestBool);
  jvalue.at("array").get_to(test.m_array);
  }


TestClassA::TestClassA()
  : m_nTestInt(42), m_fTestFloat(3.14159), m_TestString("foo"), m_bTestBool(true),// NOLINT
    m_array({ 1.1, 2.1, 3.1 })// NOLINT
{}


TEST_CASE("serializer", "[json]")
{
  TestClassA const testClass;
  json jvalue;
  to_json(jvalue,testClass);
  std::string output = jvalue.dump();
  //std::cout <<'\n'<< output << '\n';
  std::string expected =R"({"array":[1.1,2.1,3.1],"bool":true,"double":3.14159,"int":42,"string":"foo"})";
  auto rm_space = [](const char character) { return isspace(character); };

  const auto [first, last] = std::ranges::remove_if(output, rm_space);
  output.erase(first, last);
  const auto [first_exp, last_exp] = std::ranges::remove_if(expected, rm_space);
  expected.erase(first_exp, last_exp);

  REQUIRE(std::strcmp(expected.c_str(), output.c_str()) == 0);
}
