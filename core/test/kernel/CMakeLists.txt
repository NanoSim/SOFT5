project(core-test-kernel C CXX)

find_package(GTest)

if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(${soft-kernel_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} soft-kernel ${GTEST_LIBRARIES}) 
  
  add_test(${PROJECT_NAME} ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
