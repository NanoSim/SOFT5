project(tools-plugins-softkernel-test CXX)

add_test(NAME ${PROJECT_NAME}
  COMMAND ${softshell_BINARY_DIR}/softshell ${CMAKE_CURRENT_SOURCE_DIR}/soft-kernel-test.js
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(UNIX)
  set_property(TEST ${PROJECT_NAME} PROPERTY
    ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}:${soft-tools-plugins-softkernel_BINARY_DIR}:${soft-storage-plugins-json_BINARY_DIR}"
    ENVIRONMENT "SOFT_MODULES=${softshell_SOURCE_DIR}/modules")    
else()
  set_property(TEST ${PROJECT_NAME} PROPERTY
    ENVIRONMENT "PATH=${PATH};$ENV{PATH};${soft-tools-plugins-softkernel_BINARY_DIR};${soft-storage-plugins-json_BINARY_DIR}"
    ENVIRONMENT "SOFT_MODULES=${softshell_SOURCE_DIR}/modules")
endif()

