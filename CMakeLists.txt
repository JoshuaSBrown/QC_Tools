cmake_minimum_required (VERSION 3.14)
project (catnip VERSION 2.0.0)

##############################################################################
# Defining options 
##############################################################################
option(ENABLE_TESTS "Enable testing" OFF)
option(DOWNLOAD_TUTORIAL_FILES "Download tutorial files" ${ENABLE_TESTS})
option(ENABLE_INTEGRATION_TESTING "Enable integration tests" ${ENABLE_TESTS})
option(ENABLE_UNIT_TESTING "Enable unit tests" ${ENABLE_TESTS})
option(ENABLE_PROFILING "Enable profiling" OFF)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

##############################################################################
# Override default options 
##############################################################################
set(DOWNLOAD_TUTORIAL_FILES ${ENABLE_TESTS} CACHE STRING "Download tutorial files" FORCE)
set(ENABLE_INTEGRATION_TESTING ${ENABLE_TESTS} CACHE STRING "Enable integration tests" FORCE)
set(ENABLE_UNIT_TESTING ${ENABLE_TESTS} CACHE STRING "Enable unit tests" FORCE)

##############################################################################
# Defining settings 
##############################################################################
set(catnip_YEAR_PUBLISHED 2018 )
set(catnip_AUTHOR_SURNAME "\"Brown\"" )
set(catnip_AUTHOR_INITIALS "\"J. S.\"" )
set(catnip_TITLE "\"CATNIP\"")
set(catnip_URL "\"https://github.com/JoshuaSBrown/QC_Tools\"" )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COVERAGE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
set(COMMON_LIBRARIES stdc++ m)
set(LOG_LEVEL 0 CACHE STRING "Choose the log level" )
# Prevents multiple file extensions from being appended one after the other
# important for using gcov .o.gcno or .cpp.gcno now will be .gcno
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -Werror")

##############################################################################
# Check for Dependencies 
##############################################################################

find_package(mamap REQUIRED)
if( NOT ${mamap_FOUND} )
  include(${catnip_SOURCE_DIR}/cmake/InstallMAMAP.cmake)
else()
  message(STATUS "found system installed mamap will not download")
endif()

##############################################################################
# Configuring header file with cmake variables 
##############################################################################
# Configure header file to pass some of the CMake settings to the source code
configure_file(
  "${catnip_SOURCE_DIR}/src/libcatnip/catnip_config.hpp.in"
  "${catnip_SOURCE_DIR}/src/libcatnip/catnip_config.hpp"
  )

##############################################################################
# Finding dependencies 
##############################################################################
# Find bash it is important for testing using scripts
find_library (GCOV gcov)
#find_package (Eigen3 3.3 REQUIRED NO_MODULE)
#if (TARGET Eigen3::Eigen)
#  message("-- Eigen found")
#endif(TARGET Eigen3::Eigen)
add_definitions(-DLOG_LEVEL=${LOG_LEVEL})

##############################################################################
# Setting up profiling 
##############################################################################
if(${ENABLE_PROFILING})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()
##############################################################################
# Setting up testing 
##############################################################################
message("ENable tests ${ENABLE_TESTS} and integration ${ENABLE_INTEGRATION_TESTING}")
if(${ENABLE_INTEGRATION_TESTING})
  if(EXISTS "${catnip_SOURCE_DIR}/GAUSSIANFILES")
    message(STATUS "GAUSSIANFILES found will not download")
    set(DOWNLOAD_TUTORIAL_FILES OFF)
  else(EXISTS "${catnip_SOURCE_DIR}/GAUSSIANFILES")
    message(STATUS "GAUSSIANFILES not found will attempt to download")
    include(${catnip_SOURCE_DIR}/cmake/InstallGdown.cmake)
  endif(EXISTS "${catnip_SOURCE_DIR}/GAUSSIANFILES")
endif()

if(${ENABLE_UNIT_TESTING} OR ${ENABLE_INTEGRATION_TESTING})
  find_package(Catch2 2.11.1)
  if( NOT ${Catch2_FOUND} )
    include(${catnip_SOURCE_DIR}/cmake/InstallCatch2.cmake)
  else()
    message(STATUS "found system installed Catch2 will not download")
  endif()
  enable_testing()
  include(Catch)
  include(ParseAndAddCatchTests)

  if (${ENABLE_INTEGRATION_TESTING})
    file( COPY "${catnip_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.log" 
      DESTINATION "${catnip_BINARY_DIR}/GAUSSIANFILES/90_unordered")
    file( COPY "${catnip_SOURCE_DIR}/GAUSSIANFILES/90_unordered/90_pair.pun" 
      DESTINATION "${catnip_BINARY_DIR}/GAUSSIANFILES/90_unordered")

    file( COPY "${catnip_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_pair.pun" 
      DESTINATION "${catnip_BINARY_DIR}/GAUSSIANFILES/30_unordered")
    file( COPY "${catnip_SOURCE_DIR}/GAUSSIANFILES/30_unordered/ref.pun" 
      DESTINATION "${catnip_BINARY_DIR}/GAUSSIANFILES/30_unordered")
    file( COPY "${catnip_SOURCE_DIR}/GAUSSIANFILES/30_unordered/30_2.pun" 
      DESTINATION "${catnip_BINARY_DIR}/GAUSSIANFILES/30_unordered")
  endif()

  if (${ENABLE_UNIT_TESTING})
    file( COPY "${catnip_SOURCE_DIR}/src/tests/testfile.pun" 
      DESTINATION "${catnip_BINARY_DIR}/src/tests")
    file( COPY "${catnip_SOURCE_DIR}/src/tests/testfile.log" 
      DESTINATION "${catnip_BINARY_DIR}/src/tests")
  endif()

endif()
##############################################################################
# Compiling build tree/paths 
##############################################################################
add_subdirectory("${catnip_SOURCE_DIR}/src")


