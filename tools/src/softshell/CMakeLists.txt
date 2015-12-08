project (softshell C CXX)

set (CMAKE_AUTOMOC                ON)
set (CMAKE_INCLUDE_CURRENT_DIR    ON)
set (CMAKE_THREAD_PREFER_PTHREAD  TRUE)

qt5_add_resources(QT_RESOURCE softshell.qrc)

include_directories (
  ${soft-kernel_INCLUDE_DIRS}
  ${soft-tools-plugins-common_INCLUDE_DIRS})
include_directories ("../plugins/common")
set (LINK_LIBS ${LINK_LIBS} ${HDF5_LIBRARIES} ${soft-kernel_LIBS})

aux_source_directory (. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${QT_RESOURCE} softshell.qrc)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries (${PROJECT_NAME} ${LINK_LIBS})

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install (DIRECTORY modules/soft DESTINATION ${SCRIPT_MODULES_INSTALL_DIR})
install (DIRECTORY modules/utils DESTINATION ${SCRIPT_MODULES_INSTALL_DIR})
