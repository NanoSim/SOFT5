cmake_minimum_required(VERSION 3.2)
project (entities C CXX)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
set (GEN_HEADERS ${GEN_HEADERS})

function (add_forge entity)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${entity}.h
    COMMAND soft-forge ${CMAKE_CURRENT_SOURCE_DIR}/${entity}.json ${C_HEADER_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${entity}.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${entity}.json ${C_HEADER_TEMPLATE}
    )

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${entity}.c
    COMMAND soft-forge ${CMAKE_CURRENT_SOURCE_DIR}/${entity}.json ${C_SOURCE_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${entity}.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${entity}.json ${C_SOURCE_TEMPLATE} ${entity}.h
    )
  
  LIST(APPEND GEN_HEADERS ${entity}.h)
  
endfunction()

add_forge(data)

include_directories(${SOFT5_INCLUDE_DIR})
set (LINK_LIBS ${LINK_LIBS}
  ${SOFT5_KERNEL_LIBRARY}
  ${SOFT5_SOFTC_LIBRARY})

add_library(${PROJECT_NAME} SHARED data.c)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Expose public includes to other subprojects
set(${PROJECT_NAME}_INCLUDE_DIRS
  ${CMAKE_CURRENT_BINARY_DIR}
  ${SOFT5_INCLUDE_DIR}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBS
  ${PROJECT_NAME}
  ${LINK_LIBS}
  CACHE INTERNAL "${PROJECT_NAME}: Link Libs" FORCE)

message("Generated headers: ${GEN_HEADERS}")
