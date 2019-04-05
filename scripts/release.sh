!#/bin/bash

docker exec fedora:28 release_distros/release_fedora.sh
docker exec ubuntu:bionic release_distros/release_ubuntu.sh
docker exec centos:7 release_distros/release_centos.sh
