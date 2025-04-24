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
#include <catch2/matchers/catch_matchers.hpp>
// #include <catch2/matchers/catch_matchers_container_properties.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>
// #include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstddef>
#include <poutre/base/details/data_structures/image_t.hpp>
#include <poutre/base/image_interface.hpp>
#include <poutre/base/types.hpp>
// #include <poutre/base/types_traits.hpp>
#include <poutre/pixel_processing/copy_convert.hpp>
// #include <string>
#include <vector>
#include <memory>


TEST_CASE("clone", "[copy_convert]")
{
    using ImageType = poutre::details::image_t<poutre::pUINT8>;
    using ImageTypeInterface = std::unique_ptr<poutre::IInterface>;
    ImageType img({3, 4}); //-V112
    const poutre::pUINT8 value = 10;
    img.fill(value);
  
    const ImageTypeInterface getSameii = poutre::Clone(img);
  
    const auto *getSame = dynamic_cast<const ImageType *>(getSameii.get());
    REQUIRE(getSame != nullptr);
    //!= address
    REQUIRE(getSame !=  &(img));
    //!= data address
    REQUIRE(&(*((*getSame).data()))!= &(*(img.data()))); //-V522

    REQUIRE((*getSame).GetPType() == poutre::PType::PType_GrayUINT8);
    REQUIRE((*getSame).GetCType() == poutre::CompoundType::CompoundType_Scalar);
    REQUIRE((*getSame).GetXSize() == 4); //-V112
    REQUIRE((*getSame).GetYSize() == 3);
    REQUIRE((*getSame).GetRank() == 2);
    auto dims = (*getSame).GetShape();
    const std::vector<size_t> expectedcoords = { 3, 4 };
    REQUIRE_THAT(dims, Catch::Matchers::Equals(expectedcoords));
   
    auto beg = (*getSame).cbegin();
    auto end = (*getSame).cend();

    for (;beg!=end;++beg) {
        REQUIRE((*beg) == value);
    }
  }
  
  TEST_CASE("clone_geometry", "[copy_convert]")
  {
    using ImageType = poutre::details::image_t<poutre::pUINT8>;
    using ImageTypeInterface = std::unique_ptr<poutre::IInterface>;
    ImageType img({3, 4}); //-V112
    const poutre::pUINT8 value = 10;
    img.fill(value);
  
    const ImageTypeInterface getSameii = poutre::CloneGeometry(img);
  
    const auto *getSameGeo = dynamic_cast<const ImageType *>(getSameii.get());

    REQUIRE(getSameGeo!=nullptr);
    //!= address
    REQUIRE(getSameGeo !=  &(img));
    //!= data address
    REQUIRE(&(*((*getSameGeo).data()))!= &(*(img.data()))); //-V522

    REQUIRE((*getSameGeo).GetPType() == poutre::PType::PType_GrayUINT8);
    REQUIRE((*getSameGeo).GetCType() == poutre::CompoundType::CompoundType_Scalar);
    REQUIRE((*getSameGeo).GetXSize() == 4); //-V112
    REQUIRE((*getSameGeo).GetYSize() == 3);
    REQUIRE((*getSameGeo).GetRank() == 2);
    auto dims = (*getSameGeo).GetShape();
    const std::vector<size_t> expectedcoords = { 3, 4 };
    REQUIRE_THAT(dims, Catch::Matchers::Equals(expectedcoords));
  }
  
  TEST_CASE("copy", "[copy_convert]")
  {
    { // integral types
      using ImageTypeBase = poutre::details::image_t<poutre::pUINT8>;
  
      ImageTypeBase img({3, 4}); //-V112
      const poutre::pUINT8 val1 =10;
      img.fill(val1);
      ImageTypeBase img2({3, 4}); //-V112
      const poutre::pUINT8 val2 =4;
      img2.fill(val2);             //-V112
      poutre::CopyInto(img, img2);
      REQUIRE_THAT(img,  Catch::Matchers::RangeEquals(img2));
    }
    { // compound
      using ImageTypeBase = poutre::details::image_t<poutre::c3pUINT8>;
      ImageTypeBase img({3, 4}); //-V112
      const poutre::c3pUINT8 val1={1, 2, 3};
      img.fill(val1);
      ImageTypeBase img2({3, 4}); //-V112
      const poutre::c3pUINT8 val2={4, 5, 6};
      img2.fill(val2);     //-V112
      poutre::CopyInto(img, img2);
      REQUIRE_THAT(img,  Catch::Matchers::RangeEquals(img2));
    }
  }
  