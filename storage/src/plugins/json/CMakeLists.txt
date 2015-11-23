cmake_minimum_required(VERSION 3.2)
project (soft-storage-plugins-json C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Qt5Core REQUIRED)

include_directories (${soft-kernel_INCLUDE_DIRS})

set (LINK_LIBS ${LINK_LIBS})

aux_source_directory (. SRC_LIST)
add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} Qt5::Core ${LINK_LIBS})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION bin/plugins)
