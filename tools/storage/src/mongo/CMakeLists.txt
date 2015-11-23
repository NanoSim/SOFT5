cmake_minimum_required(VERSION 3.2)
project (soft-storage-mongo C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(Qt5Core   REQUIRED)
find_package(BSON      REQUIRED)
find_package(MongoC    REQUIRED)


include_directories (${MongoC_INCLUDE_DIR} ${BSON_INCLUDE_DIR} ${soft-kernel_INCLUDE_DIRS})

set (LINK_LIBS ${LINK_LIBS} ${BSON_LIBRARIES} ${MongoC_LIBRARIES})

aux_source_directory (. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} Qt5::Core ${LINK_LIBS} soft-kernel)

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY . DESTINATION include/soft5 FILES_MATCHING PATTERN "*.h")
install(FILES QMongo DESTINATION include/soft5)

