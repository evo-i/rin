set(TEST_SOURCES
  test-rin-registry.c
)

add_executable(test-rin-registry ${TEST_SOURCES})

target_link_libraries(test-rin-registry
  PRIVATE rin ${GLIB_LIBRARIES}
)

target_include_directories(test-rin-registry
  PRIVATE ${CMAKE_SOURCE_DIR}/src
          ${GLIB_INCLUDE_DIRS}
)

target_compile_options(test-rin-registry PRIVATE ${GLIB_CFLAGS_OTHER})

enable_testing()
add_test(NAME rin-registry-test COMMAND test-rin-registry)
