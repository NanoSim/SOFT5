project (soft-tools-plugins-mongo C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(Qt5Core REQUIRED)

include_directories (
  ${soft-storage-mongo_INCLUDE_DIRS}
  "../common")

set (LINK_LIBS ${LINK_LIBS} ${soft-storage-mongo_LIBS})
aux_source_directory (. SRC_LIST)

add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Export/import declaration for Windows
generate_export_header(${PROJECT_NAME})
install_runtime_library(${PROJECT_NAME} ${SCRIPT_PLUGINS_INSTALL_DIR})
