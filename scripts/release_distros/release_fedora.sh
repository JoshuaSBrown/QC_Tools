#!/bin/bash

dnf install git -y
dnf install cmake -y
dnf install make -y
dnf install gcc-c++ -y
git clone https://github.com/JoshuaSBrown/QC_Tools.git
cd QC_TOOLS
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make


