cmake_minimum_required(VERSION 3.16)

set(DUMMY_PLUGIN_SOURCES
  dummy_plugin.c
)

add_library(dummy_plugin MODULE ${DUMMY_PLUGIN_SOURCES})

target_compile_definitions(dummy_plugin PRIVATE RIN_BUILD_PLUGIN)
target_include_directories(dummy_plugin
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(dummy_plugin PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/dummy
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/dummy
)
