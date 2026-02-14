# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#

# replace_me browser sources.
set(REPLACE_ME_BROWSER_BROWSER_SRCS
  browser/browser_window.cc
  browser/browser_window.h
  browser/bytes_write_handler.cc
  browser/bytes_write_handler.h
  browser/client_app_browser.cc
  browser/client_app_browser.h
  browser/client_app_browser_delegate.cc
  browser/client_app_browser_delegate.h
  browser/client_handler.cc
  browser/client_handler.h
  browser/client_handler_base.cc
  browser/client_handler_base.h
  browser/client_handler_osr.cc
  browser/client_handler_osr.h
  browser/client_handler_std.cc
  browser/client_handler_std.h
  browser/client_prefs.cc
  browser/client_prefs.h
  browser/client_types.h
  browser/default_client_handler.cc
  browser/default_client_handler.h
  browser/image_cache.cc
  browser/image_cache.h
  browser/main_context.cc
  browser/main_context.h
  browser/main_context_impl.cc
  browser/main_context_impl.h
  browser/osr_dragdrop_events.h
  browser/osr_renderer.cc
  browser/osr_renderer.h
  browser/osr_renderer_settings.h
  browser/resource.h
  browser/root_window.cc
  browser/root_window.h
  browser/root_window_create.cc
  browser/root_window_manager.cc
  browser/root_window_manager.h
  browser/root_window_views.cc
  browser/root_window_views.h
  browser/temp_window.h
  browser/views_menu_bar.cc
  browser/views_menu_bar.h
  browser/views_overlay_browser.cc
  browser/views_overlay_browser.h
  browser/views_overlay_controls.cc
  browser/views_overlay_controls.h
  browser/views_style.cc
  browser/views_style.h
  browser/views_window.cc
  browser/views_window.h
  browser/message_handler.cc
  browser/message_handler.h
  browser/main_message_loop.cc
  browser/main_message_loop.h
  browser/main_message_loop_external_pump.cc
  browser/main_message_loop_external_pump.h
  browser/main_message_loop_std.cc
  browser/main_message_loop_std.h
  browser/event_router_browser_side.h
  browser/event_router_browser_side.cc
  )
source_group(replace_me\\\\browser FILES ${REPLACE_ME_BROWSER_BROWSER_SRCS})

set(REPLACE_ME_SERVICES_BASE_SRCS
  services/iservice.h
  services/test_service.h
  )
source_group(replace_me\\\\services FILES ${REPLACE_ME_SERVICES_BASE_SRCS})

set(REPLACE_ME_SERVICES_SRCS
  ${REPLACE_ME_SERVICES_BASE_SRCS}
  )

set(REPLACE_ME_BROWSER_SRCS
  ${REPLACE_ME_BROWSER_BROWSER_SRCS}
  )

# replace_me common sources.
set(REPLACE_ME_COMMON_SRCS
  common/binary_value_utils.cc
  common/binary_value_utils.h
  common/client_app.cc
  common/client_app.h
  common/client_app_other.cc
  common/client_app_other.h
  common/client_switches.cc
  common/client_switches.h
  common/string_util.cc
  common/string_util.h
  common/file_util.cc
  common/file_util.h 
  common/geometry_util.cc
  common/geometry_util.h
  common/resource_util.h
  common/progress_message_router_utils.h
  common/progress_message_router_utils.cpp
  common/event.h
  common/event.cpp
  common/notify.h
  common/event_notify.h
  )
source_group(replace_me\\\\common FILES ${REPLACE_ME_COMMON_SRCS})

# replace_me renderer sources.
set(REPLACE_ME_RENDERER_SRCS
  renderer/client_app_renderer.cc
  renderer/client_app_renderer.h
  renderer/client_app_renderer_delegate.cc
  renderer/client_app_renderer_delegate.h
  renderer/event_router_render_side.h
  renderer/event_router_render_side.cc
  )
source_group(replace_me\\\\renderer FILES ${REPLACE_ME_RENDERER_SRCS})

#replace_me Linux sources
set(REPLACE_ME_LINUX_SRCS
  cefclient_gtk.cc
  )
source_group(replace_me FILES ${REPLACE_ME_LINUX_SRCS})

set(REPLACE_ME_LINUX_BROWSER_SRCS
  browser/browser_window_osr_gtk.cc
  browser/browser_window_osr_gtk.h
  browser/browser_window_std_gtk.cc
  browser/browser_window_std_gtk.h
  browser/dialog_handler_gtk.cc
  browser/dialog_handler_gtk.h
  browser/main_context_impl_posix.cc
  browser/main_message_loop_multithreaded_gtk.cc
  browser/main_message_loop_multithreaded_gtk.h
  browser/print_handler_gtk.cc
  browser/print_handler_gtk.h
  browser/resource_util_linux.cc
  browser/root_window_gtk.cc
  browser/root_window_gtk.h
  browser/temp_window_x11.cc
  browser/temp_window_x11.h
  browser/util_gtk.cc
  browser/util_gtk.h
  # TODO: this file does not exist, add it!
  browser/main_message_loop_external_pump_linux.cc
  )
source_group(replace_me\\\\browser FILES ${REPLACE_ME_LINUX_BROWSER_SRCS})

set(REPLACE_ME_LINUX_COMMON_SRCS
  common/resource_util_posix.cc
  )
source_group(replace_me\\\\common FILES ${REPLACE_ME_LINUX_COMMON_SRCS})

set(REPLACE_ME_LINUX_SRCS
  ${REPLACE_ME_LINUX_SRCS}
  ${REPLACE_ME_LINUX_BROWSER_SRCS}
  ${REPLACE_ME_LINUX_COMMON_SRCS}
  )

#replace_me Mac OS X sources
set(REPLACE_ME_MACOSX_SRCS
  cefclient_mac.mm
  )
source_group(replace_me FILES ${REPLACE_ME_MACOSX_SRCS})

set(REPLACE_ME_MACOSX_BROWSER_SRCS
  browser/browser_window_osr_mac.h
  browser/browser_window_osr_mac.mm
  browser/browser_window_std_mac.h
  browser/browser_window_std_mac.mm
  browser/main_context_impl_posix.cc
  browser/osr_accessibility_helper.cc
  browser/osr_accessibility_helper.h
  browser/osr_accessibility_node.cc
  browser/osr_accessibility_node.h
  browser/osr_accessibility_node_mac.mm
  browser/root_window_mac.h
  browser/root_window_mac.mm
  browser/temp_window_mac.h
  browser/temp_window_mac.mm
  browser/text_input_client_osr_mac.h
  browser/text_input_client_osr_mac.mm
  browser/util_mac.h
  browser/util_mac.mm
  browser/views_window_mac.mm
  browser/main_message_loop_external_pump_mac.mm
  )
source_group(replace_me\\\\browser FILES ${REPLACE_ME_MACOSX_BROWSER_SRCS})

set(REPLACE_ME_MACOSX_COMMON_SRCS
  common/resource_util_mac.mm
  common/resource_util_posix.cc
  )
source_group(replace_me\\\\common FILES ${REPLACE_ME_MACOSX_COMMON_SRCS})

set(REPLACE_ME_MAC_SRCS
  ${REPLACE_ME_MACOSX_SRCS}
  ${REPLACE_ME_MACOSX_BROWSER_SRCS}
  ${REPLACE_ME_MACOSX_COMMON_SRCS}
  )

# replace_me Mac OS X helper sources.
set(REPLACE_ME_HELPER_SHARED_SRCS
  ../shared/process_helper_mac.cc
  )
source_group(shared FILES ${REPLACE_ME_HELPER_SHARED_SRCS})

set(REPLACE_ME_MAC_HELPER_SRCS
  ${REPLACE_ME_HELPER_SHARED_SRCS}
  )

#replace_me Windows sources
set(REPLACE_ME_WINDOWS_SRCS
  cefclient_win.cc
  )
source_group(replace_me FILES ${REPLACE_ME_WINDOWS_SRCS})

set(REPLACE_ME_WINDOWS_BROWSER_SRCS
  browser/browser_window_osr_win.cc
  browser/browser_window_osr_win.h
  browser/browser_window_std_win.cc
  browser/browser_window_std_win.h
  browser/main_context_impl_win.cc
  browser/main_message_loop_multithreaded_win.cc
  browser/main_message_loop_multithreaded_win.h
  browser/osr_accessibility_helper.cc
  browser/osr_accessibility_helper.h
  browser/osr_accessibility_node.cc
  browser/osr_accessibility_node.h
  browser/osr_accessibility_node_win.cc
  browser/osr_d3d11_win.cc
  browser/osr_d3d11_win.h
  browser/osr_dragdrop_win.cc
  browser/osr_dragdrop_win.h
  browser/osr_ime_handler_win.cc
  browser/osr_ime_handler_win.h
  browser/osr_render_handler_win.cc
  browser/osr_render_handler_win.h
  browser/osr_render_handler_win_d3d11.cc
  browser/osr_render_handler_win_d3d11.h
  browser/osr_render_handler_win_gl.cc
  browser/osr_render_handler_win_gl.h
  browser/osr_window_win.cc
  browser/osr_window_win.h
  browser/resource_util_win_idmap.cc
  browser/root_window_win.cc
  browser/root_window_win.h
  browser/temp_window_win.cc
  browser/temp_window_win.h
  browser/main_message_loop_external_pump_win.cc
  )
source_group(replace_me\\\\browser FILES ${REPLACE_ME_WINDOWS_BROWSER_SRCS})

set(REPLACE_ME_WINDOWS_COMMON_SRCS
  common/resource_util_win.cc
  common/util_win.cc
  common/util_win.h
  )
source_group(replace_me\\\\common FILES ${REPLACE_ME_WINDOWS_COMMON_SRCS})

set(REPLACE_ME_WINDOWS_WIN_SRCS
  win/cefclient.rc
  )
source_group(replace_me\\\\win FILES ${REPLACE_ME_WINDOWS_WIN_SRCS})

set(REPLACE_ME_WINDOWS_SRCS
  ${REPLACE_ME_WINDOWS_SRCS}
  ${REPLACE_ME_WINDOWS_BROWSER_SRCS}
  ${REPLACE_ME_WINDOWS_WIN_SRCS}
  ${REPLACE_ME_WINDOWS_COMMON_SRCS}
  )

# replace_me resources.
set(REPLACE_ME_RESOURCES_MAC_SRCS_MAC
  mac/Info.plist.in
  mac/cefclient.icns
  )
APPEND_PLATFORM_SOURCES(REPLACE_ME_RESOURCES_MAC_SRCS)
source_group(replace_me\\\\mac FILES ${REPLACE_ME_RESOURCES_MAC_SRCS})

set(REPLACE_ME_RESOURCES_MAC_ENGLISH_LPROJ_SRCS_MAC
  mac/English.lproj/InfoPlist.strings
  mac/English.lproj/MainMenu.xib
  )
APPEND_PLATFORM_SOURCES(REPLACE_ME_RESOURCES_MAC_ENGLISH_LPROJ_SRCS)
source_group(replace_me\\\\mac\\\\English.lproj FILES ${REPLACE_ME_RESOURCES_MAC_ENGLISH_LPROJ_SRCS})

set(REPLACE_ME_RESOURCES_RESOURCES_SRCS
  resources/draggable.html
  resources/logo.png
  resources/menu_icon.1x.png
  resources/menu_icon.2x.png
  )
source_group(replace_me\\\\resources FILES ${REPLACE_ME_RESOURCES_RESOURCES_SRCS})

set(REPLACE_ME_RESOURCES_SHARED_RESOURCES_SRCS
  ../shared/resources/pdf.html
  ../shared/resources/pdf.pdf
  ../shared/resources/window_icon.1x.png
  ../shared/resources/window_icon.2x.png
  )
source_group(shared\\\\resources FILES ${REPLACE_ME_RESOURCES_SHARED_RESOURCES_SRCS})

set(REPLACE_ME_RESOURCES_SRCS
  ${REPLACE_ME_RESOURCES_MAC_SRCS}
  ${REPLACE_ME_RESOURCES_MAC_ENGLISH_LPROJ_SRCS}
  ${REPLACE_ME_RESOURCES_RESOURCES_SRCS}
  ${REPLACE_ME_RESOURCES_SHARED_RESOURCES_SRCS}
  )


#
# Shared configuration.
#

# Target executable names.
set(CEF_TARGET "replace_me")
# Set to parent scope so install/CMakeLists.txt can access it
set(CEF_TARGET "replace_me" PARENT_SCOPE)
if(OS_MAC)
  set(CEF_HELPER_TARGET "REPLACE_ME_Helper")
  set(CEF_HELPER_OUTPUT_NAME "replace_me Helper")
else()
  # Logical target used to link the libcef library.
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

# Determine the target output directory.
SET_CEF_TARGET_OUT_DIR()
# Set to parent scope so install/CMakeLists.txt can access it
set(CEF_TARGET_OUT_DIR "${CEF_TARGET_OUT_DIR}" PARENT_SCOPE)

# Frontend dist root (repo root, two levels up from replace_me/replace_me/app).
get_filename_component(FRONTEND_DIST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../app" ABSOLUTE)
# Copy frontend dist to destination after build; call from each platform with appropriate DEST_DIR.
macro(COPY_FRONTEND_DIST TARGET DEST_DIR)
  if(EXISTS "${FRONTEND_DIST_ROOT}/dist")
    add_custom_command(
      TARGET ${TARGET}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${FRONTEND_DIST_ROOT}/dist"
              "${DEST_DIR}"
      COMMENT "Copying frontend dist"
      VERBATIM
      )
  endif()
endmacro()


#
# Linux configuration.
#

if(OS_LINUX)
  # All sources required by the "replace_me" target. Generates an executable that
  # is used for all processes.
  set(REPLACE_ME_SRCS
    ${REPLACE_ME_BROWSER_SRCS}
    ${REPLACE_ME_COMMON_SRCS}
    ${REPLACE_ME_RENDERER_SRCS}
    ${REPLACE_ME_RESOURCES_SRCS}
    ${REPLACE_ME_LINUX_SRCS}
    ${REPLACE_ME_SERVICES_SRCS}
    )

  # Find required libraries and update compiler/linker variables.
  FIND_LINUX_LIBRARIES("gmodule-2.0 gtk+-3.0 gthread-2.0 gtk+-unix-print-3.0 xi")

  # Executable target.
  add_executable(${CEF_TARGET} ${REPLACE_ME_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  # Set C++20 standard
  set_target_properties(${CEF_TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
  )
  # Enable C++ exceptions (override CEF default)
  target_compile_options(${CEF_TARGET} PRIVATE
    -fexceptions
  )
  # Enable RTTI (Runtime Type Information)
  target_compile_options(${CEF_TARGET} PRIVATE
    -frtti
  )
  # Add project root directory to include path so we can use local shared files
  target_include_directories(${CEF_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}")
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  
  # Disable treat warnings as errors
  target_compile_options(${CEF_TARGET} PRIVATE -Wno-error)
  
  target_link_libraries(${CEF_TARGET} PRIVATE libcef_lib libcef_dll_wrapper "GL" ${CEF_STANDARD_LIBS})

  # Set rpath so that libraries can be placed next to the executable.
  set_target_properties(${CEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
  set_target_properties(${CEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
  set_target_properties(${CEF_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})

  # We don't call deprecated GTK functions, and they can cause build failures, so disable them.
  add_definitions("-DGTK_DISABLE_DEPRECATED")

  # Copy CEF binary and resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

  # Copy replace_me resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${REPLACE_ME_RESOURCES_SRCS}" "${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET_OUT_DIR}/REPLACE_ME_files")

  # Set SUID permissions on the chrome-sandbox target.
  SET_LINUX_SUID_PERMISSIONS("${CEF_TARGET}" "${CEF_TARGET_OUT_DIR}/chrome-sandbox")

  COPY_FRONTEND_DIST(${CEF_TARGET} "${CEF_TARGET_OUT_DIR}/dist")
endif()


#
# Mac OS X configuration.
#

if(OS_MAC)
  option(OPTION_USE_ARC "Build with ARC (automatic Reference Counting) on macOS." ON)
  if(OPTION_USE_ARC)
    list(APPEND CEF_COMPILER_FLAGS
      -fobjc-arc
      )
    set_target_properties(${target} PROPERTIES
      CLANG_ENABLE_OBJC_ARC "YES"
      )
  endif()

  # All sources required by the "replace_me" target. Generates an app bundle that
  # is used only for the browser process.
  set(REPLACE_ME_SRCS
    ${REPLACE_ME_BROWSER_SRCS}
    ${REPLACE_ME_COMMON_SRCS}
    ${REPLACE_ME_RESOURCES_SRCS}
    ${REPLACE_ME_MAC_SRCS}
    ${REPLACE_ME_SERVICES_SRCS}
    )

  # All sources required by the "replace_me Helper" target. Generates an app
  # bundle that is used only for non-browser processes.
  set(REPLACE_ME_HELPER_SRCS
    ${REPLACE_ME_COMMON_SRCS}
    ${REPLACE_ME_RENDERER_SRCS}
    ${REPLACE_ME_MAC_HELPER_SRCS}
    )

  # Output path for the main app bundle.
  set(CEF_APP "${CEF_TARGET_OUT_DIR}/${CEF_TARGET}.app")

  # Variables referenced from the main Info.plist file.
  set(EXECUTABLE_NAME "${CEF_TARGET}")
  set(PRODUCT_NAME "${CEF_TARGET}")

  # Main app bundle target.
  add_executable(${CEF_TARGET} MACOSX_BUNDLE ${REPLACE_ME_RESOURCES_SRCS} ${REPLACE_ME_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  # Set C++20 standard (override CEF default which sets gnu++11)
  set_target_properties(${CEF_TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++20"
  )
  # Enable C++ exceptions (override CEF default)
  set_target_properties(${CEF_TARGET} PROPERTIES
    XCODE_ATTRIBUTE_GCC_ENABLE_CPP_EXCEPTIONS YES
  )
  # Remove -fno-exceptions and enable exceptions
  target_compile_options(${CEF_TARGET} PRIVATE
    -fexceptions
  )
  # Enable RTTI (Runtime Type Information)
  target_compile_options(${CEF_TARGET} PRIVATE
    -frtti
  )
  # Add project root directory to include path so we can use local shared files
  target_include_directories(${CEF_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}")
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  
  # Disable treat warnings as errors
  target_compile_options(${CEF_TARGET} PRIVATE -Wno-error)
  
  target_link_libraries(${CEF_TARGET} PRIVATE libcef_dll_wrapper ${CEF_STANDARD_LIBS} "-framework OpenGL")
  set_target_properties(${CEF_TARGET} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/mac/Info.plist.in
    )

  # Copy the CEF framework into the Frameworks directory and create the necessary symlinks.
  COPY_MAC_FRAMEWORK("${CEF_TARGET}" "${CEF_BINARY_DIR}" "${CEF_APP}")

  # Create the multiple Helper app bundle targets.
  foreach(_suffix_list ${CEF_HELPER_APP_SUFFIXES})
    # Convert to a list and extract the suffix values.
    string(REPLACE ":" ";" _suffix_list ${_suffix_list})
    list(GET _suffix_list 0 _name_suffix)
    list(GET _suffix_list 1 _target_suffix)
    list(GET _suffix_list 2 _plist_suffix)

    # Define Helper target and output names.
    set(_helper_target "${CEF_HELPER_TARGET}${_target_suffix}")
    set(_helper_output_name "${CEF_HELPER_OUTPUT_NAME}${_name_suffix}")

    # Create Helper-specific variants of the helper-Info.plist file. Do this
    # manually because the configure_file command (which is executed as part of
    # MACOSX_BUNDLE_INFO_PLIST) uses global env variables and would insert the
    # wrong values with multiple targets.
    set(_helper_info_plist "${CMAKE_CURRENT_BINARY_DIR}/helper-Info${_target_suffix}.plist")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/mac/helper-Info.plist.in" _plist_contents)
    string(REPLACE "\${EXECUTABLE_NAME}" "${_helper_output_name}" _plist_contents "${_plist_contents}")
    string(REPLACE "\${PRODUCT_NAME}" "${_helper_output_name}" _plist_contents "${_plist_contents}")
    string(REPLACE "\${BUNDLE_ID_SUFFIX}" "${_plist_suffix}" _plist_contents "${_plist_contents}")
    file(WRITE ${_helper_info_plist} ${_plist_contents})

    # Create Helper executable target.
    add_executable(${_helper_target} MACOSX_BUNDLE ${REPLACE_ME_HELPER_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(${_helper_target})
    # Enable C++ exceptions (override CEF default)
    set_target_properties(${_helper_target} PROPERTIES
      XCODE_ATTRIBUTE_GCC_ENABLE_CPP_EXCEPTIONS YES
    )
    target_compile_options(${_helper_target} PRIVATE
      -fexceptions
    )
    # Enable RTTI (Runtime Type Information)
    target_compile_options(${_helper_target} PRIVATE
      -frtti
    )
    # Add project root directory to include path so we can use local shared files
    target_include_directories(${_helper_target} PRIVATE "${CMAKE_SOURCE_DIR}")
    add_dependencies(${_helper_target} libcef_dll_wrapper)
    target_link_libraries(${_helper_target} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
    set_target_properties(${_helper_target} PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST ${_helper_info_plist}
      OUTPUT_NAME ${_helper_output_name}
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++20"
      )

    # Add the Helper as a dependency of the main executable target.
    add_dependencies(${CEF_TARGET} "${_helper_target}")

    # Copy the Helper app bundle into the Frameworks directory.
    add_custom_command(
      TARGET ${CEF_TARGET}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${CEF_TARGET_OUT_DIR}/${_helper_output_name}.app"
              "${CEF_APP}/Contents/Frameworks/${_helper_output_name}.app"
      VERBATIM
      )
  endforeach()

  # Manually process and copy over resource files.
  # The Xcode generator can support this via the set_target_properties RESOURCE
  # directive but that doesn't properly handle nested resource directories.
  # Remove these prefixes from input file paths.
  set(PREFIXES
    "mac/"
    "resources/"
    "../shared/resources/"
    )
  COPY_MAC_RESOURCES("${REPLACE_ME_RESOURCES_SRCS}" "${PREFIXES}" "${CEF_TARGET}" "${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_APP}")

  COPY_FRONTEND_DIST(${CEF_TARGET} "${CEF_APP}/Contents/Resources/dist")
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
  # All sources required by the "replace_me" target. Generates an executable that
  # is used for all processes.
  set(REPLACE_ME_SRCS
    ${REPLACE_ME_BROWSER_SRCS}
    ${REPLACE_ME_COMMON_SRCS}
    ${REPLACE_ME_RENDERER_SRCS}
    ${REPLACE_ME_RESOURCES_SRCS}
    ${REPLACE_ME_WINDOWS_SRCS}
    ${REPLACE_ME_SERVICES_SRCS}
    )

  # Additional flags not listed in CEF_DELAYLOAD_FLAGS.
  set(ADDITIONAL_DELAYLOAD_FLAGS
    /DELAYLOAD:glu32.dll
    /DELAYLOAD:oleaut32.dll
    /DELAYLOAD:opengl32.dll
    )

  if(USE_SANDBOX)
    # Shared library (DLL) target.
    list(APPEND CEF_SHARED_LINKER_FLAGS
      ${ADDITIONAL_DELAYLOAD_FLAGS}
    )
    add_library(${CEF_TARGET} SHARED ${REPLACE_ME_SRCS})
    SET_LIBRARY_TARGET_PROPERTIES(${CEF_TARGET})
    # Set C++20 standard
    set_target_properties(${CEF_TARGET} PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
    )
    # Enable RTTI (Runtime Type Information) for MSVC
    if(MSVC)
      target_compile_options(${CEF_TARGET} PRIVATE /GR)
    endif()
    # Add project root directory to include path so we can use local shared files
    target_include_directories(${CEF_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}")

    # Copy and rename the bootstrap binary.
    COPY_SINGLE_FILE(${CEF_TARGET} ${CEF_BINARY_DIR}/bootstrap.exe ${CEF_TARGET_OUT_DIR}/${CEF_TARGET}.exe)

    if (CMAKE_GENERATOR MATCHES "Visual Studio")
      # Make the bootstrap binary the default command for the Visual Studio debugger.
      set_target_properties(${CEF_TARGET} PROPERTIES
        VS_DEBUGGER_COMMAND "$<TARGET_FILE_DIR:${CEF_TARGET}>/${CEF_TARGET}.exe"
      )
    endif()
  else()
    # Executable target.
    list(APPEND CEF_EXE_LINKER_FLAGS
      ${ADDITIONAL_DELAYLOAD_FLAGS}
    )
    add_executable(${CEF_TARGET} WIN32 ${REPLACE_ME_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
    # Set C++20 standard
    set_target_properties(${CEF_TARGET} PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
    )
    # Enable C++ exceptions (override CEF default)
    if(MSVC)
      target_compile_options(${CEF_TARGET} PRIVATE /EHsc)
    else()
      target_compile_options(${CEF_TARGET} PRIVATE -fexceptions)
    endif()
    # Enable RTTI (Runtime Type Information)
    if(MSVC)
      target_compile_options(${CEF_TARGET} PRIVATE /GR)
    else()
      target_compile_options(${CEF_TARGET} PRIVATE -frtti)
    endif()
    # Add project root directory to include path so we can use local shared files
    target_include_directories(${CEF_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}")

    # Add the custom manifest files to the executable.
    ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}/win" "${CEF_TARGET}" "exe")
  endif()

  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  
  # Disable treat warnings as errors
  if(MSVC)
    target_compile_options(${CEF_TARGET} PRIVATE /WX- /std:c++20)
    # Enable debug symbols in Release mode for debugging
    target_compile_options(${CEF_TARGET} PRIVATE 
      $<$<CONFIG:Release>:/Zi>  # Generate debug information
    )
    # Enable PDB generation in Release mode
    target_link_options(${CEF_TARGET} PRIVATE
      $<$<CONFIG:Release>:/DEBUG>  # Generate PDB file
    )
  else()
    target_compile_options(${CEF_TARGET} PRIVATE -Wno-error)
  endif()
  
  target_link_libraries(${CEF_TARGET} PRIVATE libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS} d3d11.lib glu32.lib imm32.lib opengl32.lib)

  if(USE_ATL)
    # Required by VS2013 to link accessibility API functions.
    target_link_libraries(${CEF_TARGET} PRIVATE oleacc.lib)
  endif()

  # Copy CEF binary and resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

  if(USE_SANDBOX)
    # Set LPAC ACLs required for Windows sandbox support.
    SET_LPAC_ACLS("${CEF_TARGET}")
  endif()

  COPY_FRONTEND_DIST(${CEF_TARGET} "${CEF_TARGET_OUT_DIR}/dist")
endif()

# Example for find package. Add more find_package here

# find_package(libgit2 REQUIRED)
# target_link_libraries(${CEF_TARGET} PRIVATE ${libgit2_LIBRARIES})
# target_include_directories(${CEF_TARGET} PRIVATE ${libgit2_INCLUDE_DIRS})

# xpack - header only
target_include_directories(${CEF_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../thirdParties/xpack")