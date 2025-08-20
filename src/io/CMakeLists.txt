set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/io)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/io)

set(PoutreIOSRC_DETAILS
        ${subdirheader}/details/hdf5.hpp
)

if(POUTRE_BUILD_WITH_OIIO)
    LIST(APPEND PoutreIOSRC_DETAILS ${subdirheader}/details/oiio.hpp)
endif()

set(PoutreIOSRC_PUBLICHEADERS
        ${subdirheader}/io.hpp
        ${subdirheader}/loader.hpp
        ${subdirheader}/writer.hpp
)

set(PoutreIOSRC_CPP
        ${subdirsource}/loader.cpp
        ${subdirsource}/writer.cpp
        ${subdirsource}/hdf5.cpp
)

if(POUTRE_BUILD_WITH_OIIO)
    LIST(APPEND PoutreIOSRC_CPP ${subdirsource}/oiio.cpp)
endif()

source_group(details FILES ${PoutreIOSRC_DETAILS})
source_group(src FILES ${PoutreIOSRC_CPP})
source_group(header FILES ${PoutreIOSRC_PUBLICHEADERS})

set(PoutreIOSRC ${PoutreIOSRC_DETAILS}
        ${PoutreIOSRC_CPP}
        ${PoutreIOSRC_PUBLICHEADERS})



add_library(poutre_io ${PoutreIOSRC})
add_library(poutre_io::poutre_io ALIAS poutre_io)

# target_include_directories(poutre_io SYSTEM BEFORE PRIVATE ${HDF5_INCLUDE_DIRS})

if(POUTRE_BUILD_WITH_OIIO)
    target_compile_definitions(poutre_io PUBLIC POUTRE_BUILD_WITH_OIIO)

    target_include_directories(poutre_io
            INTERFACE
            $<BUILD_INTERFACE:${OpenImageIO_INCLUDE_DIRS}>
    )

    target_link_libraries(poutre_io
            PUBLIC
            OpenImageIO::OpenImageIO
            OpenImageIO::OpenImageIO_Util
    )
endif()


target_include_directories(poutre_io
        INTERFACE
        $<BUILD_INTERFACE:${hdf5_INCLUDE_DIRS}>
)
target_link_libraries(poutre_io
        PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PUBLIC poutre_base::poutre_base
        PRIVATE spdlog::spdlog
        PRIVATE hdf5::hdf5
        PRIVATE hdf5-static
        PRIVATE hdf5_cpp-static

)


# force custom target before start
target_include_directories(poutre_io ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_io PUBLIC cxx_std_23)

set_target_properties(
        poutre_io
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)
