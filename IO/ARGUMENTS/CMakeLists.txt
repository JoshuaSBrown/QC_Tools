
if(NOT TARGET ARGUMENTS_SRC)
  file(GLOB SRCS argument*.cpp)
  add_library(ARGUMENTS_SRC ${SRCS})
  add_subdirectory(PROPERTIES)
endif()
