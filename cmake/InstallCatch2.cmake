# If Catch2 is not found, instead use the git submodule
if (NOT EXISTS ${catnip_CURRENT_SOURCE_DIR}/external/Catch2/single_include)
  # Unable to find the header files for Catch2 or they don't exist
  message(STATUS "Downloading Catch2 submodule.")

  # Clone the submodule
  execute_process(COMMAND git submodule update --init --force -- external/Catch2 WORKING_DIRECTORY ${catnip_SOURCE_DIR})
endif()

add_subdirectory(${catnip_SOURCE_DIR}/external/Catch2)
list(APPEND CMAKE_MODULE_PATH "${catnip_SOURCE_DIR}/external/Catch2/contrib")
