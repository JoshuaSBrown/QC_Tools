#!/bin/bash

if [ $# -eq 0 ]; then
    path="."
else
    path=$1
fi

echo "PATH "$path

red=$(tput setaf 1)
green=$(tput setaf 2)
reset=$(tput sgr0)
count_fails=0

${path}/test_io -h > test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${path}/test_io -h"
else
  echo "${red}[FAILURE]${reset} ${path}/test_io -h"
  count_fails=$(($count_fails+1))
fi

${path}/test_io -cite > test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${path}/test_io -cite"
else
  echo "${red}[FAILURE]${reset} ${path}/test_io -cite"
  count_fails=$(($count_fails+1))
fi

${path}/test_io -p_P ${path}/../../GAUSSIANFILES/30/30_pair.pun  -p_1 ${path}/../../GAUSSIANFILES/30/ref.pun -p_2 ${path}/../../GAUSSIANFILES/30/30_2.pun >> test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${path}/test_io -p_P ${path}/../../GAUSSIANFILES/30/30_pair.pun  -p_1 ${path}/../../GAUSSIANFILES/30/ref.pun     -p_2 ${path}/../../GAUSSIANFILES/30/30_2.pun"
else
  echo "${red}[FAILURE]${reset} ${path}/test_io -p_P ${path}/../../GAUSSIANFILES/30/30_pair.pun  -p_1 ${path}/../../GAUSSIANFILES/30/ref.pun     -p_2 ${path}/../../GAUSSIANFILES/30/30_2.pun"
  count_fails=$((count_fails+1))
fi

${path}/test_io -l_P ${path}/../../GAUSSIANFILES/30/30_pair.log  -l_1 ${path}/../../GAUSSIANFILES/30/ref.log -l_2 ${path}/../../GAUSSIANFILES/30/30_2.log >> test_script.out
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${path}/test_io -l_P ${path}/../../GAUSSIANFILES/30/30_pair.log  -l_1 ${path}/../../GAUSSIANFILES/30/ref.log -l_2 ${path}/../../GAUSSIANFILES/30/30_2.log"
else
  echo "${red}[FAILURE]${reset} ${path}/test_io -l_P ${path}/../../GAUSSIANFILES/30/30_pair.log  -l_1 ${path}/../../GAUSSIANFILES/30/ref.log -l_2 ${path}/../../GAUSSIANFILES/30/30_2.log"
  count_fails=$((count_fails+1))
fi

exit $count_fails
