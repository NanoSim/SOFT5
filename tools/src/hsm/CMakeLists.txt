cmake_minimum_required(VERSION 3.2)
project (soft-tools-hsm C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(Qt5Core        REQUIRED)
find_package(Qt5Script      REQUIRED)
find_package(Qt5Concurrent  REQUIRED)

include_directories (
  "../plugins/common")

aux_source_directory (. SRC_LIST)

add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Script Qt5::Concurrent ${LINK_LIBS})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins)
