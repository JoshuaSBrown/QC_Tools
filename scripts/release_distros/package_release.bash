#!/bin/bash

VERSION=$(/QC_Tools/build/calc_J --version | awk '{ print $4 }' |tr -d '\n')
FORMAT=$(file /QC_Tools/build/calc_J | awk '{ print $6}' )
FORMAT=${FORMAT%?}      
DISTRO=$(lsb_release -a | grep ID | awk '{print $3}')      
DISTRO_VERSION=$(lsb_release -a | grep Release | awk '{print $2}')
tar_file=$(echo calc_J_${VERSION}_${DISTRO}_${DISTRO_VERSION}_${FORMAT}.tar.gz)
echo "tar file name "${tar_file}
ls /QC_Tools/build
tar -zcvf "${tar_file}" QC_Tools/build/calc_J                                        
mkdir TAR                                                                   
mv *tar.gz TAR/    
