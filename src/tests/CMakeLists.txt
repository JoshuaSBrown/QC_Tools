
if(${ENABLE_TESTS})
  foreach(PROG 
      test_argumentdouble
      test_argumentfile
      test_argumentint
      test_argumentparser
      test_argumentstring
      test_argumentswitch
      test_log
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
      test_qc_functions
      test_string_support)

    add_executable(unit_${PROG} ${PROG}.cpp)
    target_link_libraries(unit_${PROG} libcatnip)
    add_test(unit_${PROG} unit_${PROG})

  endforeach(PROG)

endif(${ENABLE_TESTS})

if(${ENABLE_REGRESSION_TESTS})

  foreach(PROG 
      test_logreader
      test_punreader)

    add_executable(regression_${PROG} ${PROG}.cpp)
    target_link_libraries(regression_${PROG} libcatnip)
    add_test(regression_${PROG} regression_${PROG})

  endforeach(PROG)

  add_executable(test_io test_io.cpp)
  target_link_libraries(test_io libcatnip)
  add_test(regression_test_io test_io -p_P "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30/30_pair.pun" -p_1 "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30/ref.pun" -p_2 "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30/30_2.pun")

  add_custom_command(TARGET test_io
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_io> ${CMAKE_CURRENT_SOURCE_DIR})

  if(BASH_PROGRAM)
    add_test(test_io_script ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test_script_io.sh ${CMAKE_CURRENT_SOURCE_DIR})
  endif(BASH_PROGRAM)

endif(${ENABLE_REGRESSION_TESTS})
