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
#    if(NOT TARGET fmtlib::fmtlib)
#        cpmaddpackage("gh:fmtlib/fmt#11.1.4")
#    endif()

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
                "SPDLOG_USE_STD_FORMAT ON"
                "SPDLOG_FMT_EXTERNAL OFF"
        )
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
    if(NOT TARGET ZLIB::ZLIB)
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
        #        ExternalProject_Add(
        #                zlib
        #                GIT_REPOSITORY ${ZLIB_REPO}
        #                GIT_TAG "v1.3.1"
        #                # --Update/Patch step----------
        #                UPDATE_COMMAND ""
        #                PATCH_COMMAND ""
        #                # INSTALL_COMMAND ""
        #                CMAKE_ARGS
        #                -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/zlib_install
        #                -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
        #                -DZLIB_BUILD_EXAMPLES=OFF
        #                #-DCMAKE_POLICY_DEFAULT_CMP0042:STRING=NEW
        #        )
        #        ExternalProject_Get_Property(zlib TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
        #        set(ZIB_LIBRARY zlib)
        #        set(ZLIB_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/zlib_install/include)
        #        set(ZLIB_LIBRARIES_DIRS ${CMAKE_CURRENT_BINARY_DIR}/zlib_install/lib)
    endif()

    find_package(OpenImageIO QUIET)
    if(OpenImageIO_FOUND)
        message(STATUS "OpenImageIO ${OpenImageIO_VERSION} found, image file support enabled")
        set(POUTRE_BUILD_WITH_OIIO ON PARENT_SCOPE)
    else()
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
                "hdf5_1.14.6"
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
        if(hdf5_ADDED)
            add_library(hdf5::hdf5 INTERFACE IMPORTED)
            target_include_directories(
                    hdf5::hdf5 SYSTEM
                    INTERFACE ${hdf5_SOURCE_DIR}/hl/src ${hdf5_SOURCE_DIR}/hl/c++/src
                    ${hdf5_SOURCE_DIR}/src ${hdf5_SOURCE_DIR}/c++/src ${hdf5_BINARY_DIR}/src
                    ${hdf5_SOURCE_DIR}/src/H5FDsubfiling
            )

            # message(STATUS "Cmake added local jsoncpp: ${jsoncpp_SOURCE_DIR}")
        endif()
#        ExternalProject_Add(hdf5
#                GIT_REPOSITORY ${HDF5_REPO}
#                GIT_TAG "hdf5_1.14.6"
#                EXCLUDE_FROM_ALL
#                # --Update/Patch step----------
#                UPDATE_COMMAND ""
#                PATCH_COMMAND ""
#                #INSTALL_COMMAND ""
#                CMAKE_ARGS
#                -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/hdf5_install
#                -DBUILD_STATIC_LIBS=ON
#                -DHDF5_EXTERNALLY_CONFIGURED=ON
#                -DHDF5_GENERATE_HEADERS=OFF
#                -DHDF5_DISABLE_COMPILER_WARNINGS=ON
#                -DHDF5_BUILD_DOC=OFF
#                -DBUILD_TESTING=OFF
#                -DHDF5_BUILD_TOOLS=OFF
#                -DHDF5_ENABLE_PLUGIN_SUPPORT=OFF
#                -DBUILD_SHARED_LIBS=OFF
#                -DHDF5_BUILD_HL_LIB=ON
#                -DHDF5_BUILD_FORTRAN=OFF
#                -DHDF5_BUILD_CPP_LIB=ON
#                -DHDF5_BUILD_JAVA=OFF
#                -DHDF5_BUILD_EXAMPLES=OFF
#        )
#        ExternalProject_Get_Property(hdf5 TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
#        set(HDF5_ROOT ${INSTALL_DIR})
#        set(HDF5_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/include PARENT_SCOPE)
#        # set(HDF5_BINARY_DIRS ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib PARENT_SCOPE)
#
#        if(WIN32) # or import target ?
#            set(HDF5_LIBRARIES
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/hdf5.lib
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/hdf5_cpp.lib
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/hdf5_hl.lib
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/hdf5_hl_cpp.lib)
#            set(HDF5_LIBRARIES ${HDF5_LIBRARIES} PARENT_SCOPE)
#        else()
#            set(HDF5_LIBRARIES
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/libhdf5.a
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/libhdf5_cpp.a
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/libhdf5_hl.a
#                    ${CMAKE_CURRENT_BINARY_DIR}/hdf5_install/lib/libhdf5_hl_cpp.a)
#            set(HDF5_LIBRARIES ${HDF5_LIBRARIES} PARENT_SCOPE)
#        endif()

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
        target_include_directories(xsimd::xsimd SYSTEM BEFORE INTERFACE ${xsimd_SOURCE_DIR}/include)

        # SET(XSIMD_INCLUDE_DIRECTORY "${xsimd_SOURCE_DIR}/include" PARENT_SCOPE)
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
                    BEFORE INTERFACE ${jsoncpp_SOURCE_DIR}/include
            )

            # message(STATUS "Cmake added local jsoncpp: ${jsoncpp_SOURCE_DIR}")
        endif()
    endif()

    #list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(STATUS "└─ End Dependencies.cmake")
endfunction()
