#!/bin/bash

if [ $# -eq 0 ]; then
    path="."
else
    path=$1
fi

echo "PATH "$path

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
count_fails=0

# The following commands should work 
exec_command="${path}/build/calc_J -h"
${exec_command} > test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi

exec_command="${path}/build/calc_J -p_P ${path}/GAUSSIANFILES/30/30_pair.pun  -p_1 ${path}/GAUSSIANFILES/30/ref.pun -p_2 ${path}/GAUSSIANFILES/30/30_2.pun"
${exec_command} >> test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}" 
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log  -l_1 ${path}/GAUSSIANFILES/30/ref.log -l_2 ${path}/GAUSSIANFILES/30/30_2.log"
${exec_command} >> test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log  -l_1 ${path}/GAUSSIANFILES/30/ref.log -l_2 ${path}/GAUSSIANFILES/30/30_2.log -p_1 ${path}/GAUSSIANFILES/30/ref.pun"
${exec_command} >> test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log -orb_ty_1 LUMO -l_1 ${path}/GAUSSIANFILES/30/ref.log -l_2 ${path}/GAUSSIANFILES/30/30_2.log"
${exec_command} >> test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log -orb_ty_1 LUMO -l_1 ${path}/GAUSSIANFILES/30/ref.log -orb_ty_2 LUMO -l_2 ${path}/GAUSSIANFILES/30/30_2.log"
${exec_command} >> test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi


# The following commands should not work 

exit $count_fails
