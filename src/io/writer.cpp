// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#include <poutre/base/config.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/trace.hpp>
#include <poutre/io/writer.hpp>
#include <poutre/io/io.hpp>

#include <algorithm>
#include <filesystem>
#include <format>
#include <cctype>
#include <string>

namespace poutre::io {
namespace fs = std::filesystem;

ImageWriter::ImageWriter()://m_imgPath(""),
                           m_i_name("poutre_img_1"),m_isready(false)
{
}

ImageWriter &ImageWriter::SetName(const std::string &i_name)
{
  m_i_name = i_name;
  return *this;
}
ImageWriter &ImageWriter::SetPath(const std::string &i_imgpath)
{
  m_imgPath = i_imgpath;
  m_isready = true;
  return *this;
}

void ImageWriter::Write(const IInterface &i_img) const
{
  POUTRE_ENTERING("ImageWriter::Write()");

  if( !m_isready ) {
    POUTRE_RUNTIME_ERROR("ImageWritter:  you must set path through SetPath");
}
  fs::path const localPath(m_imgPath);

  fs::path const dir = localPath.parent_path();
  if( !(fs::exists(dir)) )
  {
    POUTRE_RUNTIME_ERROR((std::format("ImageWriter: provided path {} doesn't exists", dir.string())));
  }

  // switch on extension
  auto extension = localPath.extension().string();
  std::ranges::transform(extension, extension.begin(),
     [](unsigned char charac){ return std::tolower(charac); });

  if( extension == ".h5" ) {
    DumpHDF5(localPath.string(), i_img, m_i_name);
    }
  else {
    DumpOIIO(localPath.string(), i_img);
  }
}
}