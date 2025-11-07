cmake_minimum_required(VERSION 3.16)

set(SW_PLUGIN_SOURCES
  sw_plugin.c
  sw_private.c
)

include(FetchContent)

FetchContent_Declare(
  libzip
  GIT_REPOSITORY https://github.com/nih-at/libzip.git
  GIT_TAG v1.10.1  # Последняя стабильная версия
  GIT_SHALLOW TRUE
)

set(BUILD_TOOLS OFF CACHE BOOL "Build libzip tools" FORCE)
set(BUILD_REGRESS OFF CACHE BOOL "Build regression tests" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(BUILD_DOC OFF CACHE BOOL "Build documentation" FORCE)
set(ENABLE_COMMONCRYPTO OFF CACHE BOOL "Enable use of CommonCrypto" FORCE)
set(ENABLE_GNUTLS OFF CACHE BOOL "Enable use of GnuTLS" FORCE)
set(ENABLE_MBEDTLS OFF CACHE BOOL "Enable use of mbed TLS" FORCE)
set(ENABLE_OPENSSL ON CACHE BOOL "Enable use of OpenSSL" FORCE)
set(ENABLE_WINDOWS_CRYPTO OFF CACHE BOOL "Enable use of Windows cryptography libraries" FORCE)
set(ENABLE_BZIP2 ON CACHE BOOL "Enable use of BZip2" FORCE)
set(ENABLE_LZMA ON CACHE BOOL "Enable use of LZMA" FORCE)
set(ENABLE_ZSTD ON CACHE BOOL "Enable use of Zstandard" FORCE)

set(ZIP_DOXYGEN OFF CACHE BOOL "Generate API documentation" FORCE)
if(MINGW)
  add_compile_definitions(ZIP_STATIC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types")
  add_compile_definitions(_WIN32_WINNT=0x0600)
  set(ENABLE_WINDOWS_CRYPTO OFF CACHE BOOL "Disable Windows crypto to avoid UTF-16 issues" FORCE)
endif()

FetchContent_MakeAvailable(libzip)

# Определяем тип сборки плагина
if(WIN32 AND NOT BUILD_SHARED_LIBS)
    add_library(soul_worker STATIC ${SW_PLUGIN_SOURCES})
else()
    add_library(soul_worker MODULE ${SW_PLUGIN_SOURCES})
endif()

target_include_directories(soul_worker
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(soul_worker
  PRIVATE libzip::zip
)

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

add_test(
  NAME soul_worker_plugin_test
  COMMAND test_sw_plugin "$ENV{SW_TEST_PATH}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

set_tests_properties(soul_worker_plugin_test PROPERTIES
  TIMEOUT 30
  LABELS "plugin;soul_worker"
)

