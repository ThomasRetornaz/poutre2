include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(poutre2_setup_dependencies)
    message(STATUS "┌─ Dependencies.cmake")
    #list(APPEND CMAKE_MESSAGE_INDENT "│    ")
    # ### XSIMD
    if (NOT TARGET xsimd::xsimd)
        CPMAddPackage(
                NAME xsimd
                GIT_TAG 14.3.0
                GITHUB_REPOSITORY
                "QuantStack/xsimd"
                #DOWNLOAD_ONLY True
        )
        if (xsimd_ADDED)
            # Define the header-only xsimd target
            #add_library(xsimd::xsimd INTERFACE IMPORTED GLOBAL)
            #target_include_directories(xsimd::xsimd SYSTEM BEFORE INTERFACE ${xsimd_SOURCE_DIR}/include)
        endif ()
    endif ()

    # ### GoogleBenchmark
    if (NOT TARGET googlebenchmark::googlebenchmark)
        cpmaddpackage(
                NAME googlebenchmark
                GITHUB_REPOSITORY google/benchmark

                # VERSION ${ogs.minimum_version.gtest}
                GIT_TAG v1.9.5

                OPTIONS "BENCHMARK_ENABLE_GTEST_TESTS OFF" "BENCHMARK_ENABLE_TESTING OFF"

                # "BUILD_SHARED_LIBS OFF"
                EXCLUDE_FROM_ALL YES
                SYSTEM YES
        )
    endif ()

    # ### Catch2
    if (NOT TARGET Catch2::Catch2WithMain)
        cpmaddpackage("gh:catchorg/Catch2@3.16.1")
    endif ()

    # ### CLI
    if (NOT TARGET CLI11::CLI11)
        cpmaddpackage("gh:CLIUtils/CLI11@2.7.2")
    endif ()

    #provided through mamba env
    find_package(OpenImageIO 3 QUIET)
    if (OpenImageIO_FOUND)
        message(STATUS "OpenImageIO ${OpenImageIO_VERSION} found, image file support enabled")
        set(POUTRE_BUILD_WITH_OIIO ON PARENT_SCOPE)
    else ()
        set(POUTRE_BUILD_WITH_OIIO OFF PARENT_SCOPE)
        message(WARNING "OpenImageIO not found: so no image file support")
    endif ()

    #provided through mamba env
    find_package(HDF5 QUIET COMPONENTS C CXX)
    if (HDF5_FOUND)
        message(STATUS "hdf5 ${HDF5_VERSION} found, hdf5 file support enabled")
        set(POUTRE_BUILD_WITH_HDF5 ON PARENT_SCOPE)
    else ()
        set(POUTRE_BUILD_WITH_HDF5 OFF PARENT_SCOPE)
        message(WARNING "hdf5 not found: so no hdf5 file support")
    endif ()

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
                SYSTEM YES
        )
        if (BS_thread_pool_ADDED)
            add_library(BS_thread_pool INTERFACE)
            target_include_directories(BS_thread_pool INTERFACE ${BS_thread_pool_SOURCE_DIR}/include)
        endif ()
    endif ()

    # generic graff library
    if (NOT TARGET graaf)
        CPMAddPackage(
                NAME graaf
                GITHUB_REPOSITORY bobluppes/graaf
                VERSION 1.1.1
                EXCLUDE_FROM_ALL
                SYSTEM YES
                OPTIONS
                "SKIP_EXAMPLES ON"
                "SKIP_TESTS ON"
                "SKIP_BENCHMARKS ON"
        )
        if (graaf_ADDED)
            add_library(graaf INTERFACE)
            target_include_directories(graaf INTERFACE ${graaf_SOURCE_DIR}/include)
        endif ()
    endif ()
    message(STATUS "└─ End Dependencies.cmake")
endfunction()
