# - Find numpy
# Find the native numpy includes
# This module defines
#  NUMPY_INCLUDE_DIR, where to find numpy/arrayobject.h, etc.
#  NUMPY_FOUND, If false, do not try to use numpy headers.
#
# Verbose output can be suppressed by setting NUMPY_FIND_QUIETLY to TRUE.
#

EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c 
    "from __future__ import print_function; import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE numpy_incdir
    OUTPUT_STRIP_TRAILING_WHITESPACE)

if (numpy_incdir)
  if(EXISTS ${NUMPY_INCLUDE_DIR}/numpy/arrayobject.h)
    # successful
    set (NUMPY_FOUND TRUE)
    set (NUMPY_INCLUDE_DIR "${numpy_incdir}" CACHE STRING "Numpy include path")
  else()
    set(NUMPY_FOUND FALSE)
  endif()
else()
  # Did not successfully include numpy
  set(NUMPY_FOUND FALSE)
endif()

if (NUMPY_FOUND)
  if (NOT NUMPY_FIND_QUIETLY)
    message (STATUS "Numpy headers found")
  endif (NOT NUMPY_FIND_QUIETLY)
else (NUMPY_FOUND)
  if (NUMPY_FIND_REQUIRED)
    message (FATAL_ERROR "Numpy headers missing")
  endif (NUMPY_FIND_REQUIRED)
endif (NUMPY_FOUND)

MARK_AS_ADVANCED (NUMPY_INCLUDE_DIR)
