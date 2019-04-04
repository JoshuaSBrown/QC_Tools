
file(GLOB CATNIP_SOURCE_FILES 
  *.cpp 
  io/*.cpp
  io/arguments/*.cpp 
  io/arguments/properties/*.cpp 
  io/file_readers/*.cpp)

add_library(libcatnip ${CATNIP_SOURCE_FILES})
