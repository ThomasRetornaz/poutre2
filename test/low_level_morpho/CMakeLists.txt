set(subdirsource ${PROJECT_SOURCE_DIR}/low_level_morpho)

# Test project
set(PoutreLLMTestSRC
        ${subdirsource}/ero_dil_static_se_t.cpp
        ${subdirsource}/ero_dil_runtime_se.cpp
)

add_executable(poutre_llm_tests ${PoutreLLMTestSRC})
target_link_libraries(
        poutre_llm_tests
  PRIVATE poutre2::poutre2_warnings
  poutre2::poutre2_options
  spdlog::spdlog
  Catch2::Catch2WithMain
  PUBLIC poutre_base::poutre_base
  PUBLIC poutre_structuring_element::poutre_structuring_element
  PUBLIC poutre_pixel_processing::poutre_pixel_processing
)

if (NOT EMSCRIPTEN)
  find_package(Threads)
  target_link_libraries(poutre_llm_tests
          PUBLIC Threads::Threads)
endif()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(poutre_llm_tests PUBLIC OpenMP::OpenMP_CXX)
endif()

# add_definitions(-D_GLIBCXX_USE_CXX11_ABI)
IF(BUILD_SHARED_LIBS)
  set_target_properties(poutre_llm_tests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
  set_target_properties(poutre_llm_tests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

# set_target_properties(poutre_llm_tests PROPERTIES VERSION "0.0.1"
# CXX_VISIBILITY_PRESET hidden
# VISIBILITY_INLINES_HIDDEN YES)

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET poutre_llm_tests
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:poutre_llm_tests> $<TARGET_FILE_DIR:poutre_llm_tests>
    COMMAND_EXPAND_LISTS)
endif()

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  poutre_llm_tests
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