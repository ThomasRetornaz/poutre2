set(subdirsource ${PROJECT_SOURCE_DIR}/low_level_morpho)
include_directories(${PROJECT_SOURCE_DIR}/include/poutre)

include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

set(PoutreLLMBenchSRC
  ${subdirsource}/main.hpp
  ${subdirsource}/main.cpp
  ${subdirsource}/ero_dil.cpp
)

add_executable(poutre_low_level_morpho_bench ${PoutreLLMBenchSRC})

set_target_properties(poutre_low_level_morpho_bench PROPERTIES
  FOLDER "Benchmarks/")

IF(WIN32)
  target_link_libraries(poutre_low_level_morpho_bench
    PUBLIC benchmark
    PUBLIC benchmark_main
    PUBLIC shlwapi.lib
    PRIVATE poutre2::poutre2_warnings
    poutre2::poutre2_options
    spdlog::spdlog
    Catch2::Catch2WithMain
    PUBLIC poutre_base::poutre_base
    PUBLIC poutre_pixel_processing::poutre_pixel_processing
    PUBLIC poutre_structuring_element::poutre_structuring_element
  )
ELSE()
  target_link_libraries(poutre_low_level_morpho_bench
    PUBLIC benchmark
    PUBLIC benchmark_main
    PRIVATE poutre2::poutre2_warnings
    poutre2::poutre2_options
    spdlog::spdlog
    Catch2::Catch2WithMain
    PUBLIC poutre_base::poutre_base
    PUBLIC poutre_pixel_processing::poutre_pixel_processing
    PUBLIC poutre_structuring_element::poutre_structuring_element
  )
ENDIF()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(poutre_low_level_morpho_bench PUBLIC OpenMP::OpenMP_CXX)
endif()