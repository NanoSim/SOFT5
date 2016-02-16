cmake_minimum_required(VERSION 3.2.1)
project (example-autogen)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SOFT5 REQUIRED)

set (C_SOURCE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.cjs)
set (C_HEADER_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ccode.hjs)

add_subdirectory(entities)
#add_subdirectory(src)
