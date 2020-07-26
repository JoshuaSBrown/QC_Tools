
#
#if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/Catch2/includes")
#  find_program(CURL curl)
#  if( NOT CURL)
#    message(FATAL_ERROR "Unable to download Catch2, unable to locate curl, please install curl")
#  else()
#    execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/external/Catch2/single_include/catch2)
#    execute_process(COMMAND ${CURL} https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp -o ${CMAKE_SOURCE_DIR}/external/Catch2/single_include/catch2/catch.hpp)
#  endif()
#  add_library(Catch2 INTERFACE) 
#  target_include_directories(Catch2 INTERFACE ${CMAKE_SOURCE_DIR}/external/Catch2/single_include)
#endif()

# If Catch2 is not found, instead use the git submodule
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/Catch2/single_include)
  # Unable to find the header files for Catch2 or they don't exist
  message(STATUS "Downloading Catch2 submodule.")

  # Clone the submodule
  execute_process(COMMAND git submodule update --init --force -- external/Catch2 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/external/Catch2)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/Catch2/contrib")
