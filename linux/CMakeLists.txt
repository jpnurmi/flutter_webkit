cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "flutter_webkit")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "flutter_webkit_plugin")

add_library(${PLUGIN_NAME} SHARED
  "flutter_webkit_plugin.cc"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK)

pkg_check_modules(WEBKIT IMPORTED_TARGET REQUIRED webkit2gtk-4.0)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::WEBKIT)

# List of absolute paths to libraries that should be bundled with the plugin
set(flutter_webkit_bundled_libraries
  ""
  PARENT_SCOPE
)
