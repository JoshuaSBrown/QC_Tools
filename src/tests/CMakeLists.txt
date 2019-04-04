
if(${ENABLE_TESTS})
  foreach(PROG 
      test_argumentdouble
      test_argumentfile
      test_argumentint
      test_argumentparser
      test_argumentstring
      test_argumentswitch
      test_io
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
endif(${ENABLE_TESTS})
