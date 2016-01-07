project(forign-test-softpy C CXX)

find_package(PythonLibs)
message("PYTHONLIBS_VERSION_STRING=${PYTHONLIBS_VERSION_STRING}")
set(Python_ADDITIONAL_VERSIONS "${PYTHONLIBS_VERSION_STRING}")
find_package(PythonInterp)


if(PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  add_test(${PROJECT_NAME}
    ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test_softpy.py)
    #"${soft-storage-plugins-mongo_BINARY_DIR}:${soft-storage-plugins-json_BINARY_DIR}")

  
  if (UNIX)
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "LD_LIBRARY_PATH=${soft-storage-plugins-hdf5_BINARY_DIR}:${soft-kernel_BINARY_DIR}:${soft-storage-hdf5_BINARY_DIR}:${soft-storage-mongo_BINARY_DIR}:${soft-storage-plugins-json_BINARY_DIR}:${soft-storage-plugins-mongo_BINARY_DIR}:${softc_BINARY_DIR}:$ENV{LD_LIBRARY_PATH}")
  else()
    set_property(TEST ${PROJECT_NAME} PROPERTY
      ENVIRONMENT "PATH=${soft-storage-plugins-json_BINARY_DIR}:${soft-storage-plugins-hdf5_BINARY_DIR}:${soft-kernel_BINARY_DIR}:${soft-storage-hdf5_BINARY_DIR}:${soft-storage-mongo_BINARY_DIR}:${soft-storage-plugins-json_BINARY_DIR}:${soft-storage-plugins-mongo_BINARY_DIR}:${softc_BINARY_DIR}:$ENV{PATH}")
  endif()
  
  set_property(TEST ${PROJECT_NAME} APPEND PROPERTY
    ENVIRONMENT "PYTHONPATH=${softpy_BINARY_DIR}:$ENV{PYTHONPATH}")

  set_property(TEST ${PROJECT_NAME} APPEND PROPERTY
    ENVIRONMENT "SOFTSTORAGE=${soft-storage-plugins-json_BINARY_DIR}:${soft-storage-plugins-hdf5_BINARY_DIR}:$ENV{SOFTSTORAGE}")

else()
  message("Python not found - skipping tests")
endif()
