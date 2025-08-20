set(subdirsource ${PROJECT_SOURCE_DIR}/io)

# Test project
set(PoutreIOTestSRC
        ${subdirsource}/hdf5.cpp
)

if(POUTRE_BUILD_WITH_OIIO)
    LIST(APPEND PoutreIOTestSRC ${subdirsource}/oiio.cpp)
endif()

add_executable(poutre_io_tests ${PoutreIOTestSRC})

target_link_libraries(
        poutre_io_tests
        PRIVATE poutre2::poutre2_warnings
        poutre2::poutre2_options
        spdlog::spdlog
        Catch2::Catch2WithMain
        PUBLIC poutre_base::poutre_base
        PUBLIC poutre_io::poutre_io
)

if (NOT EMSCRIPTEN)
    find_package(Threads)
    target_link_libraries(poutre_io_tests
            PUBLIC Threads::Threads)
endif()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(poutre_se_tests PUBLIC OpenMP::OpenMP_CXX)
endif()

add_definitions(-D_GLIBCXX_USE_CXX11_ABI)
IF(BUILD_SHARED_LIBS)
    set_target_properties(poutre_se_tests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
    set_target_properties(poutre_se_tests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

# set_target_properties(poutre_se_tests PROPERTIES VERSION "0.0.1"
# CXX_VISIBILITY_PRESET hidden
# VISIBILITY_INLINES_HIDDEN YES)

if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
            TARGET poutre_io_tests
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:poutre_io_tests> $<TARGET_FILE_DIR:poutre_io_tests>
            COMMAND_EXPAND_LISTS)
endif()

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
        poutre_io_tests
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