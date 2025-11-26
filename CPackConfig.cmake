# CPack Configuration for SWAE
# This file can be used to customize package generation
# Usage: cpack --config CPackConfig.cmake

# Override default generators based on platform
if(NOT CPACK_GENERATOR)
    if(WIN32)
        set(CPACK_GENERATOR "ZIP")
    elseif(APPLE)
        set(CPACK_GENERATOR "DragNDrop")
    else()
        set(CPACK_GENERATOR "TGZ")
    endif()
endif()

# Set custom output directory
if(NOT CPACK_OUTPUT_FILE_PREFIX)
    set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/packages")
endif()

# Additional configuration can be added here
