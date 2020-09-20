list( APPEND UNIT_TEST_SOURCE_FILES
  test_propertynumerical
  test_propertyfileexist
  test_propertyfileext
  test_propertysisterfile
  test_propertystringchoice
  test_propertystring
  test_propertyswitch
)

#
#  test_argumentnumerical
#  test_argumentfile
#  test_argumentparser
#  test_argumentstring
#  test_argumentswitch
#
#  test_atom
#  test_atom_group
#  test_atom_group_container
#  test_atom_system
#  test_basis_map
#  test_elements
#  test_log
#  test_parameters
#
#  test_qc_functions
#  test_string_support
#  test_swap_engine

if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    
  find_library (GCOV gcov)
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    

if(ENABLE_UNIT_TESTING)
  foreach(PROG IN LISTS UNIT_TEST_SOURCE_FILES)
    add_executable(unit_${PROG} ${PROG}.cpp)
    if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    
      set_source_files_properties( ${PROG}.cpp PROPERTIES COMPILE_FLAGS ${COVERAGE_FLAGS} )
    endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    
    target_link_libraries(unit_${PROG} libcatnip Catch2::Catch2)
    ParseAndAddCatchTests(unit_${PROG})
  endforeach(PROG)
endif(ENABLE_UNIT_TESTING)

if(ENABLE_INTEGRATION_TESTING)

  foreach(PROG 
      test_logreader
      test_punreader)

    add_executable(integration${PROG} ${PROG}.cpp)
    if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    
      set_source_files_properties( ${PROG}.cpp PROPERTIES COMPILE_FLAGS ${COVERAGE_FLAGS})
    endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    

    target_link_libraries(integration${PROG} libcatnip Catch2::Catch2)
    add_test(integration${PROG} integration${PROG})

  endforeach(PROG)

#  add_executable(test_io test_io.cpp)
#  if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    
#    set_source_files_properties( test_io.cpp PROPERTIES COMPILE_FLAGS ${COVERAGE_FLAGS})
#  endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")    
#  target_link_libraries(test_io libcatnip)
#  add_test(regression_test_io test_io -p_P "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30/30_pair.pun" -p_1 "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30/ref.pun" -p_2 "${PROJECT_SOURCE_DIR}/GAUSSIANFILES/30/30_2.pun")
#
#  add_custom_command(TARGET test_io
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_io> ${CMAKE_CURRENT_SOURCE_DIR})
#
#  find_program (BASH_PROGRAM bash)
  if(BASH_PROGRAM)
    #    add_test(regression_test_io_script ${BASH_PROGRAM} ${PROJECT_SOURCE_DIR}/src/tests/test_script_io.sh ${CMAKE_CURRENT_SOURCE_DIR})

    add_test(integration_test_calc_J_script ${BASH_PROGRAM} ${PROJECT_SOURCE_DIR}/src/tests/test_script_calc_J.sh ${CMAKE_SOURCE_DIR})

  else()
    message(STATUS "Unable to add integration tests, unable to find bash")
  endif(BASH_PROGRAM)

endif(ENABLE_INTEGRATION_TESTING)



