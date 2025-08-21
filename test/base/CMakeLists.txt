set(subdirsource ${PROJECT_SOURCE_DIR}/base)

# Test project
set(PoutreBaseTestSRC
        ${subdirsource}/chronos.cpp
        ${subdirsource}/json_serialize.cpp
        ${subdirsource}/registrar.cpp
        ${subdirsource}/image.cpp
        ${subdirsource}/image_iostring.cpp
        ${subdirsource}/coordinate.cpp
        ${subdirsource}/containerview.cpp
        ${subdirsource}/threading.cpp
        ${subdirsource}/types.cpp
        ${subdirsource}/pq.cpp
)

add_executable(poutre_base_tests ${PoutreBaseTestSRC})
target_link_libraries(
        poutre_base_tests
        PRIVATE poutre2::poutre2_warnings
        poutre2::poutre2_options
        PRIVATE json::json
        spdlog::spdlog
        Catch2::Catch2WithMain
        PUBLIC poutre_base::poutre_base
)

find_package(Threads REQUIRED)
target_link_libraries(poutre_base_tests
        PUBLIC Threads::Threads)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(poutre_base_tests PUBLIC OpenMP::OpenMP_CXX)
endif()

IF(POUTRE_CI)
    set_target_properties(poutre_base_tests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

# set_target_properties(poutre_base_tests PROPERTIES VERSION "0.0.1"
# CXX_VISIBILITY_PRESET hidden
# VISIBILITY_INLINES_HIDDEN YES)

if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
            TARGET poutre_base_tests
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:poutre_base_tests> $<TARGET_FILE_DIR:poutre_base_tests>
            COMMAND_EXPAND_LISTS)
endif()

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
        poutre_base_tests
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