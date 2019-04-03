#set(PARAMETERS_SRC parameters.cpp)
link_directories("${PROJECT_SOURCE_DIR}/STRING_SUPPORT")

add_library(PARAMETERS_SRC parameters.cpp)

if(${ENABLE_TESTS})
  add_executable(test_parameters test_parameters.cpp)
  add_dependencies(test_parameters STRING_SUPPORT_SRC PARAMETERS_SRC)
  target_link_libraries(test_parameters STRING_SUPPORT_SRC)
  target_link_libraries(test_parameters PARAMETERS_SRC)
  add_test(test_parameters test_parameters)
endif(${ENABLE_TESTS})
