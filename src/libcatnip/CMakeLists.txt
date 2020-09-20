
file(GLOB CATNIP_SOURCE_FILES 
  io/arguments/*.cpp 
  io/arguments/properties/*.cpp
  io/argumentparser.cpp
  string_support.cpp 
  io/file_readers/*.cpp)

add_library(libcatnip ${CATNIP_SOURCE_FILES})
set_property(TARGET libcatnip PROPERTY CXX_STANDARD 17)
target_include_directories(libcatnip PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
