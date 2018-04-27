#!/bin/bash

if [ $# -eq 0 ]; then
    path="."
else
    path=$1
fi

fileOut="test_script.out"

echo "PATH "$path

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
count_fails=0

findJeff() {
  values=$data
  values=($values) 
  echo "Num array "${#values[@]}
  for (( i=0; i < ${#values[@]}; i++ ))
  do
    if [ "${values[$i]}" == "J_eff" ]
    then
      J_val=${values[$i+1]}
      break
    fi
  done  
}

# The following commands should work 
exec_command="${path}/build/calc_J -h"
${exec_command} > $fileOut
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi

exec_command="${path}/build/calc_J -p_P ${path}/GAUSSIANFILES/30/30_pair.pun  -p_1 ${path}/GAUSSIANFILES/30/ref.pun -p_2 ${path}/GAUSSIANFILES/30/30_2.pun"
data=$(${exec_command} )
if [ $? -eq 0 ]; then

  findJeff 
  if (( $(bc <<< "$J_val > 0.04973" ) )) && (( $(bc <<< "$J_val < 0.04975" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 0.0497444"
    echo "actual output $J_val"
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log  -l_1 ${path}/GAUSSIANFILES/30/ref.log -l_2 ${path}/GAUSSIANFILES/30/30_2.log"
data=$(${exec_command})
if [ $? -eq 0 ]; then
  findJeff 
  if (( $(bc <<< "$J_val > 0.04973" ) )) && (( $(bc <<< "$J_val < 0.04975" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 0.0497444"
    echo "actual output $J_val"
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log  -l_1 ${path}/GAUSSIANFILES/30/ref.log -l_2 ${path}/GAUSSIANFILES/30/30_2.log -p_1 ${path}/GAUSSIANFILES/30/ref.pun"
data=$(${exec_command})
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log -orb_ty_1 LUMO -l_1 ${path}/GAUSSIANFILES/30/ref.log -l_2 ${path}/GAUSSIANFILES/30/30_2.log"
data=$(${exec_command})
if [ $? -eq 0 ]; then
  echo "${green}[SUCCESS]${reset} ${exec_command}"
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -l_P ${path}/GAUSSIANFILES/30/30_pair.log -orb_ty_1 LUMO -l_1 ${path}/GAUSSIANFILES/30/ref.log -orb_ty_2 LUMO -l_2 ${path}/GAUSSIANFILES/30/30_2.log"
data=$(${exec_command})
if [ $? -eq 0 ]; then
  findJeff 
  if (( $(bc <<< "$J_val > 0.1270" ) )) && (( $(bc <<< "$J_val < 0.1272" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 0.127182"
    echo "actual output $J_val"
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -p_P ${path}/GAUSSIANFILES/CounterPoise/Dist_1Ang/1_pair.pun -p_1 ${path}/GAUSSIANFILES/CounterPoise/Dist_1Ang/ref.pun -p_2 ${path}/GAUSSIANFILES/CounterPoise/Dist_1Ang/1_B.pun -cp"
data=$(${exec_command})
if [ $? -eq 0 ]; then
  findJeff 
  if (( $(bc <<< "$J_val > 4.505" ) )) && (( $(bc <<< "$J_val < 4.506" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 4.50589"
    echo "actual output $J_val"
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -cp -p_P ${path}/GAUSSIANFILES/CounterPoise/Dist_1Ang/1_pair.pun -p_1 ${path}/GAUSSIANFILES/CounterPoise/Dist_1Ang/ref.pun -p_2 ${path}/GAUSSIANFILES/CounterPoise/Dist_1Ang/1_B.pun "
data=$(${exec_command})
if [ $? -eq 0 ]; then
  findJeff 
  if (( $(bc <<< "$J_val > 4.505" ) )) && (( $(bc <<< "$J_val < 4.506" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 4.50589"
    echo "actual output $J_val"
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut


# The following commands should not work 

exit $count_fails
