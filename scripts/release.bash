#!/bin/bash

directory=$(pwd)
distros="centos7 fedora28 ubuntu18 debian9"
for distro in $distros; do
  echo "Building ${distro}"
  cd ${directory}/release_distros/${distro}
  pwd
  ls
  cp ${directory}/release_distros/package_release.bash ${directory}/release_distros/${distro}/
  docker build ${directory}/release_distros/${distro} -t "catnip${distro}"
  id=$(docker create "catnip${distro}")
  docker cp $id:TAR .
  rm ${directory}/release_distros/${distro}/package_release.bash
done

cd ${directory}

binaries=$(find "$(pwd -P)" | grep tar.gz)
mkdir release_packages
cd release_packages
git clone https://github.com/JoshuaSBrown/QC_Tools.git
rm -rf QC_Tools/.git
rm -rf QC_Tools/.clang-format
rm -rf QC_Tools/.travis.yml
rm -rf QC_Tools/docs
MAJOR_VERSION=$(grep VERSION_MAJOR QC_Tools/CMakeLists.txt| grep -oP '\(\K[^\)]+' | awk '{print $2}')
MINOR_VERSION=$(grep VERSION_MINOR QC_Tools/CMakeLists.txt | grep -oP '\(\K[^\)]+' | awk '{print $2}')
zip -r calc_J_${MAJOR_VERSION}.${MINOR_VERSION}_source.zip QC_Tools
tar -zcvf calc_J_${MAJOR_VERSION}.${MINOR_VERSION}_source.tar.gz QC_Tools
rm -rf QC_Tools

for binary in ${binaries}
do
  echo "$binary"
  mv $binary .
done
