#!/bin/bash

directory=$(pwd)
distros="centos7 fedora28 ubuntu18"
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
