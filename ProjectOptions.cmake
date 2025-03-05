include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(poutre2_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(poutre2_setup_options)
  option(poutre2_ENABLE_HARDENING "Enable hardening" ON)
  option(poutre2_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    poutre2_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    poutre2_ENABLE_HARDENING
    OFF)

  poutre2_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR poutre2_PACKAGING_MAINTAINER_MODE)
    option(poutre2_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(poutre2_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(poutre2_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(poutre2_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(poutre2_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(poutre2_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(poutre2_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(poutre2_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(poutre2_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(poutre2_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(poutre2_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(poutre2_ENABLE_PCH "Enable precompiled headers" OFF)
    option(poutre2_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(poutre2_ENABLE_IPO "Enable IPO/LTO" ON)
    option(poutre2_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(poutre2_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(poutre2_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(poutre2_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(poutre2_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(poutre2_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(poutre2_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(poutre2_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(poutre2_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(poutre2_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(poutre2_ENABLE_PCH "Enable precompiled headers" OFF)
    option(poutre2_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      poutre2_ENABLE_IPO
      poutre2_WARNINGS_AS_ERRORS
      poutre2_ENABLE_USER_LINKER
      poutre2_ENABLE_SANITIZER_ADDRESS
      poutre2_ENABLE_SANITIZER_LEAK
      poutre2_ENABLE_SANITIZER_UNDEFINED
      poutre2_ENABLE_SANITIZER_THREAD
      poutre2_ENABLE_SANITIZER_MEMORY
      poutre2_ENABLE_UNITY_BUILD
      poutre2_ENABLE_CLANG_TIDY
      poutre2_ENABLE_CPPCHECK
      poutre2_ENABLE_COVERAGE
      poutre2_ENABLE_PCH
      poutre2_ENABLE_CACHE)
  endif()

  poutre2_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (poutre2_ENABLE_SANITIZER_ADDRESS OR poutre2_ENABLE_SANITIZER_THREAD OR poutre2_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(poutre2_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(poutre2_global_options)
  #openMP for all
  FIND_PACKAGE(OpenMP REQUIRED)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()

  if(poutre_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    poutre2_enable_ipo()
  endif()

  poutre2_supports_sanitizers()

  if(poutre2_ENABLE_HARDENING AND poutre2_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR poutre2_ENABLE_SANITIZER_UNDEFINED
       OR poutre2_ENABLE_SANITIZER_ADDRESS
       OR poutre2_ENABLE_SANITIZER_THREAD
       OR poutre2_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${poutre2_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${poutre2_ENABLE_SANITIZER_UNDEFINED}")
    poutre2_enable_hardening(poutre2_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(poutre2_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()
  
  include(cmake/SimdDefaultFlags.cmake)

  add_library(poutre2_warnings INTERFACE)
  add_library(poutre2_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  poutre2_set_project_warnings(
    poutre2_warnings
    ${poutre2_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(poutre2_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    poutre2_configure_linker(poutre2_options)
  endif()

  include(cmake/Sanitizers.cmake)
  poutre2_enable_sanitizers(
    poutre2_options
    ${poutre2_ENABLE_SANITIZER_ADDRESS}
    ${poutre2_ENABLE_SANITIZER_LEAK}
    ${poutre2_ENABLE_SANITIZER_UNDEFINED}
    ${poutre2_ENABLE_SANITIZER_THREAD}
    ${poutre2_ENABLE_SANITIZER_MEMORY})

  set_target_properties(poutre2_options PROPERTIES UNITY_BUILD ${poutre2_ENABLE_UNITY_BUILD})

  if(poutre2_ENABLE_PCH)
    target_precompile_headers(
      poutre2_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(poutre2_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    poutre2_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(poutre2_ENABLE_CLANG_TIDY)
    poutre2_enable_clang_tidy(poutre2_options ${poutre2_WARNINGS_AS_ERRORS})
  endif()

  if(poutre2_ENABLE_CPPCHECK)
    poutre2_enable_cppcheck(${poutre2_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(poutre2_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    poutre2_enable_coverage(poutre2_options)
  endif()

  if(poutre2_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(poutre2_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(poutre2_ENABLE_HARDENING AND NOT poutre2_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR poutre2_ENABLE_SANITIZER_UNDEFINED
       OR poutre2_ENABLE_SANITIZER_ADDRESS
       OR poutre2_ENABLE_SANITIZER_THREAD
       OR poutre2_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    poutre2_enable_hardening(poutre2_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
