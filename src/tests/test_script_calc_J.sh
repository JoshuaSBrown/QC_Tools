#!/bin/bash

if [ $# -eq 0 ]; then
    path="./../"
else
    path=$1
fi

fileOut="test_script.out"

# Ensure that the bc calculator is installed
Is_bc_installed=$(command -v bc)
if [ -z "$Is_bc_installed" ]
then
  echo "To run the test script you must have bc installed!"
  exit 1
fi

echo "PATH "$path

red=$(tput setaf 1)
green=$(tput setaf 2)
reset=$(tput sgr0)
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
  count_fails=$((count_fails+1))
fi

exec_command="${path}/build/calc_J -p_P ${path}/GAUSSIANFILES/30/30_pair.pun  -p_1 ${path}/GAUSSIANFILES/30/ref.pun -p_2 ${path}/GAUSSIANFILES/30/30_2.pun"
data=$(${exec_command} )
if [ $? -eq 0 ]; then

  findJeff 
  if (( $( bc <<< "$J_val > 0.04973" ) )) && (( $( bc <<< "$J_val < 0.04975" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 0.0497444"
    echo "actual output $J_val"
    echo $( bc <<< "$J_val > 0.04973" )
    echo $( bc <<< "$J_val < 0.04975" )
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -p_P ${path}/GAUSSIANFILES/P_test/AB.pun  -p_1 ${path}/GAUSSIANFILES/P_test/A.pun -p_2 ${path}/GAUSSIANFILES/P_test/B.pun"
data=$(${exec_command} )
if [ $? -eq 0 ]; then

  findJeff 
  if (( $( bc <<< "$J_val > 0.02617" ) )) && (( $( bc <<< "$J_val < 0.02619" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 0.0261851"
    echo "actual output $J_val"
    echo $( bc <<< "$J_val > 0.02617" )
    echo $( bc <<< "$J_val < 0.02619" )
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$(($count_fails+1))
fi
echo $data >> $fileOut

exec_command="${path}/build/calc_J -p_P ${path}/GAUSSIANFILES/Square_Matrix_test/dimer.pun  -p_1 ${path}/GAUSSIANFILES/Square_Matrix_test/monomer1.pun -p_2 ${path}/GAUSSIANFILES/Square_Matrix_test/monomer2.pun"
data=$(${exec_command} )
if [ $? -eq 0 ]; then

  findJeff 
  if (( $( bc <<< "$J_val > 0.001245" ) )) && (( $( bc <<< "$J_val < 0.001247" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output 0.00124623"
    echo "actual output $J_val"
    echo $( bc <<< "$J_val > 0.001245" )
    echo $( bc <<< "$J_val < 0.001247" )
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

exec_command="${path}/build/calc_J -p_P ${path}/GAUSSIANFILES/CuBr2_Py/CuBr2-Py.pun -p_1 ${path}/GAUSSIANFILES/CuBr2_Py/CuBr2.pun -p_2 ${path}/GAUSSIANFILES/CuBr2_Py/Py.pun"
data=$(${exec_command})
if [ $? -eq 0 ]; then
  findJeff 
  if (( $(bc <<< "$J_val > -0.0055" ) )) && (( $(bc <<< "$J_val < -0.0053" ) ))
  then
    echo "${green}[SUCCESS]${reset} ${exec_command}" 
  else
    echo "${red}[FAILURE]${reset} ${exec_command} expected output -0.00542741"
    echo "actual output $J_val"
    count_fails=$(($count_fails+1))
  fi
else
  echo "${red}[FAILURE]${reset} ${exec_command}"
  count_fails=$((count_fails+1))
fi
echo $data >> $fileOut


# The following commands should not work 

exit $count_fails
