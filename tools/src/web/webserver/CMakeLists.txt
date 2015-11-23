project (softweb C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_AUTOMOC                ON)

find_package(Qt5Core    REQUIRED)
find_package(Qt5Script  REQUIRED)
find_package(Qt5Network REQUIRED)

qt5_add_resources(QT_RESOURCE webserver.qrc)

include_directories (
  ${soft-kernel_INCLUDE_DIRS})

aux_source_directory (. SRC_LIST)
add_executable(softweb ${SRC_LIST} ${QT_RESOURCE} webserver.qrc)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries (${PROJECT_NAME} soft-kernel Qt5::Core Qt5::Script Qt5::Network)

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
