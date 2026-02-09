# For VS2022 and Xcode 12+ support.
cmake_minimum_required(VERSION 3.21)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Project name.
# TODO: Change this line to match your project name when you copy this file.
project(replace_me)

# Set C++20 standard globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#
# CEF_ROOT setup.
# This variable must be set to locate the binary distribution.
# TODO: Choose one of the below examples and comment out the rest.
#

# Example 1: The current directory contains both the complete binary
#            distribution and your project.
# A. Comment in these lines:
#
# set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")

# Example 2: The binary distribution is in a separate directory from your
#            project. Locate the binary distribution using the CEF_ROOT CMake
#            variable.
# A. Create a directory structure for your project like the following:
#    myproject/
#      CMakeLists.txt    <= top-level CMake configuration
#      mytarget/
#        CMakeLists.txt  <= CMake configuration for `mytarget`
#        ... other `mytarget` source files
# B. Copy this file to "myproject/CMakeLists.txt" as the top-level CMake
#    configuration.
# C. Create the target-specific "myproject/mytarget/CMakeLists.txt" file for
#    your application. See the included cefclient and cefsimple CMakeLists.txt
#    files as an example.
# D. Comment in these lines:
#
# Set CEF_ROOT based on the operating system
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdParties/cef_binary_142.0.10+g29548e2+chromium-142.0.7444.135_windows64")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # macOS - adjust the path according to your actual CEF binary directory name
  # Check if we're on Apple Silicon (arm64) or Intel (x86_64)
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" OR CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
    # For Apple Silicon Macs, use arm64 CEF binary if available
    # If you have arm64 CEF, uncomment and adjust the path below:
    # set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdParties/cef_binary_142.0.10+g29548e2+chromium-142.0.7444.135_macosarm64")
    # For now, using x86_64 with Rosetta 2 compatibility
    set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdParties/cef_binary_142.0.10+g29548e2+chromium-142.0.7444.135_macosarm64")
    # Force x86_64 architecture to match CEF binary
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architectures for macOS" FORCE)
  else()
    # For Intel Macs
    set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdParties/cef_binary_142.0.10+g29548e2+chromium-142.0.7444.135_macosx64")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # Linux - adjust the path according to your actual CEF binary directory name
  set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdParties/cef_binary_142.0.10+g29548e2+chromium-142.0.7444.135_linux64")
else()
  message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CEF_ROOT}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

# Example 3: The binary distribution is in a separate directory from your
#            project. Locate the binary distribution using the CEF_ROOT
#            environment variable.
# A. Create a directory structure for your project like the following:
#    myproject/
#      CMakeLists.txt    <= top-level CMake configuration
#      cmake/
#        FindCEF.cmake   <= CEF CMake configuration entry point
#      mytarget/
#        CMakeLists.txt  <= CMake configuration for `mytarget`
#        ... other `mytarget` source files
# B. Copy this file to "myproject/CMakeLists.txt" as the top-level CMake
#    configuration.
# C. Copy the cmake/FindCEF.cmake file to "myproject/cmake/FindCEF.cmake".
# D. Create the target-specific "myproject/mytarget/CMakeLists.txt" file for
#    your application. See the included cefclient and cefsimple CMakeLists.txt
#    files as an example.
# E. Set the CEF_ROOT environment variable before executing CMake. For example:
#    > set CEF_ROOT=c:\path\to\cef_binary_3.2704.xxxx.gyyyyyyy_windows32
# F. Comment in these lines:
#
# set(CMAKE_MODULE_PATH ${  } "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


#
# Load the CEF configuration.
#

# Execute FindCEF.cmake which must exist in CMAKE_MODULE_PATH.
find_package(CEF REQUIRED)


#
# Define CEF-based targets.
#

# Include the libcef_dll_wrapper target.
# Comes from the libcef_dll/CMakeLists.txt file in the binary distribution
# directory.
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# Include application targets.
# Comes from the <target>/CMakeLists.txt file in the current directory.
# TODO: Change these lines to match your project target when you copy this file.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/replace_me")
  add_subdirectory(replace_me)
endif()

# Display configuration settings.
PRINT_CEF_CONFIG()

#
# Include installation and packaging configuration
# Note: This must be included after the replace_me target is defined
#
get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
include("${PROJECT_ROOT}/install/CPackConfig.cmake")
add_subdirectory("${PROJECT_ROOT}/install" install)
