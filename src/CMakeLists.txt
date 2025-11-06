set(RIN_SOURCES
  rin-registry.c
  rin-registry-entry.c
)

set(RIN_HEADERS
  rin-registry.h
  rin-registry-entry.h
  rin-plugin-api.h
)

add_library(rin SHARED ${RIN_SOURCES} ${RIN_HEADERS})

target_include_directories(rin
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${GLIB_INCLUDE_DIRS}
)

target_link_libraries(rin
  PRIVATE ${GLIB_LIBRARIES}
)

target_compile_options(rin PRIVATE ${GLIB_CFLAGS_OTHER})
