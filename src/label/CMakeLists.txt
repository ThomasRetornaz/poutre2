set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/label)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/label)

set(PoutreLABELSRC_DETAILS
        ${subdirheader}/details/label_t.hpp
)

set(PoutreLABELSRC_PUBLICHEADERS
        ${subdirheader}/label.hpp
        ${subdirheader}/label_binary.hpp
        ${subdirheader}/label_flat_zones.hpp
        ${subdirheader}/label_extrema.hpp
)

set(PoutreLABELSRC_CPP
        ${subdirsource}/label_binary.cpp
        ${subdirsource}/label_flat_zones.cpp
        ${subdirsource}/label_extrema.cpp
)

source_group(details FILES ${PoutreLABELSRC_DETAILS})
source_group(src FILES ${PoutreLABELSRC_CPP})
source_group(header FILES ${PoutreLABELSRC_PUBLICHEADERS})

set(PoutreLABELSRC ${PoutreLABELSRC_DETAILS}
        ${PoutreLABELSRC_CPP}
        ${PoutreLABELSRC_PUBLICHEADERS})


add_library(poutre_label ${PoutreLABELSRC})
add_library(poutre_label::poutre_label ALIAS poutre_label)


target_link_libraries(poutre_label PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PRIVATE spdlog::spdlog
        PUBLIC poutre_base::poutre_base
        PUBLIC poutre_pixel_processing::poutre_pixel_processing
        PUBLIC poutre_geodesy::poutre_geodesy
)


# force custom target before start
target_include_directories(poutre_label ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_label PUBLIC cxx_std_23)

set_target_properties(
        poutre_label
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)
