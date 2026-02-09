#
# CPack configuration for cross-platform packaging
#

# Include CPack module
include(CPack)

# Set package information
set(CPACK_PACKAGE_NAME "NutStash")
set(CPACK_PACKAGE_VENDOR "NutStash")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NutStash - Git Repository Manager")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "NutStash")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")

# Set package file name
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

# Set install directory
set(CPACK_PACKAGE_INSTALL_DIRECTORY "NutStash")

#
# Windows MSI configuration
#
if(WIN32)
  set(CPACK_GENERATOR "WIX;ZIP")
  
  # Set WiX Toolset path (prefer v3.x for CPack compatibility)
  if(EXISTS "C:/Program Files (x86)/WiX Toolset v3.14")
    set(CPACK_WIX_ROOT "C:/Program Files (x86)/WiX Toolset v3.14")
  elseif(EXISTS "C:/Program Files/WiX Toolset v6.0")
    set(CPACK_WIX_ROOT "C:/Program Files/WiX Toolset v6.0")
  endif()
  
  # WIX specific settings
  set(CPACK_WIX_UPGRADE_GUID "A1B2C3D4-E5F6-4A5B-8C9D-0E1F2A3B4C5D")
  set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/nutstash/nutstash/resources/logo.png")
  set(CPACK_WIX_UI_BANNER "${CMAKE_SOURCE_DIR}/nutstash/nutstash/resources/logo.png")
  set(CPACK_WIX_UI_DIALOG "${CMAKE_SOURCE_DIR}/nutstash/nutstash/resources/logo.png")
  
  # MSI properties
  set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "https://github.com/yourusername/nutstash")
  set(CPACK_WIX_PROPERTY_ARPHELPLINK "https://github.com/yourusername/nutstash/issues")
  set(CPACK_WIX_PROPERTY_ARPPRODUCTICON "${CPACK_WIX_PRODUCT_ICON}")
  
  # WIX will automatically create Start Menu shortcuts for installed executables
endif()

#
# macOS DMG configuration
#
if(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
  
  # DMG specific settings
  set(CPACK_DMG_VOLUME_NAME "NutStash")
  set(CPACK_DMG_FORMAT "UDZO")
  set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/nutstash/nutstash/resources/logo.png")
  set(CPACK_DMG_DS_STORE_SETUP_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/cmake/SetUpDSStore.scpt")
  
  # Set DMG window size and position
  set(CPACK_DMG_WINDOW_BACKGROUND_COLOR "FFFFFF")
  set(CPACK_DMG_WINDOW_POSITION_X "200")
  set(CPACK_DMG_WINDOW_POSITION_Y "200")
  set(CPACK_DMG_WINDOW_SIZE_WIDTH "600")
  set(CPACK_DMG_WINDOW_SIZE_HEIGHT "400")
  
  # Create Applications symlink
  set(CPACK_DMG_APPLICATIONS_LINK "ON")
endif()

#
# Linux configuration (optional, for future use)
#
if(UNIX AND NOT APPLE)
  set(CPACK_GENERATOR "TGZ;DEB;RPM")
  
  # DEB specific settings
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "NutStash Team")
  set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtk-3-0, libgdk-pixbuf2.0-0")
  
  # RPM specific settings
  set(CPACK_RPM_PACKAGE_GROUP "Applications/Utilities")
  set(CPACK_RPM_PACKAGE_LICENSE "MIT")
  set(CPACK_RPM_PACKAGE_REQUIRES "gtk3")
endif()

# Set component architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CPACK_PACKAGE_ARCHITECTURE "x86_64")
else()
  set(CPACK_PACKAGE_ARCHITECTURE "x86")
endif()

# Enable component packaging
set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)
set(CPACK_COMPONENT_GROUP_APPLICATION_DISPLAY_NAME "NutStash Application")
set(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "NutStash")



