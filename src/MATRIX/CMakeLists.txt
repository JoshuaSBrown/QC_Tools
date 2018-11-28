add_library(MATRIX_SRC matrix.cpp)

if(${ENABLE_TESTS})
  add_executable(test_matrix test_matrix.cpp)
  target_link_libraries(test_matrix MATRIX_SRC)
  add_test(test_matrix test_matrix)
endif()
