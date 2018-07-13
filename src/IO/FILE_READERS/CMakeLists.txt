#set(FILE_READERS_SRC filereader.cpp punreader.cpp logreader.cpp)
#set_property(GLOBAL)
add_library(FILE_READERS_SRC  filereader.cpp punreader.cpp logreader.cpp)

add_executable(test_punreader test_punreader.cpp)
#add_dependencies(test_punreader STRING_SUPPORT_SRC MATRIX_SRC FILE_READERS_SRC)
target_link_libraries(test_punreader FILE_READERS_SRC MATRIX_SRC STRING_SUPPORT_SRC)

add_test(test_punreader test_punreader)

add_executable(test_logreader test_logreader.cpp)
#add_dependencies(test_logreader STRING_SUPPORT_SRC MATRIX_SRC FILE_READERS_SRC)
target_link_libraries(test_logreader FILE_READERS_SRC MATRIX_SRC STRING_SUPPORT_SRC)
add_test(test_logreader test_logreader)
