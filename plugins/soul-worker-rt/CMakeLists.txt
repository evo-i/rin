cmake_minimum_required(VERSION 3.16)

add_library(soul_worker_rt SHARED
  soul_worker_rt_plugin.c
)

target_include_directories(soul_worker_rt
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(soul_worker_rt PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/soul-worker-rt
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/soul-worker-rt
)

add_executable(test_c test-c.c)

target_link_libraries(test_c PRIVATE soul_worker_rt
  advapi32
)
