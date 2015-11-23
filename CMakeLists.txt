cmake_minimum_required(VERSION 3.2)
project(soft5)

option (WITH_WEB "Compile with Web support"               ON)
option (WITH_HSM "Compile with HSM plugin"                ON)
option (WITH_PLUGIN_GSL "Compile GSL plugin"              ON)
option (WITH_PLUGIN_NLOPT "Compile NLOpt plugin"          ON)
option (WITH_FOREIGN_C "Compile with C API"               ON)
option (WITH_FOREIGN_FORTRAN "Compile with Fortran API"   ON)
#option (WITH_FOREIGN_PYTHON "Compile with Python API" OFF)
#option (WITH_FOREIGN_HASKELL "Compile with Haskell API" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (SOFT_MAJOR_VERSION 5)
set (SOFT_MINOR_VERSION 0)
set (SOFT_MICRO_VERSION 1)

# Core library
add_subdirectory(core/src/kernel)

# Storage
add_subdirectory(storage/src/hdf5)
add_subdirectory(storage/src/mongo)
add_subdirectory(storage/src/plugins/mongo)
add_subdirectory(storage/src/plugins/json)

# Tools
add_subdirectory(tools/src/softshell)
add_subdirectory(tools/src/plugins/mongo)

if (WITH_PLUGIN_GSL)
  add_subdirectory(tools/src/plugins/gsl)
endif(WITH_PLUGIN_GSL)

if (WITH_PLUGIN_NLOPT)
  add_subdirectory(tools/src/plugins/nlopt)
endif(WITH_PLUGIN_NLOPT)

if (WITH_HSM)
  add_subdirectory(tools/src/hsm)
endif(WITH_HSM)

if (WITH_WEB)
  add_subdirectory(tools/src/web/webserver)
endif(WITH_WEB)


# Foreign

if (WITH_FOREIGN_C)
  add_subdirectory(foreign/c/src/softc)
endif (WITH_FOREIGN_C)

if (WITH_FOREIGN_FORTRAN)
  add_subdirectory(foreign/fortran/src/softf)
endif (WITH_FOREIGN_FORTRAN)

if (WITH_FOREIGN_PYTHON)
  message("Python is currently not supported")
endif (WITH_FOREIGN_PYTHON)

if (WITH_FOREIGN_HASKELL)
  message("Haskell is currently not supported")
endif (WITH_FOREIGN_HASKELL)
