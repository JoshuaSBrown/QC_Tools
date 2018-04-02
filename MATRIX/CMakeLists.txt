add_library(MATRIX_SRC matrix.cpp)

add_executable(test_matrix test_matrix.cpp)
target_link_libraries(test_matrix MATRIX_SRC)
add_test(test_matrix test_matrix)
