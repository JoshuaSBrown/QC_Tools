add_library(FILE_READERS_SRC  filereader.cpp punreader.cpp logreader.cpp)

if(${ENABLE_TESTS})
  add_executable(test_punreader test_punreader.cpp)
  target_link_libraries(test_punreader FILE_READERS_SRC MATRIX_SRC STRING_SUPPORT_SRC)

  add_test(test_punreader test_punreader)

  add_executable(test_logreader test_logreader.cpp)
  target_link_libraries(test_logreader FILE_READERS_SRC MATRIX_SRC STRING_SUPPORT_SRC)
  add_test(test_logreader test_logreader)
endif()
