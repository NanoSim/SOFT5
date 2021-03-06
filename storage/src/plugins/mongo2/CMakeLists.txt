project (soft-storage-plugins-mongo2 C CXX)

set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (CMAKE_AUTOMOC TRUE)

include_directories (${BSON_INCLUDE_DIR} ${MONGOC_INCLUDE_DIR} ${soft-kernel_INCLUDE_DIRS} ${soft-storage-mongo_INCLUDE_DIRS})
set (LINK_LIBS ${LINK_LIBS} ${soft-kernel_LIBS} ${soft-storage-mongo_LIBS})

aux_source_directory (. SRC_LIST)
add_definitions(-DQT_PLUGIN)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# TODO: We're making this refer to json/ even though it is a storage plugin, is this OK?
set(${PROJECT_NAME}_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/..
  CACHE INTERNAL "${PROJECT_NAME}: Include directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${STORAGE_PLUGINS_INSTALL_DIR}
  ARCHIVE DESTINATION ${STORAGE_PLUGINS_INSTALL_DIR}
  )
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/${STORAGE_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so )")
install(CODE "MESSAGE(\"-- Created symlink: ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/lib${PROJECT_NAME}.so -> ${CMAKE_INSTALL_PREFIX}/${STORAGE_PLUGINS_INSTALL_DIR}/lib${PROJECT_NAME}.so  \")")
