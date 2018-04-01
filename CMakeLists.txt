cmake_minimum_required (VERSION 2.6)
project (calcJ)

# The version number
set( calc_J_VERSION_MAJOR 1)
set( calc_J_VERSION_MINOR 0)

# Configure header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/calcJconfig.hpp.in"
  "${PROJECT_BINARY_DIR}/calcJconfig.hpp"
  )

SET( CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -pedantic")

# Add the binary tree to the search path for include files
# so that we will find calcJconfig.hpp
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/IO")
include_directories("${PROJECT_SOURCE_DIR}/IO/ARGUMENTS")
include_directories("${PROJECT_SOURCE_DIR}/IO/ARGUMENTS/PROPERTIES")
include_directories("${PROJECT_SOURCE_DIR}/IO/FILE_READERS")
include_directories("${PROJECT_SOURCE_DIR}/CONSTANTS")
include_directories("${PROJECT_SOURCE_DIR}/MATRIX")
include_directories("${PROJECT_SOURCE_DIR}/PARAMETERS")
include_directories("${PROJECT_SOURCE_DIR}/QC_FUNCTIONS")
include_directories("${PROJECT_SOURCE_DIR}/STRING_SUPPORT")

add_subdirectory(IO)
add_subdirectory(MATRIX)
add_subdirectory(PARAMETERS)
add_subdirectory(QC_FUNCTIONS)
add_subdirectory(STRING_SUPPORT)

add_executable(calcJ main.cpp)
target_link_libraries(calcJ IO_SRC)
target_link_libraries(calcJ FILE_READERS_SRC)
target_link_libraries(calcJ ARGUMENTS_SRC)
target_link_libraries(calcJ PROPERTIES_SRC)
target_link_libraries(calcJ CONSTANTS_SRC)
target_link_libraries(calcJ MATRIX_SRC)
target_link_libraries(calcJ PARAMETERS_SRC)
target_link_libraries(calcJ QC_FUNCTIONS_SRC)
target_link_libraries(calcJ STRING_SUPPORT_SRC)

install( TARGETS calcJ DESTINATION bin)
