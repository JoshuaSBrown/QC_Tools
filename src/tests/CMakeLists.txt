
if(${ENABLE_TESTS})
  foreach(PROG 
      test_argumentdouble
      test_argumentfile
      test_argumentint
      test_argumentparser
      test_argumentstring
      test_argumentswitch
      test_log
      test_logreader
      test_matrix
      test_parameters
      test_propertydouble
      test_propertyfileexist
      test_propertyfileext
      test_propertyint
      test_propertysisterfile
      test_propertystringchoice
      test_propertystring
      test_propertyswitch
      test_punreader
      test_qc_functions
      test_string_support)

    add_executable(unit_${PROG} ${PROG}.cpp)
    target_link_libraries(unit_${PROG} libcatnip)
    add_test(unit_${PROG} unit_${PROG})

  endforeach(PROG)

  add_executable(regression_test_io test_io.cpp)
  target_link_libraries(regression_test_io libcatnip)
  add_test(regression_test_io regression_test_io -p_P ../../GAUSSIANFILES/30/30_pair.pun -p_1 ../../GAUSSIANFILES/30/ref.pun -p_2 ../../GAUSSIANFILES/30/30_2.pun)

  add_custom_command(TARGET regression_test_io
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_io> ${CMAKE_CURRENT_SOURCE_DIR})

  if(BASH_PROGRAM)
    add_test(test_io_script ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test_script_io.sh ${CMAKE_CURRENT_SOURCE_DIR})
  endif(BASH_PROGRAM)

endif(${ENABLE_TESTS})
