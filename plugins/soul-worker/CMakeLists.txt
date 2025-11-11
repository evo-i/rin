cmake_minimum_required(VERSION 3.16)

set(SW_PLUGIN_SOURCES
  soul_worker_plugin.c
  soul_worker_private.c
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(libzip REQUIRED IMPORTED_TARGET libzip)

add_library(soul_worker MODULE ${SW_PLUGIN_SOURCES})

target_include_directories(soul_worker
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(soul_worker
  PRIVATE PkgConfig::libzip
)

set_target_properties(soul_worker PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/soul-worker
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/soul-worker
)
