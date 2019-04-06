#!/bin/bash

directory=$(pwd)
distros="centos7 fedora28 ubuntu18 debian9"
for distro in $distros; do
  cd ${directory}/release_distros/${distro}
  pwd
  ls
  cp ${directory}/release_distros/package_release.bash ${directory}/release_distros/${distro}/
  docker build ${directory}/release_distros/${distro} -t catnip${distro}
  id=$(docker create catnip${distro})
  docker cp $id:TAR .
  rm ${directory}/release_distros/${distro}/package_release.bash
done

mkdir temp_release_zip
cd temp_release_zip
git clone https://github.com/JoshuaSBrown/QC_Tools.git
rm -rf QC_Tools/.git
rm -rf QC_Tools/.clang-format
rm -rf QC_Tools/travis
rm -rf QC_Tools/.travis.yml
rm -rf QC_Tools/docs
rm -rf QC_Tools/scripts
MAJOR_VERSION=$(cat QC_Tools/CMakeLists.txt | grep VERSION_MAJOR | grep -oP '\(\K[^\)]+' | awk '{print $2}')
MINOR_VERSION=$(cat QC_Tools/CMakeLists.txt | grep VERSION_MINOR | grep -oP '\(\K[^\)]+' | awk '{print $2}')
zip -r calc_J_${MAJOR_VERSION}.${MINOR_VERSION}_source.zip QC_Tools
tar -zcvf calc_J_${MAJOR_VERSION}.${MINOR_VERSION}_source.tar.gz QC_Tools
rm -rf QC_Tools

