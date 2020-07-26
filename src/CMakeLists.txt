add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libcatnip")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tools")
if(ENABLE_UNIT_TESTING OR ENABLE_INTEGRATION_TESTING)
  message(STATUS "adding test directory")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()


