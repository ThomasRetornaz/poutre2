
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
#include <poutre/io/loader.hpp>
#include <poutre/io/io.hpp>

#include <algorithm>
#include <filesystem>
#include <cctype>
#include <memory>
#include <string>

namespace poutre::io {
namespace fs = std::filesystem;

ImageLoader::ImageLoader()://m_imgPath(""),
                             m_i_name("poutre_img_1"),m_isready(false)
{
}

ImageLoader &ImageLoader::SetName(const std::string &i_name)
{
  m_i_name = i_name;
  return *this;
}
ImageLoader &ImageLoader::SetPath(const std::string &i_imgpath)
{
  m_imgPath = i_imgpath;
  m_isready = true;
  return *this;
}

std::unique_ptr<IInterface> ImageLoader::Load() const
{
  POUTRE_ENTERING("ImageLoader::Load()");
  if( !m_isready )
    {
    POUTRE_RUNTIME_ERROR("ImageLoader:  you must set path through SetPath");
    }
  const fs::path localPath(m_imgPath);
  if( !fs::exists(localPath) )
  {
    POUTRE_RUNTIME_ERROR("ImageLoader:: path doesn't exist ");
  }
  // switch on extension
  auto extension = localPath.extension().string();

  std::ranges::transform(extension, extension.begin(), [](unsigned char charac) { return static_cast<unsigned char>(std::tolower(charac)); });

  if( extension == ".h5" )
  {
    return LoadHDF5(localPath.string(),m_i_name);
  }
#if defined POUTRE_BUILD_WITH_OIIO
  return LoadOIIO(localPath.string());
#else
  POUTRE_RUNTIME_ERROR("Not supported file type. May you need to build with OpenImageIO");
#endif
}
}