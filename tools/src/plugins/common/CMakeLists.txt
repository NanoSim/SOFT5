cmake_minimum_required(VERSION 3.2)
project (soft-tools-plugins-common C CXX)
set (CMAKE_THREAD_PREFER_PTHREAD TRUE)

find_package(Qt5Core REQUIRED)
find_package(Qt5Script REQUIRED)

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

  
    
