project (softpy C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)


find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
#find_package(PythonInterp REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${soft-kernel_SOURCE_DIR}
  ${softc_SOURCE_DIR}
  )

set(CMAKE_SWIG_FLAGS "")

set_source_files_properties(softpy.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_module(softpy python softpy.i)
swig_link_libraries(softpy softc ${PYTHON_LIBRARIES})

set_source_files_properties(entity.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(entity.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_module(entity python entity.i)
swig_link_libraries(entity soft-kernel ${PYTHON_LIBRARIES})




#aux_source_directory(. SRC_LIST)
#add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
#target_link_libraries(${PROJECT_NAME} Qt5::Core soft-kernel )
#
## Expose public includes to other subprojects
#set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
#  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
#
#install (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
#install(DIRECTORY . DESTINATION include/soft5/softc FILES_MATCHING PATTERN "*.h")

