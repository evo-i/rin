cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "" FORCE)
    message(STATUS "Using vcpkg toolchain from vendor/vcpkg")
  endif()
endif()

project(rin LANGUAGES C)

if (NOT CMAKE_TOOLCHAIN_FILE)
  message (STATUS "No toolchain file specified, using default settings.")
endif ()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

enable_testing()

add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(utils)
