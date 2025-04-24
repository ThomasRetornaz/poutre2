set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/pixel_processing)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/pixel_processing)

set(PoutrePPSRC_DETAILS
  ${subdirheader}/details/copy_convert_t.hpp
)

set(PoutrePPSRC_PUBLICHEADERS
  ${subdirheader}/pixel_processing.hpp
  ${subdirheader}/copy_convert.hpp
  )

set(PoutrePPSRC_CPP
  ${subdirsource}/copy_convert.cpp
  )

source_group(details FILES ${PoutrePPSRC_DETAILS})
source_group(src FILES ${PoutrePPSRC_CPP})
source_group(header FILES ${PoutrePPSRC_PUBLICHEADERS})

set(PoutreBaseSRC ${PoutrePPSRC_DETAILS}
  ${PoutrePPSRC_CPP}
  ${PoutrePPSRC_PUBLICHEADERS})


add_library(poutre_pixel_processing ${PoutreBaseSRC})
add_library(poutre_pixel_processing::poutre_pixel_processing ALIAS poutre_pixel_processing)


target_link_libraries(poutre_pixel_processing PRIVATE poutre2_options
  PRIVATE poutre2_warnings
  PRIVATE jsoncpp_lib
  PRIVATE spdlog::spdlog
  INTERFACE xsimd
  PUBLIC poutre_base::poutre_base
  PUBLIC OpenMP::OpenMP_CXX
)

# force custom target before start
add_dependencies(poutre_pixel_processing xsimd)
target_include_directories(poutre_pixel_processing ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_pixel_processing PUBLIC cxx_std_23)

set_target_properties(
  poutre_pixel_processing
  PROPERTIES VERSION ${PROJECT_VERSION}
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES)
