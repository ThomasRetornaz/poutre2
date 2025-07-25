set(subdirsource ${PROJECT_SOURCE_DIR}/pixel_processing)

# Test project
set(PoutrePixelProcessingTestSRC
  ${subdirsource}/copy_convert.cpp
  ${subdirsource}/arith.cpp
  ${subdirsource}/compare.cpp
  ${subdirsource}/transpose.cpp
)

add_executable(poutre_pixel_processing_tests ${PoutrePixelProcessingTestSRC})
target_link_libraries(
  poutre_pixel_processing_tests
  PRIVATE poutre2::poutre2_warnings
  poutre2::poutre2_options
  jsoncpp_lib
  spdlog::spdlog
  Catch2::Catch2WithMain
  PUBLIC poutre_pixel_processing::poutre_pixel_processing
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(poutre_pixel_processing_tests PUBLIC OpenMP::OpenMP_CXX)
endif()

# add_definitions(-D_GLIBCXX_USE_CXX11_ABI)
IF(BUILD_SHARED_LIBS)
  set_target_properties(poutre_pixel_processing_tests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
  set_target_properties(poutre_pixel_processing_tests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

# set_target_properties(poutre_pixel_processing_tests PROPERTIES VERSION "0.0.1"
# CXX_VISIBILITY_PRESET hidden
# VISIBILITY_INLINES_HIDDEN YES)

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET poutre_pixel_processing_tests
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:poutre_pixel_processing_tests> $<TARGET_FILE_DIR:poutre_pixel_processing_tests>
    COMMAND_EXPAND_LISTS)
endif()

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  poutre_pixel_processing_tests
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