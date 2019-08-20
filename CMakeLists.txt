cmake_minimum_required (VERSION 2.8)
project (calc_J)

##############################################################################
# Defining options 
##############################################################################
option(DOWNLOAD_TUTORIAL_FILES "Download tutorial files" OFF)
option(ENABLE_INTEGRATION_TESTS "Enable integration tests" OFF)
option(ENABLE_TESTS "Enable tests" OFF)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

##############################################################################
# Defining settings 
##############################################################################
set(calcJ_VERSION_MAJOR 1 )
set(calcJ_VERSION_MINOR 9 )
set(calcJ_YEAR_PUBLISHED 2018 )
set(calcJ_AUTHOR_SURNAME "\"Brown\"" )
set(calcJ_AUTHOR_INITIALS "\"J. S.\"" )
set(calcJ_TITLE "\"CATNIP\"")
set(calcJ_URL "\"https://github.com/JoshuaSBrown/QC_Tools\"" )
set(COVERAGE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
set(COMMON_LIBRARIES stdc++ m)
set(LOG_LEVEL 0 CACHE INT "Choose the log level" )
# Prevents multiple file extensions from being appended one after the other
# important for using gcov .o.gcno or .cpp.gcno now will be .gcno
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -pedantic -Werror")

##############################################################################
# Configuring header file with cmake variables 
##############################################################################
# Configure header file to pass some of the CMake settings to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/src/libcatnip/calcJconfig.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/libcatnip/calcJconfig.hpp"
  )

##############################################################################
# Finding dependencies 
##############################################################################
# Find bash it is important for testing using scripts
find_program (BASH_PROGRAM bash)
find_library (GCOV gcov)
add_definitions(-DLOG_LEVEL=${LOG_LEVEL})

##############################################################################
# Setting up testing 
##############################################################################
if(ENABLE_INTEGRATION_TESTS)
  if(EXISTS "${PROJECT_SOURCE_DIR}/GAUSSIANFILES")
    message("GAUSSIANFILES found will not download")
    set(DOWNLOAD_TUTORIAL_FILES OFF)
  else(EXISTS "${PROJECT_SOURCE_DIR}/GAUSSIANFILES")
    message("GAUSSIANFILES not found will download")
    set(DOWNLOAD_TUTORIAL_FILES ON)
  endif(EXISTS "${PROJECT_SOURCE_DIR}/GAUSSIANFILES")
endif()

if(DOWNLOAD_TUTORIAL_FILES)
  message("Downloading gaussian tutorial files")
  execute_process(COMMAND ${PROJECT_SOURCE_DIR}/scripts/test_suite_install.bash "${PROJECT_SOURCE_DIR}" )
endif()

if(ENABLE_TESTS OR ENABLE_INTEGRATION_TESTS)
  enable_testing()
  add_subdirectory("${PROJECT_SOURCE_DIR}/src/tests")
endif()

if (ENABLE_INTEGRATION_TESTS)
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.log" 
    DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/90_unordered")
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.pun" 
    DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/90_unordered")

  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_pair.pun" 
    DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/ref.pun" 
    DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")
  file( COPY "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_2.pun" 
    DESTINATION "${PROJECT_BINARY_DIR}/GAUSSIANFILES/30_unordered")
endif()

if (ENABLE_TESTS)
  file( COPY "${PROJECT_SOURCE_DIR}/src/tests/testfile.pun" 
    DESTINATION "${PROJECT_BINARY_DIR}/src/tests")
  file( COPY "${PROJECT_SOURCE_DIR}/src/tests/testfile.log" 
    DESTINATION "${PROJECT_BINARY_DIR}/src/tests")
endif()

##############################################################################
# Compiling build tree/paths 
##############################################################################
# Add the binary tree to the search path for include files
# so that we will find calcJconfig.hpp
include_directories("${PROJECT_BINARY_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/libcatnip")
include_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io/arguments/properties")
include_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io/arguments")
include_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io/file_readers")
include_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io")

link_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io/arguments/properties")
link_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io/arguments")
link_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io/file_readers")
link_directories("${PROJECT_SOURCE_DIR}/src/libcatnip/io")
link_directories("${PROJECT_SOURCE_DIR}/src/libcatnip")

add_subdirectory("${PROJECT_SOURCE_DIR}/src/libcatnip")

##############################################################################
# Creating calc_J and setting up code coverage if on  
##############################################################################
add_executable(calc_J src/tools/main.cpp)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_link_libraries(calc_J gcov)
  set_source_files_properties( src/tools/main.cpp PROPERTIES COMPILE_FLAGS ${COVERAGE_FLAGS})
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
target_link_libraries(calc_J libcatnip ${COMMON_LIBRARIES})
install( TARGETS calc_J DESTINATION bin)
