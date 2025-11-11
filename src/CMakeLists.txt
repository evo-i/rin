set(RIN_SOURCES
  rin-registry.c
  rin-registry-entry.c
)

set(RIN_HEADERS
  rin-registry.h
  rin-registry-entry.h
  rin-plugin-api.h
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLib REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GObject REQUIRED IMPORTED_TARGET gobject-2.0)
pkg_check_modules(GIO REQUIRED IMPORTED_TARGET gio-2.0)
pkg_check_modules(GModule REQUIRED IMPORTED_TARGET gmodule-2.0)

add_library(rin SHARED ${RIN_SOURCES} ${RIN_HEADERS})

target_include_directories(rin
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
         ${GLIB_INCLUDE_DIRS}
         ${GOBJECT_INCLUDE_DIRS}
         ${GIO_INCLUDE_DIRS}
         ${GMODULE_INCLUDE_DIRS}
)

target_link_libraries(rin
  PRIVATE PkgConfig::GLib
          PkgConfig::GObject
          PkgConfig::GIO
          PkgConfig::GModule
)
