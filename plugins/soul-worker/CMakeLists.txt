cmake_minimum_required(VERSION 3.16)
set(ENV{SW_TEST_PATH} "")

set(SW_PLUGIN_SOURCES
  sw_plugin.c
  sw_private.c
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

add_library(soul_worker MODULE ${SW_PLUGIN_SOURCES})

target_include_directories(soul_worker
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${LIBZIP_INCLUDE_DIRS}
)

target_link_libraries(soul_worker
  PRIVATE ${LIBZIP_LIBRARIES}
)

target_compile_options(soul_worker PRIVATE ${LIBZIP_CFLAGS_OTHER})

set_target_properties(soul_worker PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

# Test executable
add_executable(test_sw_plugin test_plugin.c sw_private.c)

target_include_directories(test_sw_plugin
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(test_sw_plugin PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Add test
if(CMAKE_TESTING_ENABLED OR BUILD_TESTING)
  enable_testing()
  
  # Test requires Soul Worker installation path
  # Set SW_TEST_PATH environment variable to enable test
  if(DEFINED ENV{SW_TEST_PATH})
    add_test(
      NAME soul_worker_plugin_test
      COMMAND test_sw_plugin "$ENV{SW_TEST_PATH}"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )
    
    set_tests_properties(soul_worker_plugin_test PROPERTIES
      TIMEOUT 30
      LABELS "plugin;soul_worker"
    )
  else()
    message(STATUS "Soul Worker test disabled. Set SW_TEST_PATH environment variable to enable.")
  endif()
endif()
