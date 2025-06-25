set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/low_level_morpho)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/low_level_morpho)

set(PoutreLLMSRC_DETAILS
        ${subdirheader}/details/ero_dil_static_se_t.hpp
        ${subdirheader}/details/ero_dil_runtime_nl_se_t.hpp
)

set(PoutreLLMSRC_PUBLICHEADERS
        ${subdirheader}/low_level_morpho.hpp
        ${subdirheader}/ero_dil.hpp
)

set(PoutreLLMSRC_CPP
        ${subdirsource}/ero_dil_static.cpp
        ${subdirsource}/ero_dil.cpp
)

source_group(details FILES ${PoutreLLMSRC_DETAILS})
source_group(src FILES ${PoutreLLMSRC_CPP})
source_group(header FILES ${PoutreLLMSRC_PUBLICHEADERS})

set(PoutreLLMSRC ${PoutreLLMSRC_DETAILS}
        ${PoutreLLMSRC_CPP}
        ${PoutreLLMSRC_PUBLICHEADERS})


add_library(poutre_low_level_morpho ${PoutreLLMSRC})
add_library(poutre_low_level_morpho::poutre_low_level_morpho ALIAS poutre_low_level_morpho)


target_link_libraries(poutre_low_level_morpho PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PRIVATE spdlog::spdlog
        PUBLIC poutre_base::poutre_base
        PUBLIC poutre_pixel_processing::poutre_pixel_processing
        PUBLIC poutre_structuring_element::poutre_structuring_element
)


# force custom target before start
target_include_directories(poutre_low_level_morpho ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_low_level_morpho PUBLIC cxx_std_23)

set_target_properties(
        poutre_low_level_morpho
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)
