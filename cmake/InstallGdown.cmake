
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/GAUSSIANFILES")
  find_program(GDOWN gdown ~/.local/bin)

  if( NOT GDOWN)
    find_program(PIP NAMES pip pip3 PATHS ~/.local/bin)
    if( NOT PIP)
      message(FATAL_ERROR "Unable to download tutorial files with gdown, tutorial"
      "files are needed to run both the tutorials and the integration tests," 
      "unable to locate pip to install gdown, please install pip, or install gdown"
      "through some other means.")
    else()
      message("Pip is ")
      execute_process(COMMAND ${PIP} "install" "--user" "--upgrade" "pip")
      execute_process(COMMAND ${PIP} "install" "--user" "gdown" )
    endif()
    find_program(GDOWN gdown ~/.local/bin)

  endif()

  if( GDOWN )
    find_program(UNZIP unzip)
    execute_process(COMMAND ${GDOWN} https://drive.google.com/uc?id=1rCsj_jpMyE0S0cokFJDyBSA0aPNiIHNb  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    execute_process(COMMAND ${UNZIP} "GAUSSIANFILES.zip" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
    execute_process(COMMAND rm "GAUSSIANFILES.zip" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  else()
    message(FATAL_ERROR "Found pip, but still unable to install or find gdown")
  endif()
endif()
