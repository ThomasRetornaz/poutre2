// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/poutreJsonSerialization.hpp>

#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include <memory>
#include <string>

// NOLINTBEGIN

namespace poutre {
bool JsonSerializer::Serialize(InterfaceJsonSerializable *iptrJSObj, std::string &ostr)
{
  if (iptrJSObj == nullptr) { return false; }

  Json::Value serializeRoot;
  iptrJSObj->Serialize(serializeRoot);

  Json::StreamWriterBuilder builder;
  Json::Value settings = Json::nullValue;
  Json::Value invalid_settings;
  builder.setDefaults(&settings);

  if (!builder.validate(&invalid_settings)) { return false; }

  ostr = Json::writeString(builder, serializeRoot);

  return true;
}

bool JsonSerializer::Deserialize(InterfaceJsonSerializable *iptrJSObj, const std::string &istr)
{
  if (iptrJSObj == nullptr) { return false; }

  Json::Value value;

  std::string jsonReaderError;
  Json::CharReaderBuilder jsonReaderBuilder;
  std::unique_ptr<Json::CharReader> const reader(jsonReaderBuilder.newCharReader());

  if (!reader->parse(istr.c_str(), istr.c_str() + istr.size(), &value, &jsonReaderError)) { return false; }
  iptrJSObj->Deserialize(value);

  return true;
}

}// namespace poutre
// NOLINTEND