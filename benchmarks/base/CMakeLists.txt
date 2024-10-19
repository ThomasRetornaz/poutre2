set(subdirsource ${PROJECT_SOURCE_DIR}/base)
include_directories(${PROJECT_SOURCE_DIR}/include/poutre)

include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

set(PoutreBaseBenchSRC
  ${subdirsource}/main.hpp
  ${subdirsource}/main.cpp
  ${subdirsource}/containerview.cpp
)

add_executable(poutre_base_bench ${PoutreBaseBenchSRC})

set_target_properties(poutre_base_bench PROPERTIES
  FOLDER "Benchmarks/")

IF(WIN32)
  target_link_libraries(poutre_base_bench
    PUBLIC benchmark
    PUBLIC benchmark_main
    PUBLIC shlwapi.lib
    PRIVATE poutre::poutre_warnings
    poutre::poutre_options
    jsoncpp_lib
    spdlog::spdlog
    Catch2::Catch2WithMain
    PUBLIC poutre_base::poutre_base
    PUBLIC OpenMP::OpenMP_CXX
  )
ELSE()
  target_link_libraries(poutre_base_bench
    PUBLIC benchmark
    PUBLIC benchmark_main
    PRIVATE poutre::poutre_warnings
    poutre::poutre_options
    jsoncpp_lib
    spdlog::spdlog
    Catch2::Catch2WithMain
    PUBLIC poutre_base::poutre_base
    PUBLIC OpenMP::OpenMP_CXX
  )
ENDIF()
