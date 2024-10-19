
set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/base)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/base)

set(PoutreBaseSRC_DETAILS
  ${subdirheader}/details/simd/poutreSimd.hpp
  ${subdirheader}/details/simd/poutreSimdAlgorithm.hpp
  ${subdirheader}/details/threading/poutreThreading.hpp
  ${subdirheader}/details/data_structures/array_view.hpp
  )

set(PoutreBaseSRC_PUBLICHEADERS
  ${subdirheader}/poutreBase.hpp
  ${subdirheader}/poutreConfig.hpp
  ${subdirheader}/poutreTypes.hpp
  ${subdirheader}/poutreJsonSerialization.hpp
  ${subdirheader}/poutreTrace.hpp
  ${subdirheader}/poutreRegistrar.hpp
  ${subdirheader}/poutreChronos.hpp
  ${subdirheader}/poutreDenseIterator.hpp)

set(PoutreBaseSRC_CPP
  ${subdirsource}/poutreJsonSerialization.cpp
  ${subdirsource}/poutreTrace.cpp
  ${subdirsource}/poutreType.cpp
  ${subdirsource}/poutreChronos.cpp
  ${subdirsource}/poutreThreading.cpp)

source_group(details FILES ${PoutreBaseSRC_DETAILS})
source_group(src FILES ${PoutreBaseSRC_CPP})
source_group(header FILES ${PoutreBaseSRC_PUBLICHEADERS})

set(PoutreBaseSRC ${PoutreBaseSRC_DETAILS}
  ${PoutreBaseSRC_CPP}
  ${PoutreBaseSRC_PUBLICHEADERS})

add_library(poutre_base ${PoutreBaseSRC})
add_library(poutre_base::poutre_base ALIAS poutre_base)

target_link_libraries(poutre_base PRIVATE poutre_options
  PRIVATE poutre_warnings
  PRIVATE jsoncpp_lib
  PRIVATE spdlog::spdlog
  INTERFACE xsimd
  PUBLIC OpenMP::OpenMP_CXX
)

# force custom target before start
add_dependencies(poutre_base xsimd)
target_include_directories(poutre_base ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_base PUBLIC cxx_std_23)

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(poutre_base PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

set_target_properties(
  poutre_base
  PROPERTIES VERSION ${PROJECT_VERSION}
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES)
