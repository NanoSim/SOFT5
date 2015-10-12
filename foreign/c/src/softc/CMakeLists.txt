cmake_minimum_required(VERSION 3.2)
project (softc C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)

aux_source_directory(. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Qt5::Core soft-kernel )

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

install (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY . DESTINATION include/soft5/softc FILES_MATCHING PATTERN "*.h")

