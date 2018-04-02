cmake_minimum_required (VERSION 2.6)
project (calc_J)

# The version number
set( calcJ_VERSION_MAJOR 1)
set( calcJ_VERSION_MINOR 0)

enable_testing()

# Configure header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/calcJconfig.hpp.in"
  "${PROJECT_BINARY_DIR}/calcJconfig.hpp"
  )

SET( CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -pedantic")

file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.log" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/90_unordered")
file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/90_unordered")

file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_pair.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")
file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/ref.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")
file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_2.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")

file( COPY "${PROJECT_SOURCE_DIR}/IO/testfile.pun" DESTINATION "${PROJECT_BINARY_DIR}/IO")
file( COPY "${PROJECT_SOURCE_DIR}/IO/testfile.log" DESTINATION "${PROJECT_BINARY_DIR}/IO")

# Add the binary tree to the search path for include files
# so that we will find calcJconfig.hpp
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/CONSTANTS")
include_directories("${PROJECT_SOURCE_DIR}/STRING_SUPPORT")
include_directories("${PROJECT_SOURCE_DIR}/MATRIX")
include_directories("${PROJECT_SOURCE_DIR}/IO/ARGUMENTS/PROPERTIES")
include_directories("${PROJECT_SOURCE_DIR}/IO/ARGUMENTS")
include_directories("${PROJECT_SOURCE_DIR}/IO/FILE_READERS")
include_directories("${PROJECT_SOURCE_DIR}/IO")
include_directories("${PROJECT_SOURCE_DIR}/PARAMETERS")
include_directories("${PROJECT_SOURCE_DIR}/QC_FUNCTIONS")

link_directories("${PROJECT_SOURCE_DIR}/IO/ARGUMENTS/PROPERTIES")
link_directories("${PROJECT_SOURCE_DIR}/IO/ARGUMENTS")
link_directories("${PROJECT_SOURCE_DIR}/IO/FILE_READERS")
link_directories("${PROJECT_SOURCE_DIR}/IO")
link_directories("${PROJECT_SOURCE_DIR}/MATRIX")
link_directories("${PROJECT_SOURCE_DIR}/STRING_SUPPORT")
link_directories("${PROJECT_SOURCE_DIR}/PARAMETERS")
link_directories("${PROJECT_SOURCE_DIR}/QC_FUNCTIONS")

add_subdirectory(MATRIX)
add_subdirectory(STRING_SUPPORT)
add_subdirectory(IO)
add_subdirectory(PARAMETERS)
add_subdirectory(QC_FUNCTIONS)

add_executable(calc_J main.cpp)
target_link_libraries(calc_J QC_FUNCTIONS_SRC IO_SRC FILE_READERS_SRC PARAMETERS_SRC ARGUMENTS_SRC PROPERTIES_SRC MATRIX_SRC STRING_SUPPORT_SRC)
#target_link_libraries(calcJ MATRIX_SRC)
#target_link_libraries(calcJ PROPERTIES_SRC)
#target_link_libraries(calcJ FILE_READERS_SRC)
#target_link_libraries(calcJ ARGUMENTS_SRC)
#target_link_libraries(calcJ IO_SRC)
#target_link_libraries(calcJ PARAMETERS_SRC)
#target_link_libraries(calcJ QC_FUNCTIONS_SRC)

install( TARGETS calc_J DESTINATION bin)
