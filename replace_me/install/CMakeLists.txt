#
# Installation rules for packaging
#

# Check if required variables are defined
if(NOT DEFINED CEF_TARGET)
  message(FATAL_ERROR "CEF_TARGET is not defined. This file must be included after the nutstash target is created.")
endif()

if(NOT DEFINED CEF_TARGET_OUT_DIR)
  message(FATAL_ERROR "CEF_TARGET_OUT_DIR is not defined. This file must be included after the nutstash target is created.")
endif()

# Get the project root directory
# install/CMakeLists.txt is in install/ subdirectory, so go up one level to get project root
get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# Install target executable
if(OS_WINDOWS)
  # Windows: Install executable and all DLLs
  install(TARGETS ${CEF_TARGET}
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
    ARCHIVE DESTINATION .
    COMPONENT Application
  )
  
  # Install all files from output directory (CEF binaries, resources, etc.)
  # This includes DLLs, PAK files, locales, etc.
  install(DIRECTORY ${CEF_TARGET_OUT_DIR}/
    DESTINATION .
    COMPONENT Application
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING
    PATTERN "*.dll"
    PATTERN "*.exe"
    PATTERN "*.pak"
    PATTERN "*.dat"
    PATTERN "*.bin"
    PATTERN "*.json"
    PATTERN "*.manifest"
    PATTERN "locales/*"
  )
  
  # Install frontend build artifacts (dist directory)
  if(EXISTS "${PROJECT_ROOT}/dist")
    install(DIRECTORY "${PROJECT_ROOT}/dist/"
      DESTINATION dist
      COMPONENT Application
      USE_SOURCE_PERMISSIONS
    )
  endif()
  
elseif(OS_MAC)
  # macOS: Install app bundle
  install(TARGETS ${CEF_TARGET}
    BUNDLE DESTINATION .
    COMPONENT Application
  )
  
  # Install frontend build artifacts into app bundle Resources
  # Use install(CODE ...) to copy dist directory after bundle is installed
  # This works correctly with CPack DragNDrop generator
  if(EXISTS "${PROJECT_ROOT}/dist")
    # Store the dist path in a variable that will be available during install
    set(DIST_DIR "${PROJECT_ROOT}/dist")
    
    install(CODE "
      set(BUNDLE_PATH \"\${CMAKE_INSTALL_PREFIX}/${CEF_TARGET}.app\")
      set(RESOURCES_DIST \"\${BUNDLE_PATH}/Contents/Resources/dist\")
      set(DIST_SOURCE \"${DIST_DIR}\")
      
      # Check if dist source exists
      if(NOT EXISTS \"\${DIST_SOURCE}\")
        message(FATAL_ERROR \"Dist source directory not found: \${DIST_SOURCE}\")
      endif()
      
      # Check if bundle exists (should exist after install(TARGETS ...))
      if(EXISTS \"\${BUNDLE_PATH}\")
        # Create Resources/dist directory if it doesn't exist
        file(MAKE_DIRECTORY \"\${RESOURCES_DIST}\")
        
        # Copy all files from dist directory
        file(COPY \"\${DIST_SOURCE}/\"
             DESTINATION \"\${RESOURCES_DIST}\"
             USE_SOURCE_PERMISSIONS
             FILES_MATCHING
             PATTERN \"*\"
        )
      else()
        message(WARNING \"Bundle not found at: \${BUNDLE_PATH}\")
      endif()
    " COMPONENT Application)
  endif()
  
elseif(OS_LINUX)
  # Linux: Install executable
  install(TARGETS ${CEF_TARGET}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    COMPONENT Application
  )
  
  # Install all CEF files from output directory
  install(DIRECTORY ${CEF_TARGET_OUT_DIR}/
    DESTINATION bin
    COMPONENT Application
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING
    PATTERN "*.so"
    PATTERN "*.pak"
    PATTERN "*.dat"
    PATTERN "*.bin"
    PATTERN "*.json"
    PATTERN "locales/*"
  )
  
  # Install frontend build artifacts
  if(EXISTS "${PROJECT_ROOT}/dist")
    install(DIRECTORY "${PROJECT_ROOT}/dist/"
      DESTINATION share/nutstash/dist
      COMPONENT Application
      USE_SOURCE_PERMISSIONS
    )
  endif()
  
  # Install desktop file and icon (optional)
  # install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/nutstash.desktop"
  #   DESTINATION share/applications
  #   COMPONENT Application
  # )
endif()

