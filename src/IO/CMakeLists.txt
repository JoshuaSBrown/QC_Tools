#set(IO_SRC io.cpp argumentparser.cpp)
if(NOT TARGET IO_SRC)
  add_library(IO_SRC io.cpp argumentparser.cpp)
  add_subdirectory(FILE_READERS)
  add_subdirectory(ARGUMENTS)
endif()

add_executable(test_io test_io.cpp)
target_link_libraries(test_io  IO_SRC FILE_READERS_SRC ARGUMENTS_SRC PROPERTIES_SRC PARAMETERS_SRC MATRIX_SRC STRING_SUPPORT_SRC)
add_test(test_io test_io -p_P ../../../GAUSSIANFILES/30/30_pair.pun -p_1 ../../../GAUSSIANFILES/30/ref.pun -p_2 ../../../GAUSSIANFILES/30/30_2.pun)

add_executable(test_argumentparser test_argumentparser.cpp)
target_link_libraries(test_argumentparser  IO_SRC FILE_READERS_SRC ARGUMENTS_SRC PROPERTIES_SRC PARAMETERS_SRC MATRIX_SRC STRING_SUPPORT_SRC)
add_test(test_argumentparser test_argumentparser)

add_custom_command(TARGET test_io 
                   POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_io> ${CMAKE_CURRENT_SOURCE_DIR})

if(BASH_PROGRAM)
  add_test(test_io_script ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test_script_io.sh ${CMAKE_CURRENT_SOURCE_DIR})
endif(BASH_PROGRAM)
