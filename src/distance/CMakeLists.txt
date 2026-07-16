set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/distance)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/distance)

set(PoutreDISTSRC_DETAILS
        ${subdirheader}/details/mdistance_t.hpp
        ${subdirheader}/details/geodesic_distance_t.hpp
)

set(PoutreDISTSRC_PUBLICHEADERS
        ${subdirheader}/distance.hpp
        ${subdirheader}/mdistance.hpp
        ${subdirheader}/geodesic_distance.hpp
)

set(PoutreDISTSRC_CPP
        ${subdirsource}/mdistance.cpp
        ${subdirsource}/geodesic_distance.cpp
)

source_group(details FILES ${PoutreDISTSRC_DETAILS})
source_group(src FILES ${PoutreDISTSRC_CPP})
source_group(header FILES ${PoutreDISTSRC_PUBLICHEADERS})

set(PoutreDISTSRC ${PoutreDISTSRC_DETAILS}
        ${PoutreDISTSRC_CPP}
        ${PoutreDISTSRC_PUBLICHEADERS})


add_library(poutre_distance ${PoutreDISTSRC})
add_library(poutre_distance::poutre_distance ALIAS poutre_distance)


target_link_libraries(poutre_distance PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PRIVATE spdlog::spdlog
        PUBLIC poutre_base::poutre_base
        PUBLIC poutre_pixel_processing::poutre_pixel_processing
)


# force custom target before start
target_include_directories(poutre_distance ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_distance PUBLIC cxx_std_23)

set_target_properties(
        poutre_distance
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)
