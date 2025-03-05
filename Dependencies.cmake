include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(poutre2_setup_dependencies)
  message(STATUS "┌─ Dependencies.cmake")
  list(APPEND CMAKE_MESSAGE_INDENT "│    ")

  include(ExternalProject)

  # ### Benchmark
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

  # ### FMTLIB
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

  # ### Catch2
  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.7.1")
  endif()

  # ### CLI
  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage("gh:CLIUtils/CLI11@2.4.2")
  endif()

  # ### ftxui
  if(NOT TARGET ftxui::screen)
    cpmaddpackage("gh:ArthurSonzogni/FTXUI@5.0.0")
  endif()

  # ### tools
  if(NOT TARGET tools::tools)
    cpmaddpackage("gh:lefticus/tools#update_build_system")
  endif()

  # ### ZLIB
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

      # message(STATUS "Cmake added local zlib: ${zlib_SOURCE_DIR}")
    endif()
  endif()

  # ### JPEG-TURBO
  set(JPEG_TURBO_REPO "https://github.com/libjpeg-turbo/libjpeg-turbo.git"
    CACHE STRING "libjpeg-turbo repository location."
  )

  ExternalProject_Add(libjpeg-turbo
    GIT_REPOSITORY ${JPEG_TURBO_REPO}
    GIT_TAG "3.0.90"
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}

    # --Update/Patch step----------
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    INSTALL_COMMAND ""

    CMAKE_ARGS
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}

    # --Configure step-------------
    #
    BUILD_COMMAND ""
  )

  ExternalProject_Get_Property(libjpeg-turbo TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)

  # message("Build libjpeg-turbo src ${SOURCE_DIR} in ${BINARY_DIR}")

  # add_library(libjpeg-turbo INTERFACE IMPORTED)
  # target_include_directories(
  # libjpeg-turbo SYSTEM
  # INTERFACE ${jsoncpp_SOURCE_DIR}/include
  # )
  INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg-turbo
    ${CMAKE_CURRENT_BINARY_DIR}/libjpeg-turbo
  )
  set(JPEG_LIBRARY jpeg-static)
  set(JPEG_INCLUDE_DIRS ${SOURCE_DIR}/include)
  set(JPEG_LIBRARIES ${BINARY_DIR})

  set(JPEG_FOUND TRUE)

  # message(STATUS "**************JPEG: ${JPEG_INCLUDE_DIRS} ${JPEG_LIBRARIES}")

  # ### PNG
  if(NOT TARGET png::png)
    cpmaddpackage(
      NAME
      png
      VERSION
      1.6.44
      GITHUB_REPOSITORY
      "pnggroup/libpng"
      OPTIONS
      "ZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIRS}"
      "ZLIB_LIBRARY=${ZLIB_LIBRARIES}"
      "BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}"
      "CMAKE_POSITION_INDEPENDENT_CODE YES"
      "PNG_TESTS NO"
      "PNG_TOOLS NO")
  endif()

  # ### XSIMD

  # clone approach
  ExternalProject_Add(XsimdDep
    GIT_REPOSITORY https://github.com/QuantStack/xsimd.git
    GIT_TAG 13.2.0
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

  # message("Build XsimdDep src ${SOURCE_DIR} in ${BINARY_DIR}")
  add_library(xsimd INTERFACE)
  target_include_directories(xsimd INTERFACE
    $<BUILD_INTERFACE:${SOURCE_DIR}/include> # we can do better
    $<INSTALL_INTERFACE:include/xsimd>
  )
  SET(XSIMD_INCLUDE_DIRECTORY "${SOURCE_DIR}/include" PARENT_SCOPE)

  # ### MDSPAN
  # ExternalProject_Add(mdspanDep
  #   GIT_REPOSITORY https://github.com/kokkos/mdspan.git
  #   GIT_TAG stable
  #   PREFIX ${CMAKE_CURRENT_BINARY_DIR}

  #   # --Update/Patch step----------
  #   UPDATE_COMMAND ""
  #   PATCH_COMMAND ""
  #   INSTALL_COMMAND ""

  #   # --Configure step-------------
  #   #
  #   BUILD_COMMAND ""
  # )

  # ExternalProject_Get_Property(mdspanDep TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)

  # # message("Build mdspan src ${SOURCE_DIR} in ${BINARY_DIR}")
  # add_library(mdspan INTERFACE)
  # target_include_directories(mdspan INTERFACE
  #   $<BUILD_INTERFACE:${SOURCE_DIR}/include> # we can do better
  #   $<INSTALL_INTERFACE:include/mdspan>
  # )
  # SET(MDSPAN_INCLUDE_DIRECTORY "${SOURCE_DIR}/include" PARENT_SCOPE)

  # ### JSONCPP
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

      # message(STATUS "Cmake added local jsoncpp: ${jsoncpp_SOURCE_DIR}")
    endif()
  endif()

  list(POP_BACK CMAKE_MESSAGE_INDENT)
  message(STATUS "└─ End Dependencies.cmake")
endfunction()
