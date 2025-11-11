find_package(PkgConfig REQUIRED)
pkg_check_modules(GLib REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GObject REQUIRED IMPORTED_TARGET gobject-2.0)

set(TEST_SOURCES
  test-rin-registry.c
)

add_executable(test-rin-registry ${TEST_SOURCES})

target_include_directories(test-rin-registry
  PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test-rin-registry
  PRIVATE rin
          PkgConfig::GLib
          PkgConfig::GObject
)

target_compile_options(test-rin-registry PRIVATE ${GLIB_CFLAGS_OTHER})

add_test(NAME rin-registry-test COMMAND test-rin-registry)

add_dependencies(test-rin-registry rin soul_worker dummy_plugin)
