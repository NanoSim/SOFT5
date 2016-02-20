project (autogen-test C)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_THREAD_PREFER_PTHREAD TRUE)

aux_source_directory(. SRC_LIST)

set (SRC_LIST
  ${SRC_LIST}
  )
set (LINK_LIBS
  ${LINK_LIBS}
  ${testlib-f90_LIBS}
  ${entities_LIBS}
  )
include_directories(
  ${testlib-f90_INCLUDE_DIRS}
  ${entities_INCLUDE_DIRS}
  )

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})