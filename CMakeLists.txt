cmake_minimum_required(VERSION 3.2)
include(CPack)
project(soft5)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package (BSON REQUIRED)
find_package (Qt5Core REQUIRED)
find_package (Qt5Network REQUIRED) 
find_package (Qt5Script REQUIRED) 
find_package (Qt5Concurrent REQUIRED)

set (SOFT_MAJOR_VERSION 5)
set (SOFT_MINOR_VERSION 1)
set (SOFT_MICRO_VERSION 3)
set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

# Install targets
set(SOFTBASE lib/${PROJECT_NAME})
set(STORAGE_PLUGINS_INSTALL_DIR ${SOFTBASE}/storage)
set(SCRIPT_MODULES_INSTALL_DIR ${SOFTBASE}/modules)
set(SCRIPT_PLUGINS_INSTALL_DIR ${SOFTBASE}/plugins)
set(INCLUDE_INSTALL_DIR include/${PROJECT_NAME})

option (WITH_STORAGE_HD5 "Enable HDF5 storage support"               ON)
option (WITH_STORAGE_MONGO "Enable MongoDB storage support"          ON)
option (WITH_STORAGE_JSON "Enable JSON storage support"              ON)
option (WITH_FOREIGN_INTERFACES "Enable foreign language interfaces" OFF)
option (WITH_TOOLS "Build tools and utilities" OFF)


option (WITH_TOOLS "Enable tools" OFF)
if (WITH_TOOLS)
  add_subdirectory(tools)
endif (WITH_TOOLS)

option (WITH_FOREIGN_INTERFACES)
  add_subdirectory(foreign)
endif (WITH_FOREIGN_INTERFACES)
