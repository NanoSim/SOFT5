project (softshell C CXX)

set (CMAKE_AUTOMOC                ON)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)

find_package (Qt5Core        REQUIRED)
find_package (Qt5Script      REQUIRED)
find_package (Qt5Concurrent  REQUIRED)
find_package (Qt5Network     REQUIRED)

qt5_add_resources(QT_RESOURCE softshell.qrc)

include_directories (
  ${soft-kernel_INCLUDE_DIRS}
  ${soft-tools-plugins-common_INCLUDE_DIRS})
include_directories ("../plugins/common")
aux_source_directory (. SRC_LIST)
add_executable(softshell ${SRC_LIST} ${QT_RESOURCE} softshell.qrc)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries (softshell soft-kernel Qt5::Core Qt5::Script Qt5::Concurrent Qt5::Network)

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
