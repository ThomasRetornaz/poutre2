set(subdirsource ${PROJECT_SOURCE_DIR}/label)
include_directories(${PROJECT_SOURCE_DIR}/include/poutre)

include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

set(PoutreDistanceBenchSRC
        ${subdirsource}/main.hpp
        ${subdirsource}/main.cpp
        ${subdirsource}/distance.cpp
)

add_executable(poutre_distance_bench ${PoutreDistanceBenchSRC})
add_definitions(-DDATA_DIR=\"${CMAKE_SOURCE_DIR}/utilities/images\")

set_target_properties(poutre_distance_bench PROPERTIES
        FOLDER "Benchmarks/")

IF (WIN32)
    target_link_libraries(poutre_distance_bench
            PUBLIC benchmark
            PUBLIC benchmark_main
            PUBLIC shlwapi.lib
            PRIVATE poutre2::poutre2_warnings
            poutre2::poutre2_options
            spdlog::spdlog
            PUBLIC poutre_base::poutre_base
            PUBLIC poutre_pixel_processing::poutre_pixel_processing
            PUBLIC poutre_structuring_element::poutre_structuring_element
            PUBLIC poutre_io::poutre_io
            PUBLIC poutre_distance::poutre_distance
    )
ELSE ()
    target_link_libraries(poutre_distance_bench
            PUBLIC benchmark
            PUBLIC benchmark_main
            PRIVATE poutre2::poutre2_warnings
            poutre2::poutre2_options
            spdlog::spdlog
            PUBLIC poutre_base::poutre_base
            PUBLIC poutre_pixel_processing::poutre_pixel_processing
            PUBLIC poutre_structuring_element::poutre_structuring_element
            PUBLIC poutre_io::poutre_io
            PUBLIC poutre_distance::poutre_distance
    )
ENDIF ()
