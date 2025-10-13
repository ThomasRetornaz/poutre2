set(subdirsource ${PROJECT_SOURCE_DIR}/pixel_processing)
include_directories(${PROJECT_SOURCE_DIR}/include/poutre)

include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

set(PoutrePPBenchSRC
        ${subdirsource}/main.hpp
        ${subdirsource}/main.cpp
        ${subdirsource}/arith.cpp
        ${subdirsource}/compare.cpp
        ${subdirsource}/transpose.cpp
)

add_executable(poutre_pixel_processing_bench ${PoutrePPBenchSRC})

set_target_properties(poutre_pixel_processing_bench PROPERTIES
        FOLDER "Benchmarks/")

IF (WIN32)
    target_link_libraries(poutre_pixel_processing_bench
            PUBLIC benchmark
            PUBLIC benchmark_main
            PUBLIC shlwapi.lib
            PRIVATE poutre2::poutre2_warnings
            poutre2::poutre2_options
            spdlog::spdlog
            Catch2::Catch2WithMain
            PUBLIC poutre_base::poutre_base
            #PUBLIC poutre_pixel_processing::poutre_pixel_processing
    )
ELSE ()
    target_link_libraries(poutre_pixel_processing_bench
            PUBLIC benchmark
            PUBLIC benchmark_main
            PRIVATE poutre2::poutre2_warnings
            poutre2::poutre2_options
            spdlog::spdlog
            Catch2::Catch2WithMain
            PUBLIC poutre_base::poutre_base
            #PUBLIC poutre_pixel_processing::poutre_pixel_processing
    )
ENDIF ()
