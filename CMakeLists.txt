cmake_minimum_required(VERSION 3.2)
include(CPack)
project(soft5)

option (WITH_WEB "Compile with Web support"               ON)
option (WITH_HSM "Compile with HSM plugin"                ON)
option (WITH_STORAGE_HD5 "HDF5 storage support"           ON)
option (WITH_STORAGE_MONGO "MongoDB storage support"      ON)
option (WITH_STORAGE_JSON "JSON storage support"          ON)
option (WITH_PLUGIN_GSL "Compile GSL plugin"              ON)
option (WITH_PLUGIN_NLOPT "Compile NLOpt plugin"          ON)
option (WITH_PLUGIN_HD5 "Compile HD5 plugin"              ON)
option (WITH_PLUGIN_MONGO "Compile HD5 plugin"            ON)
option (WITH_FOREIGN_C "Compile with C API"               ON)
option (WITH_FOREIGN_FORTRAN "Compile with Fortran API"   OFF)
option (WITH_FOREIGN_PYTHON "Compile with Python API"     OFF)
#option (WITH_FOREIGN_HASKELL "Compile with Haskell API" OFF)
option (WITH_TESTS "Compile tests"                        ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package (BSON REQUIRED)
find_package (Qt5Core REQUIRED)
find_package (Qt5Network REQUIRED) 
find_package (Qt5Script REQUIRED) 
find_package (Qt5Concurrent REQUIRED)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

if (WITH_PLUGIN_HD5 OR WITH_STORAGE_HD5)
  find_package(HDF5 REQUIRED)
endif ()

if (WITH_STORAGE_MONGO OR WITH_PLUGIN_MONGO)
  find_package(MONGOC REQUIRED)
endif()

set (SOFT_MAJOR_VERSION 5)
set (SOFT_MINOR_VERSION 1)
set (SOFT_MICRO_VERSION 3)

# Tests
# Run them with "ctest -V" or "make check" to get verbose output.
if (WITH_TESTS)
  enable_testing()
  add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose -F)
endif (WITH_TESTS)



# Install targets
set(SOFTBASE lib/${PROJECT_NAME})
set(STORAGE_PLUGINS_INSTALL_DIR ${SOFTBASE}/storage)
set(SCRIPT_MODULES_INSTALL_DIR ${SOFTBASE}/modules)
set(SCRIPT_PLUGINS_INSTALL_DIR ${SOFTBASE}/plugins)
set(INCLUDE_INSTALL_DIR include/${PROJECT_NAME})

# Core library
add_subdirectory(core/src/kernel)
if (WITH_TESTS)
add_subdirectory(core/test/kernel)
endif (WITH_TESTS)

# Storage
if (WITH_STORAGE_HD5)
  add_subdirectory(storage/src/hdf5)
  if (WITH_TESTS)
    add_subdirectory(storage/test/hdf5)
  endif (WITH_TESTS)
endif (WITH_STORAGE_HD5)

if (WITH_STORAGE_MONGO)
  add_subdirectory(storage/src/mongo)
  add_subdirectory(storage/src/plugins/mongo)
  if (WITH_TESTS)	
     add_subdirectory(storage/test/mongo)	
  endif (WITH_TESTS)
endif (WITH_STORAGE_MONGO)

if (WITH_STORAGE_JSON)
  add_subdirectory(storage/src/plugins/json)
endif (WITH_STORAGE_JSON)

if (WITH_TESTS)
#add_subdirectory(storage/test/mongo)
endif (WITH_TESTS)

# Tools
add_subdirectory(tools/src/softshell)
if (WITH_PLUGIN_MONGO)
  add_subdirectory(tools/src/plugins/mongo)
endif (WITH_PLUGIN_MONGO)

if (WITH_PLUGIN_GSL)
  add_subdirectory(tools/src/plugins/gsl)
endif(WITH_PLUGIN_GSL)

if (WITH_PLUGIN_NLOPT)
  add_subdirectory(tools/src/plugins/nlopt)
endif(WITH_PLUGIN_NLOPT)

if (WITH_PLUGIN_HD5)
  add_subdirectory(tools/src/plugins/hdf5)
endif(WITH_PLUGIN_HD5)

if (WITH_HSM)
  add_subdirectory(tools/src/plugins/hsm)
endif(WITH_HSM)

if (WITH_WEB)
  add_subdirectory(tools/src/web)
endif(WITH_WEB)


# Foreign

if (WITH_FOREIGN_C)
  add_subdirectory(foreign/c/src/softc)
endif (WITH_FOREIGN_C)

if (WITH_FOREIGN_FORTRAN)
  add_subdirectory(foreign/fortran/src/softf)
endif (WITH_FOREIGN_FORTRAN)

if (WITH_FOREIGN_PYTHON)
  add_subdirectory(foreign/python/src/softpy)
endif (WITH_FOREIGN_PYTHON)

if (WITH_FOREIGN_HASKELL)
  message("Haskell is currently not supported")
endif (WITH_FOREIGN_HASKELL)



