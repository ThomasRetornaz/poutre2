include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(poutre2_setup_dependencies)
  message(STATUS "┌─ Dependencies.cmake")
  list(APPEND CMAKE_MESSAGE_INDENT "│    ")
  # For each dependency, see if it's
  # already been provided to us by a parent project
  # if(NOT TARGET googletest::googletest)
  #   cpmaddpackage(
  #     NAME googletest
  #     GITHUB_REPOSITORY google/googletest

  #     # VERSION ${ogs.minimum_version.gtest}
  #     GIT_TAG v1.15.2
  #     OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt ON"
  #     "BUILD_SHARED_LIBS OFF"
  #     EXCLUDE_FROM_ALL YES SYSTEM TRUE
  #   )

  #   if(googletest_ADDED AND WIN32)
  #     target_compile_options(gtest PRIVATE /EHsc)
  #     target_compile_options(gmock PRIVATE /EHsc)
  #   endif()
  # endif()

  if(NOT TARGET googlebenchmark::googlebenchmark)
    cpmaddpackage(
      NAME googlebenchmark
      GITHUB_REPOSITORY google/benchmark

      # VERSION ${ogs.minimum_version.gtest}
      GIT_TAG v1.9.0

      OPTIONS "BENCHMARK_ENABLE_GTEST_TESTS OFF" "BENCHMARK_ENABLE_TESTING OFF"
      # "BUILD_SHARED_LIBS OFF"
      EXCLUDE_FROM_ALL YES SYSTEM TRUE
    )
  endif()

  # if(NOT TARGET benchmark::benchmark)
  # cpmaddpackage("gh:google/benchmark#1.9.0")
  # endif()
  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage("gh:fmtlib/fmt#11.0.2")
  endif()

  if(NOT TARGET spdlog::spdlog)
    cpmaddpackage(
      NAME
      spdlog
      VERSION
      1.14.1
      GITHUB_REPOSITORY
      "gabime/spdlog"
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.7.1")
  endif()

  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage("gh:CLIUtils/CLI11@2.4.2")
  endif()

  if(NOT TARGET ftxui::screen)
    cpmaddpackage("gh:ArthurSonzogni/FTXUI@5.0.0")
  endif()

  if(NOT TARGET tools::tools)
    cpmaddpackage("gh:lefticus/tools#update_build_system")
  endif()

  if(NOT TARGET zlib::zlib)
    # Add ZLIB
    set(ZLIB_REPO "https://github.com/madler/zlib.git"
      CACHE STRING "ZLIB repository location."
    )
    CPMAddPackage(
      NAME zlib
      GIT_REPOSITORY ${ZLIB_REPO}
      GIT_TAG "v1.3.1"
      OPTIONS
      "CMAKE_POSITION_INDEPENDENT_CODE True"
    )

    if(zlib_ADDED)
      target_include_directories(zlib
        PUBLIC $<BUILD_INTERFACE:${zlib_BINARY_DIR}>
        PUBLIC $<INSTALL_INTERFACE:include>)
      target_include_directories(zlibstatic
        PUBLIC $<BUILD_INTERFACE:${zlib_BINARY_DIR}>
        PUBLIC $<INSTALL_INTERFACE:include>)

      message(STATUS "Cmake added local zlib: ${zlib_SOURCE_DIR}")
    endif()
  endif()

  # if(NOT TARGET xsimd::xsimd)
  # # Add XSIMD
  # set(XSIMD_REPO https://github.com/QuantStack/xsimd.git
  # CACHE STRING "XSIMD repository location."
  # )

  # CPMAddPackage(
  # NAME xsimd
  # GIT_REPOSITORY ${XSIMD_REPO}
  # GIT_TAG "master"

  # # OPTIONS
  # # "CMAKE_POSITION_INDEPENDENT_CODE True"
  # )

  # if(xsimd_ADDED)
  # add_library(xsimd::xsimd INTERFACE IMPORTED)
  # target_include_directories(
  # xsimd::xsimd SYSTEM
  # INTERFACE ${xsimd_SOURCE_DIR}/include
  # )
  # message(STATUS "Cmake added local xsimd: ${xsimd_SOURCE_DIR}/include")
  # endif()
  # endif()

  # clone approach
  # find_package(git REQUIRED)
  include(ExternalProject)
  ExternalProject_Add(XsimdDep
    GIT_REPOSITORY https://github.com/QuantStack/xsimd.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}

    # --Update/Patch step----------
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    INSTALL_COMMAND ""

    # --Configure step-------------
    #
    BUILD_COMMAND ""
  )

  ExternalProject_Get_Property(XsimdDep TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
  message("Build XsimdDep src ${SOURCE_DIR} in ${BINARY_DIR}")

  add_library(xsimd INTERFACE)
  target_include_directories(xsimd INTERFACE
    $<BUILD_INTERFACE:${SOURCE_DIR}/include> # we can do better
    $<INSTALL_INTERFACE:include/xsimd>
  )

  if(NOT TARGET jsoncpp::jsoncpp)
    cpmaddpackage(
      NAME
      jsoncpp
      GIT_TAG
      1.9.6
      GITHUB_REPOSITORY
      "open-source-parsers/jsoncpp"
      OPTIONS "JSONCPP_WITH_TESTS OFF" 
              "JSONCPP_WITH_CMAKE_PACKAGE OFF"
              "JSONCPP_WITH_PKGCONFIG_SUPPORT OFF"
              "JSONCPP_WITH_POST_BUILD_UNITTEST OFF"
              "BUILD_SHARED_LIBS ON")

    if(jsoncpp_ADDED)
      add_library(jsoncpp::jsoncpp INTERFACE IMPORTED)
      target_include_directories(
        jsoncpp::jsoncpp SYSTEM
        INTERFACE ${jsoncpp_SOURCE_DIR}/include
      )
      message(STATUS "Cmake added local jsoncpp: ${jsoncpp_SOURCE_DIR}")
    endif()
  endif()

  list(POP_BACK CMAKE_MESSAGE_INDENT)
  message(STATUS "└─ End Dependencies.cmake")
endfunction()
