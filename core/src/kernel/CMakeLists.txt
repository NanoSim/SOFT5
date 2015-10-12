cmake_minimum_required(VERSION 3.2)
project (soft-kernel C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(BSON REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network)
find_package(Qt5Script)
find_package(Qt5Concurrent)

include_directories (${BSON_INCLUDE_DIR})
set (LIBS ${LIBS} ${BSON_LIBRARIES})

aux_source_directory(. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Script Qt5::Concurrent )

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY . DESTINATION include/soft5 FILES_MATCHING PATTERN "*.h")
install(FILES Soft DESTINATION include/soft5)
