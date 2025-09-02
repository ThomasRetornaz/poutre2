set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/geodesy)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/geodesy)

set(PoutreGEOSRC_DETAILS
        ${subdirheader}/details/mreconstruct_t.hpp
        ${subdirheader}/details/leveling_t.hpp
)

set(PoutreGEOSRC_PUBLICHEADERS
        ${subdirheader}/extrema.hpp
        ${subdirheader}/leveling.hpp
        ${subdirheader}/geodesy.hpp
        ${subdirheader}/mreconstruct.hpp
)

set(PoutreGEOSRC_CPP
        ${subdirsource}/extrema.cpp
        ${subdirsource}/leveling.cpp
        ${subdirsource}/mreconstruct.cpp
)

source_group(details FILES ${PoutreGEOSRC_DETAILS})
source_group(src FILES ${PoutreGEOSRC_CPP})
source_group(header FILES ${PoutreGEOSRC_PUBLICHEADERS})

set(PoutreGEOSRC ${PoutreGEOSRC_DETAILS}
        ${PoutreGEOSRC_CPP}
        ${PoutreGEOSRC_PUBLICHEADERS})


add_library(poutre_geodesy ${PoutreGEOSRC})
add_library(poutre_geodesy::poutre_geodesy ALIAS poutre_geodesy)


target_link_libraries(poutre_geodesy PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PRIVATE spdlog::spdlog
        PUBLIC poutre_base::poutre_base
)


# force custom target before start
target_include_directories(poutre_geodesy ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_geodesy PUBLIC cxx_std_23)

set_target_properties(
        poutre_geodesy
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)
