set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/structuring_element)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/structuring_element)

set(PoutreSESRC_DETAILS
        ${subdirheader}/details/neighbor_list_se_t.hpp
        ${subdirheader}/details/neighbor_list_static_se_t.hpp
)

set(PoutreSESRC_PUBLICHEADERS
        ${subdirheader}/structuring_element.hpp
        ${subdirheader}/se_interface.hpp
        ${subdirheader}/predefined_nl_se.hpp
        ${subdirheader}/se_chained.hpp
        ${subdirheader}/se_types_and_tags.hpp
)

set(PoutreSESRC_CPP
        ${subdirsource}/predefined_nl_se.cpp
        ${subdirsource}/se_chained.cpp
)

source_group(details FILES ${PoutreSESRC_DETAILS})
source_group(src FILES ${PoutreSESRC_CPP})
source_group(header FILES ${PoutreSESRC_PUBLICHEADERS})

set(PoutreSESRC ${PoutreSESRC_DETAILS}
        ${PoutreSESRC_CPP}
        ${PoutreSESRC_PUBLICHEADERS})


add_library(poutre_structuring_element ${PoutreSESRC})
add_library(poutre_structuring_element::poutre_structuring_element ALIAS poutre_structuring_element)


target_link_libraries(poutre_structuring_element PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PRIVATE spdlog::spdlog
        PUBLIC poutre_base::poutre_base
)


# force custom target before start
target_include_directories(poutre_structuring_element ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_structuring_element PUBLIC cxx_std_23)

set_target_properties(
        poutre_structuring_element
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)
