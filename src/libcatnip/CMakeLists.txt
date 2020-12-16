
file(GLOB CATNIP_SOURCE_FILES 
  io/io.cpp
  string_support.cpp 
  io/file_readers/*.cpp
  *.cpp)

add_library(libcatnip ${CATNIP_SOURCE_FILES})
set_property(TARGET libcatnip PROPERTY CXX_STANDARD 17)
target_include_directories(libcatnip PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libcatnip PUBLIC mamap)
