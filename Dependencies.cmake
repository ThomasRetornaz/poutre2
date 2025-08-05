include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(poutre2_setup_dependencies)
    message(STATUS "┌─ Dependencies.cmake")
    #list(APPEND CMAKE_MESSAGE_INDENT "│    ")

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
        cpmaddpackage("gh:fmtlib/fmt#11.1.4")
    endif()

    # ### BOOST PREPROCESSOR
  if(NOT TARGET boost_preprocessor)
        cpmaddpackage(
                NAME boost_preprocessor
                GIT_TAG boost-1.88.0
                GITHUB_REPOSITORY "boostorg/preprocessor")
    endif()

    if(NOT TARGET spdlog::spdlog)
        cpmaddpackage(
                NAME
                spdlog
                VERSION
                1.15.2
                GITHUB_REPOSITORY
                "gabime/spdlog"
                OPTIONS
                "SPDLOG_FMT_EXTERNAL ON")
    endif()

    # ### Catch2
    if(NOT TARGET Catch2::Catch2WithMain)
        cpmaddpackage("gh:catchorg/Catch2@3.8.1")
    endif()

    # ### CLI
    if(NOT TARGET CLI11::CLI11)
        cpmaddpackage("gh:CLIUtils/CLI11@2.5.0")
    endif()

    # ### ftxui
    if(NOT TARGET ftxui::screen)
        cpmaddpackage("gh:ArthurSonzogni/FTXUI@6.0.2")
    endif()

    # ### tools
    if(NOT TARGET tools::tools)
        cpmaddpackage("gh:lefticus/tools#update_build_system")
    endif()


    #set(SYSROOT ${CMAKE_CURRENT_BINARY_DIR}/EXTERNAL_PROJECT_INSTALLED_FILES)
    # ### ZLIB
    #    if(NOT TARGET zlib::zlib)
    #        set(ZLIB_REPO "https://github.com/madler/zlib.git"
    #                CACHE STRING "ZLIB repository location."
    #        )
    #        ExternalProject_Add(
    #                external_project_zlib
    #                GIT_REPOSITORY ${ZLIB_REPO}
    #                GIT_TAG "v1.3.1"
    #                # --Update/Patch step----------
    #                UPDATE_COMMAND ""
    #                PATCH_COMMAND ""
    #                INSTALL_COMMAND ""
    #                CMAKE_ARGS
    #                -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}
    #                -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
    #                #-DCMAKE_POLICY_DEFAULT_CMP0042:STRING=NEW
    #        )
    #        #  ExternalProject_Get_Property(external_project_zlib TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
    #        #  set(ZIB_LIBRARY zlib)
    #        #  set(ZLIB_INCLUDE_DIRS ${SOURCE_DIR}/include)
    #        #  set(ZLIB_LIBRARIES ${BINARY_DIR})
    #    endif()




    if (NOT TARGET openimageio::openimageio)
        set(OPENIMAGEIO_REPO "https://github.com/AcademySoftwareFoundation/OpenImageIO"
                CACHE STRING "OPENIMAGEIO repository location."
        )
        # Doesn't work
#        cpmaddpackage(
#                NAME
#                openimageio
#                VERSION
#                3.0.8.1
#                GITHUB_REPOSITORY
#                "AcademySoftwareFoundation/OpenImageIO"
#                OPTIONS
#                "BUILD_TESTING OFF"
#                "BUILD_DOCS OFF"
#                "INSTALL_DOCS OFF"
#                "INSTALL_FONTS OFF"
#                "OIIO_BUILD_TOOLS 0"
#                "USE_PYTHON 0"
#                "OIIO_INTERNALIZE_FMT OFF"
#                "OpenImageIO_BUILD_MISSING_DEPS all"
#                "BUILD_SHARED_LIBS 0"
#                "LINKSTATIC 1"
#        )#"CMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}")

        ExternalProject_Add(openimageio
                GIT_REPOSITORY ${OPENIMAGEIO_REPO}
                GIT_TAG "v3.0.8.1"
                # --Update/Patch step----------
                UPDATE_COMMAND ""
                PATCH_COMMAND ""
                INSTALL_COMMAND ""
                CMAKE_ARGS
                #-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}
                -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
                -DBUILD_TESTING=OFF
                -DBUILD_DOCS=OFF
                -DINSTALL_DOCS=OFF
                -DINSTALL_FONTS=OFF
                -DOIIO_BUILD_TOOLS=0
                -DUSE_PYTHON=0
                -DOpenImageIO_BUILD_MISSING_DEPS=all
                -DBUILD_SHARED_LIBS=0
                -DLINKSTATIC=1
        )
    endif()

    # ### XSIMD

    CPMAddPackage(
            NAME xsimd
            GIT_TAG 13.2.0
            GITHUB_REPOSITORY
            "QuantStack/xsimd"
            DOWNLOAD_ONLY True
    )
    if(xsimd_ADDED)
        # Define the header-only xsimd target
        add_library(xsimd::xsimd INTERFACE IMPORTED GLOBAL)
        target_include_directories(xsimd::xsimd SYSTEM INTERFACE ${xsimd_SOURCE_DIR})

        SET(XSIMD_INCLUDE_DIRECTORY "${xsimd_SOURCE_DIR}/include" PARENT_SCOPE)
    endif()

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

    #list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(STATUS "└─ End Dependencies.cmake")
endfunction()
