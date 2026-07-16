set(subdirsource ${PROJECT_SOURCE_DIR}/distance)

# Test project
set(PoutreDISTTestSRC
        ${subdirsource}/mdistance.cpp
        ${subdirsource}/geodesic_distance.cpp
)

add_executable(poutre_distance_tests ${PoutreDISTTestSRC})

target_link_libraries(
        poutre_distance_tests
        PRIVATE poutre2::poutre2_warnings
        poutre2::poutre2_options
        spdlog::spdlog
        Catch2::Catch2WithMain
        PUBLIC poutre_base::poutre_base
        PUBLIC poutre_pixel_processing::poutre_pixel_processing
        PUBLIC poutre_low_level_morpho::poutre_low_level_morpho
        PUBLIC poutre_distance::poutre_distance
)

if (NOT EMSCRIPTEN)
    find_package(Threads)
    target_link_libraries(poutre_distance_tests
            PUBLIC Threads::Threads)
endif ()

add_definitions(-D_GLIBCXX_USE_CXX11_ABI)

IF (POUTRE_CI)
    set_target_properties(poutre_distance_tests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF ()

# set_target_properties(poutre_se_tests PROPERTIES VERSION "0.0.1"
# CXX_VISIBILITY_PRESET hidden
# VISIBILITY_INLINES_HIDDEN YES)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
            TARGET poutre_distance_tests
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:poutre_distance_tests> $<TARGET_FILE_DIR:poutre_distance_tests>
            COMMAND_EXPAND_LISTS)
endif ()

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
        poutre_distance_tests
        TEST_PREFIX
        "unittests."
        REPORTER
        XML
        OUTPUT_DIR
        .
        OUTPUT_PREFIX
        "unittests."
        OUTPUT_SUFFIX
        .xml)