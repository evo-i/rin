# Fetch cargs
include(FetchContent)
FetchContent_Declare(
    cargs
    GIT_REPOSITORY https://github.com/likle/cargs.git
    GIT_TAG v1.2.0
)
FetchContent_MakeAvailable(cargs)

# swae executable
add_executable(swae
  swae.c
  plugin_loader.c
)

target_include_directories(swae PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(swae PRIVATE
  cargs
)

if(WIN32)
  # Windows doesn't need dl
else()
  target_link_libraries(swae PRIVATE dl)
endif()

# Installation rules
install(TARGETS swae
  RUNTIME DESTINATION bin
  COMPONENT swae
)

# Install plugins directory relative to swae executable
# This creates a 'plugins' directory next to the swae binary
# Only install soul-worker plugin, exclude dummy
# Runtime dependencies (DLLs) are already in plugins/soul-worker/ directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/plugins/
  DESTINATION bin/plugins
  COMPONENT swae
  FILES_MATCHING
  PATTERN "*.dll"
  PATTERN "*.so"
  PATTERN "*.dylib"
  PATTERN "CMakeFiles" EXCLUDE
  PATTERN "*.cmake" EXCLUDE
  PATTERN "*.obj" EXCLUDE
  PATTERN "*.o" EXCLUDE
  PATTERN "dummy" EXCLUDE
)
install(TARGETS swae DESTINATION bin)
