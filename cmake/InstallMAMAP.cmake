# If Catch2 is not found, instead use the git submodule
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/MAMAP/include/mamap/argument_parser.hpp)
  # Unable to find the header files for Catch2 or they don't exist
  message(STATUS "Downloading MAMAP submodule.")

  # Clone the submodule
  execute_process(COMMAND git submodule update --init --force -- external/MAMAP WORKING_DIRECTORY ${catnip_SOURCE_DIR})
endif()

add_subdirectory(${catnip_SOURCE_DIR}/external/MAMAP)
