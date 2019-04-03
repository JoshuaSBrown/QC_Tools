cmake_minimum_required (VERSION 2.8)
project (calc_J)

# The version number
set( calcJ_VERSION_MAJOR 1 )
set( calcJ_VERSION_MINOR 8 )
set( calcJ_YEAR_PUBLISHED 2018 )
set( calcJ_AUTHOR_SURNAME "\"Brown\"" )
set( calcJ_AUTHOR_INITIALS "\"J. S.\"" )
set( calcJ_TITLE "\"CATNIP\"")
set( calcJ_URL "\"https://github.com/JoshuaSBrown/QC_Tools\"" )

option(ENABLE_REGRESSION_TESTS "Enable Regression tests" OFF)
option(ENABLE_TESTS "Enable tests" OFF)
if (${ENABLE_TESTS})
  enable_testing()
endif()
# Configure header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/src/calcJconfig.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/calcJconfig.hpp"
  )

# Find bash it is important for testing using scripts
find_program (BASH_PROGRAM bash)

SET( LOG_LEVEL 0 CACHE INT "Choose the log level" )
add_definitions(-DLOG_LEVEL=${LOG_LEVEL})

SET( CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -pedantic -Werror")

# Code Coverage Configuration
add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif(CODE_COVERAGE)

if (${ENABLE_REGRESSION_TESTS})
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.log" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/90_unordered")
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/90_unordered")

  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_pair.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/ref.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_2.pun" DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")

  file( COPY "${PROJECT_SOURCE_DIR}/src/IO/testfile.pun" DESTINATION "${PROJECT_BINARY_DIR}/src/IO")
  file( COPY "${PROJECT_SOURCE_DIR}/src/IO/testfile.log" DESTINATION "${PROJECT_BINARY_DIR}/src/IO")
endif()

# Add the binary tree to the search path for include files
# so that we will find calcJconfig.hpp
include_directories("${PROJECT_BINARY_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/CONSTANTS")
include_directories("${PROJECT_SOURCE_DIR}/src/STRING_SUPPORT")
include_directories("${PROJECT_SOURCE_DIR}/src/MATRIX")
include_directories("${PROJECT_SOURCE_DIR}/src/IO/ARGUMENTS/PROPERTIES")
include_directories("${PROJECT_SOURCE_DIR}/src/IO/ARGUMENTS")
include_directories("${PROJECT_SOURCE_DIR}/src/IO/FILE_READERS")
include_directories("${PROJECT_SOURCE_DIR}/src/IO")
include_directories("${PROJECT_SOURCE_DIR}/src/PARAMETERS")
include_directories("${PROJECT_SOURCE_DIR}/src/QC_FUNCTIONS")

link_directories("${PROJECT_SOURCE_DIR}/src/IO/ARGUMENTS/PROPERTIES")
link_directories("${PROJECT_SOURCE_DIR}/src/IO/ARGUMENTS")
link_directories("${PROJECT_SOURCE_DIR}/src/IO/FILE_READERS")
link_directories("${PROJECT_SOURCE_DIR}/src/IO")
link_directories("${PROJECT_SOURCE_DIR}/src/MATRIX")
link_directories("${PROJECT_SOURCE_DIR}/src/STRING_SUPPORT")
link_directories("${PROJECT_SOURCE_DIR}/src/PARAMETERS")
link_directories("${PROJECT_SOURCE_DIR}/src/QC_FUNCTIONS")

add_subdirectory(src/MATRIX)
add_subdirectory(src/STRING_SUPPORT)
add_subdirectory(src/IO)
add_subdirectory(src/PARAMETERS)
add_subdirectory(src/QC_FUNCTIONS)

add_executable(calc_J src/main.cpp)
target_link_libraries(calc_J QC_FUNCTIONS_SRC IO_SRC FILE_READERS_SRC PARAMETERS_SRC ARGUMENTS_SRC PROPERTIES_SRC MATRIX_SRC STRING_SUPPORT_SRC)

if(BASH_PROGRAM)
  if(${ENABLE_REGRESSION_TESTS})
  add_test(test_calc_J_script ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/src/test_script_calc_J.sh ${CMAKE_SOURCE_DIR})
  endif(${ENABLE_REGRESSION_TESTS})
endif(BASH_PROGRAM)

install( TARGETS calc_J DESTINATION bin)
