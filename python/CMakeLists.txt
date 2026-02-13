# find python interpreter and numpy
find_package(Python COMPONENTS Development.Module NumPy REQUIRED)
CPMAddPackage(
        NAME nanobind
        GITHUB_REPOSITORY wjakob/nanobind
        VERSION 2.11.0
        EXCLUDE_FROM_ALL
        SYSTEM
)
add_library(nanobind INTERFACE)
target_include_directories(nanobind INTERFACE ${nanobind_SOURCE_DIR}/include)

# Import nanobind through CMake's find_package mechanism
find_package(nanobind CONFIG REQUIRED)


# We are now ready to compile the actual extension module
nanobind_add_module(
        # Name of the extension
        pypoutre

        # Target the stable ABI for Python 3.12+, which reduces
        # the number of binary wheels that must be built. This
        # does nothing on older Python versions
        STABLE_ABI

        # Build libnanobind statically and merge it into the
        # extension (which itself remains a shared library)
        #
        # If your project builds multiple extensions, you can
        # replace this flag by NB_SHARED to conserve space by
        # reusing a shared libnanobind across libraries
        NB_STATIC

        # Mark the include directories of nanobind and Python as SYSTEM include directories,
        # which suppresses any potential warning messages originating there. This is mainly
        # of relevance if your project artificially raises the warning level via flags like
        # -pedantic, -Wcast-qual, -Wsign-conversion.
        NB_SUPPRESS_WARNINGS


        # Source code goes here
        main.cpp
        base/types.cpp
        base/image_interface.cpp
        io/io.cpp
        pixel_processing/arith.cpp
        pixel_processing/copy_convert.cpp
        pixel_processing/compare.cpp
        structuring_element/se_types.cpp
        structuring_element/se_interface.cpp
        low_level_morpho/ero_dil.cpp
        geodesy/geodesy.cpp
        label/label.cpp
)

target_link_libraries(
        pypoutre
        PRIVATE poutre2_options
        PRIVATE poutre2_warnings
        PRIVATE poutre_base::poutre_base
        PRIVATE poutre_io::poutre_io
        PRIVATE poutre_pixel_processing::poutre_pixel_processing
        PRIVATE poutre_structuring_element::poutre_structuring_element
        PRIVATE poutre_low_level_morpho::poutre_low_level_morpho
        PRIVATE poutre_geodesy::poutre_geodesy
        PRIVATE poutre_label::poutre_label
)
# target_compile_options(pypoutre PUBLIC -Wno-old-style-cast) # overload resolution
# target_compile_options(pypoutre PUBLIC -Wno-error=old-style-cast) # overload resolution