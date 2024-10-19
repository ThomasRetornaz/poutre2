cmake_minimum_required(VERSION 3.15...3.23)

project(benchmarks LANGUAGES CXX)

# ---- becnhmarks as standalone project the exported config package ----
if(PROJECT_IS_TOP_LEVEL OR BENCH_INSTALLED_VERSION)
  find_package(poutre CONFIG REQUIRED) # for intro, project_options, ...

  if(NOT TARGET poutre_options)
    message(FATAL_ERROR "Required config package not found!")
    return() # be strictly paranoid for Template Janitor github action! CK
  endif()
endif()

# ---- Dependencies ----

add_subdirectory(base)
