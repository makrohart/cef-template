# Findlibgit2.cmake
# Finds the libgit2 library
#
# This module defines:
#  libgit2_FOUND          - True if libgit2 is found
#  libgit2_INCLUDE_DIRS   - Include directories for libgit2
#  libgit2_LIBRARIES      - Libraries to link against
#  libgit2_LIBRARY        - Path to the libgit2 library file
#  libgit2_DLL            - Path to the libgit2 DLL file (Windows)

# Set the base path for libgit2
# Use CMAKE_SOURCE_DIR to get the top-level source directory
# or CMAKE_CURRENT_LIST_DIR to get the directory containing this file
if(DEFINED CMAKE_SOURCE_DIR)
    set(LIBGIT2_ROOT_DIR "${CMAKE_SOURCE_DIR}/thirdParties/libgit2" CACHE PATH "Root directory of libgit2")
else()
    # Fallback: calculate from Find module location
    get_filename_component(LIBGIT2_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../thirdParties/libgit2" ABSOLUTE)
    set(LIBGIT2_ROOT_DIR "${LIBGIT2_ROOT_DIR}" CACHE PATH "Root directory of libgit2")
endif()

# Clear any previously cached values that might be wrong
unset(libgit2_INCLUDE_DIR CACHE)
unset(libgit2_LIBRARY CACHE)

# Determine the configuration suffix based on operating system and build type
# For multi-config generators (Visual Studio, Xcode), CMAKE_BUILD_TYPE is empty
# We'll handle both configurations separately
if(APPLE)
    # macOS: use arm-dbg for Debug, arm-rel for Release (if exists)
    # For Xcode (multi-config), we'll search both and let per-config logic handle it
    set(LIBGIT2_CONFIG_SUFFIX "arm-dbg")
    if(EXISTS "${LIBGIT2_ROOT_DIR}/arm-rel")
        set(LIBGIT2_CONFIG_SUFFIX_RELEASE "arm-rel")
    else()
        set(LIBGIT2_CONFIG_SUFFIX_RELEASE "arm-dbg")
    endif()
elseif(WIN32)
    # Windows: use x64-windows paths
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIBGIT2_CONFIG_SUFFIX "x64-windows-dbg")
    else()
        set(LIBGIT2_CONFIG_SUFFIX "x64-windows-rel")
    endif()
else()
    # Linux: use linux paths (if exists)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIBGIT2_CONFIG_SUFFIX "linux-dbg")
    else()
        set(LIBGIT2_CONFIG_SUFFIX "linux-rel")
    endif()
endif()

# Find include directory
# Platform-specific search - ensure we only search the correct platform paths
if(APPLE)
    # macOS: ONLY search macOS-specific paths, never Windows or Linux paths
    find_path(libgit2_INCLUDE_DIR
        NAMES git2.h
        PATHS
            "${LIBGIT2_ROOT_DIR}/arm-dbg/include"
            "${LIBGIT2_ROOT_DIR}/arm-rel/include"
        NO_DEFAULT_PATH
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
    )
elseif(WIN32)
    # Windows: ONLY search Windows-specific paths
    find_path(libgit2_INCLUDE_DIR
        NAMES git2.h
        PATHS
            "${LIBGIT2_ROOT_DIR}/x64-windows-rel/include"
            "${LIBGIT2_ROOT_DIR}/x64-windows-dbg/include"
        NO_DEFAULT_PATH
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
    )
else()
    # Linux: ONLY search Linux-specific paths
    find_path(libgit2_INCLUDE_DIR
        NAMES git2.h
        PATHS
            "${LIBGIT2_ROOT_DIR}/linux-rel/include"
            "${LIBGIT2_ROOT_DIR}/linux-dbg/include"
        NO_DEFAULT_PATH
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
    )
endif()

# Find library file
# macOS: library is in lib/ directory (standard install layout)
# Windows: library is in the config directory directly
if(APPLE)
    # macOS: search in lib/ directory (standard install layout)
    find_library(libgit2_LIBRARY
        NAMES libgit2 git2
        PATHS
            "${LIBGIT2_ROOT_DIR}/arm-dbg/lib"
            "${LIBGIT2_ROOT_DIR}/arm-rel/lib"
        NO_DEFAULT_PATH
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
    )
elseif(WIN32)
    # Windows: ONLY search Windows-specific paths
    find_library(libgit2_LIBRARY
        NAMES git2
        PATHS
            "${LIBGIT2_ROOT_DIR}/x64-windows-rel"
            "${LIBGIT2_ROOT_DIR}/x64-windows-dbg"
        NO_DEFAULT_PATH
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
    )
else()
    # Linux: ONLY search Linux-specific paths
    find_library(libgit2_LIBRARY
        NAMES git2
        PATHS
            "${LIBGIT2_ROOT_DIR}/linux-rel"
            "${LIBGIT2_ROOT_DIR}/linux-dbg"
        NO_DEFAULT_PATH
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
    )
endif()

# Find DLL file (Windows only)
if(WIN32)
    find_file(libgit2_DLL
        NAMES git2.dll
        PATHS
            "${LIBGIT2_ROOT_DIR}/${LIBGIT2_CONFIG_SUFFIX}"
        NO_DEFAULT_PATH
    )
endif()

# Handle per-configuration library paths for multi-config generators (Visual Studio, Xcode)
if(CMAKE_CONFIGURATION_TYPES)
    # Clear per-config caches
    unset(libgit2_LIBRARY_DEBUG CACHE)
    unset(libgit2_LIBRARY_RELEASE CACHE)
    
    if(APPLE)
        # Debug configuration for macOS - search in lib/ directory
        find_library(libgit2_LIBRARY_DEBUG
            NAMES libgit2 git2
            PATHS
                "${LIBGIT2_ROOT_DIR}/arm-dbg/lib"
            NO_DEFAULT_PATH
            NO_CMAKE_PATH
            NO_CMAKE_ENVIRONMENT_PATH
            NO_SYSTEM_ENVIRONMENT_PATH
            NO_CMAKE_SYSTEM_PATH
        )
        
        # Release configuration for macOS - search in lib/ directory
        find_library(libgit2_LIBRARY_RELEASE
            NAMES libgit2 git2
            PATHS
                "${LIBGIT2_ROOT_DIR}/arm-rel/lib"
                "${LIBGIT2_ROOT_DIR}/arm-dbg/lib"
            NO_DEFAULT_PATH
            NO_CMAKE_PATH
            NO_CMAKE_ENVIRONMENT_PATH
            NO_SYSTEM_ENVIRONMENT_PATH
            NO_CMAKE_SYSTEM_PATH
        )
    elseif(WIN32)
        # Debug configuration for Windows - ONLY search Windows paths
        find_library(libgit2_LIBRARY_DEBUG
            NAMES git2
            PATHS
                "${LIBGIT2_ROOT_DIR}/x64-windows-dbg"
            NO_DEFAULT_PATH
            NO_CMAKE_PATH
            NO_CMAKE_ENVIRONMENT_PATH
            NO_SYSTEM_ENVIRONMENT_PATH
            NO_CMAKE_SYSTEM_PATH
        )
        
        # Release configuration for Windows - ONLY search Windows paths
        find_library(libgit2_LIBRARY_RELEASE
            NAMES git2
            PATHS
                "${LIBGIT2_ROOT_DIR}/x64-windows-rel"
            NO_DEFAULT_PATH
            NO_CMAKE_PATH
            NO_CMAKE_ENVIRONMENT_PATH
            NO_SYSTEM_ENVIRONMENT_PATH
            NO_CMAKE_SYSTEM_PATH
        )
    endif()
    
    # Set the library based on configuration
    if(libgit2_LIBRARY_DEBUG AND libgit2_LIBRARY_RELEASE)
        set(libgit2_LIBRARY
            debug ${libgit2_LIBRARY_DEBUG}
            optimized ${libgit2_LIBRARY_RELEASE}
        )
    elseif(libgit2_LIBRARY_RELEASE)
        set(libgit2_LIBRARY ${libgit2_LIBRARY_RELEASE})
    elseif(libgit2_LIBRARY_DEBUG)
        set(libgit2_LIBRARY ${libgit2_LIBRARY_DEBUG})
    endif()
    
    # Handle DLL paths for multi-config
    if(WIN32)
        find_file(libgit2_DLL_DEBUG
            NAMES git2.dll
            PATHS
                "${LIBGIT2_ROOT_DIR}/x64-windows-dbg"
            NO_DEFAULT_PATH
        )
        
        find_file(libgit2_DLL_RELEASE
            NAMES git2.dll
            PATHS
                "${LIBGIT2_ROOT_DIR}/x64-windows-rel"
            NO_DEFAULT_PATH
        )
    endif()
endif()

# Set include directories
if(libgit2_INCLUDE_DIR)
    set(libgit2_INCLUDE_DIRS ${libgit2_INCLUDE_DIR})
else()
    set(libgit2_INCLUDE_DIRS)
endif()

# Set libraries
if(libgit2_LIBRARY)
    set(libgit2_LIBRARIES ${libgit2_LIBRARY})
else()
    set(libgit2_LIBRARIES)
endif()

# Mark as found if both include and library are found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libgit2
    FOUND_VAR libgit2_FOUND
    REQUIRED_VARS
        libgit2_INCLUDE_DIR
        libgit2_LIBRARY
    FAIL_MESSAGE "libgit2 not found. Please ensure libgit2 is installed in ${LIBGIT2_ROOT_DIR}"
)

# Mark advanced variables
mark_as_advanced(
    libgit2_INCLUDE_DIR
    libgit2_LIBRARY
    libgit2_LIBRARY_DEBUG
    libgit2_LIBRARY_RELEASE
    libgit2_DLL
    libgit2_DLL_DEBUG
    libgit2_DLL_RELEASE
)
