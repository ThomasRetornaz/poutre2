include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(poutre2_setup_dependencies)
    message(STATUS "┌─ Dependencies.cmake")
    #list(APPEND CMAKE_MESSAGE_INDENT "│    ")

    include(ExternalProject)

    # ### GoogleBenchmark
    if (NOT TARGET googlebenchmark::googlebenchmark)
        cpmaddpackage(
                NAME googlebenchmark
                GITHUB_REPOSITORY google/benchmark

                # VERSION ${ogs.minimum_version.gtest}
                GIT_TAG v1.9.5

                OPTIONS "BENCHMARK_ENABLE_GTEST_TESTS OFF" "BENCHMARK_ENABLE_TESTING OFF"

                # "BUILD_SHARED_LIBS OFF"
                EXCLUDE_FROM_ALL YES SYSTEM TRUE
        )
    endif ()

    # ### BOOST HISTOGRAM
    #    if(NOT TARGET boost_histogram)
    #        cpmaddpackage(
    #                NAME boost_histogram
    #                GIT_TAG boost-1.89.0
    #                GITHUB_REPOSITORY "boostorg/histogram")
    #    endif()

    # ### SPDLOG
    if (NOT TARGET spdlog::spdlog)
        cpmaddpackage(
                NAME
                spdlog
                VERSION
                1.17.0
                GITHUB_REPOSITORY
                "gabime/spdlog"
                OPTIONS
                "SPDLOG_USE_STD_FORMAT ON"
                "SPDLOG_FMT_EXTERNAL OFF"
        )
    endif ()

    # ### Catch2
    if (NOT TARGET Catch2::Catch2WithMain)
        cpmaddpackage("gh:catchorg/Catch2@3.12.0")
    endif ()

    # ### CLI
    if (NOT TARGET CLI11::CLI11)
        cpmaddpackage("gh:CLIUtils/CLI11@2.6.1")
    endif ()

    # ### ftxui
    if (NOT TARGET ftxui::screen)
        cpmaddpackage("gh:ArthurSonzogni/FTXUI@6.1.9")
    endif ()

    # ### tools
    if (NOT TARGET tools::tools)
        cpmaddpackage("gh:lefticus/tools#update_build_system")
    endif ()


    #set(SYSROOT ${CMAKE_CURRENT_BINARY_DIR}/EXTERNAL_PROJECT_INSTALLED_FILES)
    # ### ZLIB
    if (NOT TARGET ZLIB::ZLIB)
        cpmaddpackage(
                NAME
                zlib
                VERSION
                1.3.1
                GITHUB_REPOSITORY
                "madler/zlib"
                OPTIONS
                "CMAKE_POSITION_INDEPENDENT_CODE ON"
                "ZLIB_BUILD_EXAMPLES OFF"
        )
    endif ()

    find_package(OpenImageIO QUIET)
    if (0)
        message(STATUS "OpenImageIO ${OpenImageIO_VERSION} found, image file support enabled")
        set(POUTRE_BUILD_WITH_OIIO ON PARENT_SCOPE)
    else ()
        message(WARNING "OpenImageIO not found: so no image file support")
    endif ()
    # message(FATAL_ERROR ${OpenImageIO_INCLUDE_DIRS})

    #    if (NOT TARGET openimageio::openimageio)
    #        set(OPENIMAGEIO_REPO "https://github.com/AcademySoftwareFoundation/OpenImageIO"
    #                CACHE STRING "OPENIMAGEIO repository location."
    #        )
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
    #                "OIIO_INTERNALIZE_FMT ON"
    #                "OpenImageIO_BUILD_MISSING_DEPS all"
    #                "BUILD_SHARED_LIBS 0"
    #                "LINKSTATIC 1"
    #        )#"CMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}")
    ##
    ##        ExternalProject_Add(openimageio
    ##                GIT_REPOSITORY ${OPENIMAGEIO_REPO}
    ##                GIT_TAG "v3.0.8.1"
    ##                EXCLUDE_FROM_ALL
    ##                # --Update/Patch step----------
    ##                UPDATE_COMMAND ""
    ##                PATCH_COMMAND ""
    ##                # INSTALL_COMMAND ""
    ##                CMAKE_ARGS
    ##                -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/oiio_install
    ##                -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
    ##                -DBUILD_TESTING=OFF
    ##                -DBUILD_DOCS=OFF
    ##                -DINSTALL_DOCS=OFF
    ##                -DINSTALL_FONTS=OFF
    ##                -DOIIO_BUILD_TOOLS=0
    ##                -DUSE_PYTHON=0
    ##                -DOpenImageIO_BUILD_MISSING_DEPS=all
    ##                -DBUILD_SHARED_LIBS=0
    ##                -DLINKSTATIC=1
    ##        )
    #
    ##        ExternalProject_Get_Property(openimageio TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
    ##        set (OpenImageIO_ROOT ${BINARY_DIR})
    ##        set(OIIO_INCLUDE_DIRS ${OpenImageIO_ROOT}/include PARENT_SCOPE)
    ##        if(WIN32) # OR import static ?
    ##            set(OIIO_LIBRARIES ${OpenImageIO_ROOT}/lib/OpenImageIO.lib ${OpenImageIO_ROOT}/lib/OpenImageIO_Util.lib)
    ##            set(OIIO_LIBRARIES ${OIIO_LIBRARIES} PARENT_SCOPE)
    ##        else()
    ##            set(OIIO_LIBRARIES
    ##                    ${OpenImageIO_ROOT}/lib/libOpenImageIO.a
    ##                    ${OpenImageIO_ROOT}/lib/libOpenImageIO_Util.a
    ##            )
    ##            set(OIIO_LIBRARIES ${OIIO_LIBRARIES} PARENT_SCOPE)
    ##        endif()
    #
    #    endif()

    if (NOT TARGET hdf5::hdf5)
        cpmaddpackage(
                NAME
                hdf5
                GIT_TAG
                "hdf5_2.0.0"
                GITHUB_REPOSITORY
                "HDFGroup/hdf5"
                OPTIONS
                "BUILD_STATIC_LIBS ON"
                "HDF5_EXTERNALLY_CONFIGURED ON"
                "HDF5_GENERATE_HEADERS OFF"
                "HDF5_DISABLE_COMPILER_WARNINGS ON"
                "HDF5_BUILD_DOC OFF"
                "BUILD_TESTING OFF"
                "HDF5_BUILD_TOOLS OFF"
                "HDF5_ENABLE_PLUGIN_SUPPORT OFF"
                "BUILD_SHARED_LIBS OFF"
                "HDF5_BUILD_HL_LIB ON"
                "HDF5_BUILD_FORTRAN OFF"
                "HDF5_BUILD_CPP_LIB ON"
                "HDF5_BUILD_JAVA OFF"
                "HDF5_BUILD_EXAMPLES OFF"
                "ZLIB_USE_EXTERNAL ON" # BUG dependencies on zlib not found
        )
        if (hdf5_ADDED)
            add_library(hdf5::hdf5 INTERFACE IMPORTED)
            target_include_directories(
                    hdf5::hdf5 SYSTEM
                    INTERFACE ${hdf5_SOURCE_DIR}/hl/src ${hdf5_SOURCE_DIR}/hl/c++/src
                    ${hdf5_SOURCE_DIR}/src ${hdf5_SOURCE_DIR}/c++/src ${hdf5_BINARY_DIR}/src
                    ${hdf5_SOURCE_DIR}/src/H5FDsubfiling
            )
        endif ()
    endif ()

    # ### XSIMD
    if (NOT TARGET xsimd::xsimd)
        CPMAddPackage(
                NAME xsimd
                GIT_TAG 14.0.0
                GITHUB_REPOSITORY
                "QuantStack/xsimd"
                #DOWNLOAD_ONLY True
        )
        if (xsimd_ADDED)
            # Define the header-only xsimd target
            add_library(xsimd::xsimd INTERFACE IMPORTED GLOBAL)
            target_include_directories(xsimd::xsimd SYSTEM BEFORE INTERFACE ${xsimd_SOURCE_DIR}/include)
        endif ()
    endif ()

    if (NOT TARGET json::json)
        # ### nolhmann json
        CPMAddPackage(
                NAME json
                GIT_TAG "v3.12.0"
                GITHUB_REPOSITORY
                "nlohmann/json"
                OPTIONS
                "JSON_BuildTests OFF"
                "JSON_CI OFF"
                "JSON_Diagnostics OFF"
                "JSON_Install OFF"
        )
        if (json_ADDED)
            add_library(json::json INTERFACE IMPORTED)
            target_include_directories(
                    json::json SYSTEM
                    BEFORE INTERFACE ${json_SOURCE_DIR}/include
            )
        endif ()
    endif ()

    # generic threadpool
    if (NOT TARGET BS_thread_pool)
        CPMAddPackage(
                NAME BS_thread_pool
                GITHUB_REPOSITORY bshoshany/thread-pool
                VERSION 5.1.0
                EXCLUDE_FROM_ALL
                SYSTEM
        )
        add_library(BS_thread_pool INTERFACE)
        target_include_directories(BS_thread_pool INTERFACE ${BS_thread_pool_SOURCE_DIR}/include)
    endif ()

    # generic graff library
    if (NOT TARGET graaf)
        CPMAddPackage(
                NAME graaf
                GITHUB_REPOSITORY bobluppes/graaf
                VERSION 1.1.1
                EXCLUDE_FROM_ALL
                SYSTEM
                OPTIONS
                "SKIP_EXAMPLES ON"
                "SKIP_TESTS ON"
                "SKIP_BENCHMARKS ON"
        )
        add_library(graaf INTERFACE)
        target_include_directories(graaf INTERFACE ${graaf_SOURCE_DIR}/include)
    endif ()

    #list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(STATUS "└─ End Dependencies.cmake")
endfunction()
