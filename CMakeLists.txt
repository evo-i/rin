cmake_minimum_required(VERSION 3.16)
project(rin LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0 gobject-2.0 gmodule-2.0 gio-2.0)
enable_testing()
add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(utils)
