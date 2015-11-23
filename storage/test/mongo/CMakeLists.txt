project(storage-test-mongo C CXX)

find_package(GTest)
find_package(MongoC REQUIRED)
find_package(BSON   REQUIRED)

if(GTEST_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  aux_source_directory(. SRC_LIST)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
  include_directories(
    ${soft-kernel_INCLUDE_DIRS} 
    ${soft-storage-mongo_INCLUDE_DIRS} 
    ${MongoC_INCLUDE_DIR}
    ${BSON_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} 
    soft-kernel 
    soft-storage-mongo
    ${GTEST_BOTH_LIBRARIES}) 

  add_test(${PROJECT_NAME} ${RUNNER} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

else(GTEST_FOUND)
  message("Google test not found - skipping tests")
endif(GTEST_FOUND)
