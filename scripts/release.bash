#!/bin/bash

directory=$(pwd)
# CENTOS
cd ${directory}/release_distros/centos7
pwd
ls
cp ${directory}/release_distros/package_release.bash ${directory}/release_distros/centos7/
docker build ${directory}/release_distros/centos7 -t catnipcentos7
id=$(docker create catnipcentos7)
docker cp $id:TAR .
rm ${directory}/release_distros/centos7/package_release.bash

# FEDORA
#cd ${directory}/release_distros/fedora28
#pwd
#ls
#docker build ${directory}/release_distros/fedora28 -t catnipfedora28
#id=$(docker create catnipfedora28)
#docker cp $id:QC_Tools/TAR .
#
## UBUNTU
#cd ${directory}/release_distros/ubuntu18
#pwd
#ls
#docker build ${directory}/release_distros/ubuntu18 -t catnipubuntu18
#id=$(docker create catnipubuntu18)
#docker cp $id:QC_Tools/TAR .
