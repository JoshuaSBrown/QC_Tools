project(ARGUMENTS_SRC)

if(NOT TARGET ARGUMENTS_SRC)
  add_library(ARGUMENTS_SRC argumentint.cpp argumentswitch.cpp argumentdouble.cpp argumentstring.cpp argumentfile.cpp)
  add_subdirectory(PROPERTIES)

  foreach(PROG test_argumentint test_argumentswitch test_argumentdouble test_argumentstring test_argumentfile)
    add_executable(${PROG} ${PROG}.cpp)
    add_dependencies(${PROG} STRING_SUPPORT_SRC PROPERTIES_SRC ARGUMENTS_SRC)
    add_dependencies(PROPERTIES_SRC STRING_SUPPORT_SRC)
    add_dependencies(ARGUMENTS_SRC PROPERTIES_SRC STRING_SUPPORT_SRC)
    target_link_libraries(${PROG} ARGUMENTS_SRC PROPERTIES_SRC STRING_SUPPORT_SRC)
    add_test(${PROG} ${PROG})  
  endforeach(PROG)
endif()


