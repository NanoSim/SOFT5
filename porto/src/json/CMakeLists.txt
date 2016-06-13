project (porto-json CXX)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC TRUE)

find_package(Qt5Core REQUIRED)

set(${PROJECT_NAME}_INCLUDE_DIRS
  ${softc_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include_directories (${porto-json_INCLUDE_DIRS})

set (LINK_LIBS Qt5::Core ${softc_LIBS})

aux_source_directory (. SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)

target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# TODO: This needs a specified PORTO_INSTALL_DIR -- where is this defined?
# install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${PORTO_INSTALL_DIR})
