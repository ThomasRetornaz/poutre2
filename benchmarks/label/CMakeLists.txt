set(subdirsource ${PROJECT_SOURCE_DIR}/label)
include_directories(${PROJECT_SOURCE_DIR}/include/poutre)

include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

set(PoutreLabelBenchSRC
        ${subdirsource}/main.hpp
        ${subdirsource}/main.cpp
        ${subdirsource}/label.cpp
)

add_executable(poutre_label_bench ${PoutreLabelBenchSRC})
add_definitions(-DDATA_DIR=\"${CMAKE_SOURCE_DIR}/utilities/images\")

set_target_properties(poutre_label_bench PROPERTIES
        FOLDER "Benchmarks/")

IF (WIN32)
    target_link_libraries(poutre_label_bench
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
            PUBLIC poutre_label::poutre_label
    )
ELSE ()
    target_link_libraries(poutre_label_bench
            PUBLIC benchmark
            PUBLIC benchmark_main
            PRIVATE poutre2::poutre2_warnings
            poutre2::poutre2_options
            spdlog::spdlog
            PUBLIC poutre_base::poutre_base
            PUBLIC poutre_pixel_processing::poutre_pixel_processing
            PUBLIC poutre_structuring_element::poutre_structuring_element
            PUBLIC poutre_io::poutre_io
            PUBLIC poutre_label::poutre_label
    )
ENDIF ()
