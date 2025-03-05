cmake_minimum_required(VERSION 3.21)

# This template attempts to be "fetch_content"-able
# so that it works well with tools like CPM or other
# manual dependency management

# Only set the cxx_standard if it is not set by someone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS ON)

# Set the project name and language
project(
  poutre2
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/ThomasRetornaz/poutre2"
  LANGUAGES CXX C ASM ASM_MASM)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
OPTION(POUTRE_BUILD_TEST "Build Tests Suites" YES)
OPTION(POUTRE_BUILD_BENCH "Build Benchmark Suites" YES)
OPTION(POUTRE_BUILD_DOC "Build doxygen documentation" NO)

if(BUILD_SHARED_LIBS)
  message(STATUS "Building shared libraries")
  add_definitions(-DPOUTRE_DYNAMIC)
else()
  message(STATUS "Building static libraries")
endif()

include(cmake/PreventInSourceBuilds.cmake)
include(ProjectOptions.cmake)

poutre2_setup_options()

poutre2_global_options()
include(Dependencies.cmake)
poutre2_setup_dependencies()

poutre2_local_options()

# dont know if this should be set globally from here or not...
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(GIT_SHA
  "Unknown"
  CACHE STRING "SHA this build was generated from")
string(
  SUBSTRING "${GIT_SHA}"
  0
  8
  GIT_SHORT_SHA)

target_compile_features(poutre2_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(poutre2::poutre2_options ALIAS poutre2_options)
add_library(poutre2::poutre2_warnings ALIAS poutre2_warnings)

# add_library(poutre2::poutre2_options INTERFACE IMPORTED)
# add_library(poutre2::poutre2_warnings INTERFACE IMPORTED)

# configure files based on CMake configuration options
add_subdirectory(configured_files)

# Adding the src:
include_directories(BEFORE ${XSIMD_INCLUDE_DIRECTORY})
# include_directories(BEFORE ${MDSPAN_INCLUDE_DIRECTORY})
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)

# Don't even look at tests/benchmark/documentation if we're not top level
if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

# Adding documentation
if(POUTRE_BUILD_DOC)
  message(STATUS "Configuring the documentation")
  include(cmake/Doxygen.cmake)
  poutre2_enable_doxygen("")
endif()

# Adding the benchmarks:
if(POUTRE_BUILD_BENCH)
  message(STATUS "Building Benchmarks")
  add_subdirectory(benchmarks)
endif()

# Adding the tests:
include(CTest)

if(BUILD_TESTING)
  message(STATUS "Building Tests. Be sure to check out test/constexpr_tests.cpp for constexpr testing")
  add_subdirectory(test)
endif()

if(poutre2_BUILD_FUZZ_TESTS)
  message(AUTHOR_WARNING "Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")

  if(NOT poutre2_ENABLE_ADDRESS_SANITIZER AND NOT poutre2_ENABLE_THREAD_SANITIZER)
    message(WARNING "You need asan or tsan enabled for meaningful fuzz testing")
  endif()

  # add_subdirectory(fuzz_test)
endif()

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC s debugger, and we can run the target from visual studio
if(MSVC)
  get_all_installable_targets(all_targets)
  message("all_targets=${all_targets}")
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT poutre_base)

if(CMAKE_SKIP_INSTALL_RULES)
  return()
endif()

include(cmake/PackageProject.cmake)

# Add other targets that you want installed here, by default we just package the one executable
# we know we want to ship
# poutre_package_project(
# TARGETS
# intro
# poutre_options
# poutre_warnings
# # FIXME: this does not work! CK
# # PRIVATE_DEPENDENCIES_CONFIGURED project_options project_warnings
# )

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
