project (softpy C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)


find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${soft-kernel_SOURCE_DIR}
  ${softc_SOURCE_DIR}
  )

set(CMAKE_SWIG_FLAGS "")

set(SWIG_MODULE_softpy_EXTRA_DEPS
  numpy.i
  softpy-entity.i
  softpy-datamodel.i
  softpy-python.i
)

if(USE_PYTHON3)
  set_source_files_properties(softpy.i PROPERTIES SWIG_FLAGS "-py3 -Wall")
else()
  set_source_files_properties(softpy.i PROPERTIES SWIG_FLAGS "-Wall")
endif()

swig_add_module(softpy python softpy.i)
swig_link_libraries(softpy softc ${PYTHON_LIBRARIES})



#install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
#install(DIRECTORY . DESTINATION include/soft5/softc FILES_MATCHING PATTERN "*.h")

