# Enable doxygen doc builds of source
function(poutre2_enable_doxygen DOXYGEN_THEME)
  # If not specified, use the top readme file as the first page
  if((NOT DOXYGEN_USE_MDFILE_AS_MAINPAGE) AND EXISTS "${PROJECT_SOURCE_DIR}/README.md")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
  endif()

  # FIXME

  # get_filename_component(POUTRE_root_dir_abs ${PROJECT_SOURCE_DIR} ABSOLUTE)
  # set(POUTRE_DOCUMENTATION_ROOT ${POUTRE_root_dir_abs}/doc)
  # set(POUTRE_DOXYGEN_PATH ${POUTRE_DOCUMENTATION_ROOT}/Doxygen)

  # find_file(DOXYGEN_CONFIG_FILE Doxyfile.in ${POUTRE_DOXYGEN_PATH})
  # mark_as_advanced(DOXYGEN_CONFIG_FILE)

  # set(DOXY_OUTPUT_DIR           ${CMAKE_BINARY_DIR}/documentation/doxygen)
  # set(DOXY_CONFIG_FILE          ${DOXY_OUTPUT_DIR}/Doxyfile.generated)

  # set(DOXY_STRIP_FROM_PATH      ${POUTRE_root_dir_abs})
  # set(DOXY_STRIP_FROM_INC_PATH  ${POUTRE_root_dir_abs})
    
  # option(DOXYGEN_GENERATE_XML   "Generates xml documentation"   NO)
  # option(DOXYGEN_GENERATE_LATEX "Generates LateX documentation" NO)
  # option(DOXYGEN_GENERATE_HTML  "Generates HTML documentation"  YES)
  
  # mark_as_advanced(DOXYGEN_GENERATE_HTML)
  # mark_as_advanced(DOXYGEN_GENERATE_XML)
  # mark_as_advanced(DOXYGEN_GENERATE_HTML)

  # mark_as_advanced(DOXYGEN_HTML_HEADER)
  # mark_as_advanced(DOXYGEN_HTML_FOOTER)
  # mark_as_advanced(DOXYGEN_HTML_STYLESHEET)
 

  # set better defaults for doxygen
  is_verbose(_is_verbose)
  if(NOT ${_is_verbose})
    set(DOXYGEN_QUIET YES)
  endif()
  set(DOXYGEN_CALLER_GRAPH YES)
  set(DOXYGEN_CALL_GRAPH YES)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_GENERATE_TREEVIEW YES)
  # svg files are much smaller than jpeg and png, and yet they have higher quality
  set(DOXYGEN_DOT_IMAGE_FORMAT svg)
  set(DOXYGEN_DOT_TRANSPARENT YES)

  # If not specified, exclude the vcpkg files and the files CMake downloads under _deps (like project_options)
  if(NOT DOXYGEN_EXCLUDE_PATTERNS)
    set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_CURRENT_BINARY_DIR}/*" "${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/*" "${CMAKE_CURRENT_BINARY_DIR}/_deps/*")
  endif()

  if("${DOXYGEN_THEME}" STREQUAL "")
    set(DOXYGEN_THEME "awesome-sidebar")
  endif()

  if("${DOXYGEN_THEME}" STREQUAL "awesome" OR "${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
    # use a modern doxygen theme
    # https://github.com/jothepro/doxygen-awesome-css v1.6.1
    FetchContent_Declare(_doxygen_theme
                         URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v1.6.1.zip)
    FetchContent_MakeAvailable(_doxygen_theme)
    if("${DOXYGEN_THEME}" STREQUAL "awesome" OR "${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
      set(DOXYGEN_HTML_EXTRA_STYLESHEET "${_doxygen_theme_SOURCE_DIR}/doxygen-awesome.css")
    endif()
    if("${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
      set(DOXYGEN_HTML_EXTRA_STYLESHEET ${DOXYGEN_HTML_EXTRA_STYLESHEET}
                                        "${_doxygen_theme_SOURCE_DIR}/doxygen-awesome-sidebar-only.css")
    endif()
  else()
    # use the original doxygen theme
  endif()

  # find doxygen and dot if available
  find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

  # add doxygen-docs target
  message(STATUS "Adding `doxygen-docs` target that builds the documentation.")
  doxygen_add_docs(doxygen-docs ALL ${PROJECT_SOURCE_DIR}/include
                   COMMENT "Generating documentation - entry file: ${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
                   #CONFIG_FILE ${DOXYGEN_CONFIG_FILE}
                   )
endfunction()
