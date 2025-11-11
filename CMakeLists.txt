cmake_minimum_required(VERSION 3.16)

project(rin 
    VERSION 1.0.0
    DESCRIPTION "Soul Worker Archive Extractor"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

enable_testing()

add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(utils)

# CPack configuration
set(CPACK_PACKAGE_NAME "swae")
set(CPACK_PACKAGE_VENDOR "evo-i")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Standalone Soul Worker Archive Extractor with plugin support")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "swae")

# Install only swae component
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${PROJECT_NAME};swae;/")

# Component configuration
set(CPACK_COMPONENTS_ALL swae)
set(CPACK_COMPONENT_SWAE_DISPLAY_NAME "SWAE - Soul Worker Archive Extractor")
set(CPACK_COMPONENT_SWAE_DESCRIPTION "Standalone archive extractor utility with Soul Worker plugin")
set(CPACK_COMPONENT_SWAE_REQUIRED TRUE)

# Resource files
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/utils/swae/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/utils/swae/README.md")

# Platform-specific settings
if(WIN32)
    # Windows: NSIS installer
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "SWAE - Soul Worker Archive Extractor")
    set(CPACK_NSIS_PACKAGE_NAME "SWAE")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_HELP_LINK "https://github.com/evo-i/rin")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/evo-i/rin")
    set(CPACK_NSIS_CONTACT "https://github.com/evo-i/rin/issues")
elseif(APPLE)
    # macOS: DragNDrop DMG
    set(CPACK_GENERATOR "ZIP;DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "SWAE")
    set(CPACK_DMG_FORMAT "UDZO")
else()
    # Linux: TGZ, DEB, RPM
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
    
    # DEB-specific
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "evo-i")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libzip4 | libzip5")
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    
    # RPM-specific
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/Archiving")
    set(CPACK_RPM_PACKAGE_REQUIRES "libzip >= 1.0")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
endif()

include(CPack)
