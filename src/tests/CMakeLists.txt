
if(${ENABLE_TESTS})
  foreach(PROG 
      test_log
      test_matrix
      test_parameters
      test_qc_functions
      test_string_support)

    add_executable(unit_${PROG} ${PROG}.cpp)
    target_link_libraries(unit_${PROG} libcatnip)
    add_test(unit_${PROG} unit_${PROG})

  endforeach(PROG)
endif(${ENABLE_TESTS})
