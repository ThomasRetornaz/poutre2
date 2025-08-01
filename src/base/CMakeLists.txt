set(subdirheader ${PROJECT_SOURCE_DIR}/include/poutre/base)
set(subdirsource ${PROJECT_SOURCE_DIR}/src/base)

set(PoutreBaseSRC_DETAILS
        ${subdirheader}/details/simd/simd_helpers.hpp
        ${subdirheader}/details/simd/simd_algorithm.hpp
        ${subdirheader}/details/threading/threading_helpers.hpp
        ${subdirheader}/details/data_structures/array_view.hpp
        ${subdirheader}/details/data_structures/pq.hpp
        ${subdirheader}/details/data_structures/image_t.hpp
)

set(PoutreBaseSRC_PUBLICHEADERS
        ${subdirheader}/base.hpp
        ${subdirheader}/config.hpp
        ${subdirheader}/types.hpp
        ${subdirheader}/types_traits.hpp
        ${subdirheader}/json.hpp
        ${subdirheader}/trace.hpp
        ${subdirheader}/registrar.hpp
        ${subdirheader}/chronos.hpp
        ${subdirheader}/image_interface.hpp)

set(PoutreBaseSRC_CPP
        ${subdirsource}/json.cpp
        ${subdirsource}/trace.cpp
        ${subdirsource}/types.cpp
        ${subdirsource}/chronos.cpp
        ${subdirsource}/threading_helpers.cpp
        ${subdirsource}/image_interface.cpp
        ${subdirsource}/image_t.cpp
)

source_group(details FILES ${PoutreBaseSRC_DETAILS})
source_group(src FILES ${PoutreBaseSRC_CPP})
source_group(header FILES ${PoutreBaseSRC_PUBLICHEADERS})

set(PoutreBaseSRC ${PoutreBaseSRC_DETAILS}
        ${PoutreBaseSRC_CPP}
        ${PoutreBaseSRC_PUBLICHEADERS})


# add_definitions(-DMDSPAN_IMPL_STANDARD_NAMESPACE=Kokos)

add_library(poutre_base ${PoutreBaseSRC})
add_library(poutre_base::poutre_base ALIAS poutre_base)


target_link_libraries(poutre_base PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PRIVATE jsoncpp_lib
        PRIVATE spdlog::spdlog
        INTERFACE xsimd::xsimd
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(poutre_base PUBLIC OpenMP::OpenMP_CXX)
endif()


find_package(Threads REQUIRED)
target_link_libraries(poutre_base
        PUBLIC Threads::Threads)


# set_source_files_properties(
#   ${subdirsource}/image_t.cpp
#   PROPERTIES
#   COMPILE_FLAGS "-w"
# )

# force custom target before start
add_dependencies(poutre_base xsimd::xsimd)
target_include_directories(poutre_base ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(poutre_base PUBLIC cxx_std_23)

IF(BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES(poutre_base PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

set_target_properties(
        poutre_base
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)