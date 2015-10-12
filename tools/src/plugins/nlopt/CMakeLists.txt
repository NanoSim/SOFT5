cmake_minimum_required(VERSION 3.2)
project (soft-tools-plugins-nlopt C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(Qt5Core    REQUIRED)
find_package(Qt5Script  REQUIRED)
find_package(NLOPT      REQUIRED)

include_directories (
  ${NLOPT_INCLUDE_DIR}
  ${soft-kernel_INCLUDE_DIRS}
  "../common")

set (LINK_LIBS ${LINK_LIBS} ${NLOPT_LIBRARIES})
aux_source_directory (. SRC_LIST)

add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Script ${LINK_LIBS})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION plugins)
