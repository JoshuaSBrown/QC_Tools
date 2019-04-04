
file(GLOB CATNIP_SOURCE_FILES 
  *.cc 
  io/*.cc
  io/arguments/*.cc 
  io/arguments/properties/*.cc 
  io/file_readers/*.cc)

add_library(libcatnip ${CATNIP_SOURCE_FILES}_)
