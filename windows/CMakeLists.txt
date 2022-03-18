cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "hotplug_flutter")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "hotplug_flutter_plugin")

if(MSVC)
message(STATUS "is windows")
add_definitions("/WX-")
endif(MSVC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB hotplug_SRC ${CMAKE_CURRENT_SOURCE_DIR}/os/win/hiddev_detector.cc
                      ${CMAKE_CURRENT_SOURCE_DIR}/os/win/hiddev_detector.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/usb_tracer.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/usb_tracer.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/util.cpp)

add_library(${PLUGIN_NAME} SHARED
  "hotplug_flutter_plugin.cpp" ${hotplug_SRC}
)

target_include_directories(${PLUGIN_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/os/win)

apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)

# List of absolute paths to libraries that should be bundled with the plugin
set(hotplug_flutter_bundled_libraries
  ""
  PARENT_SCOPE
)
