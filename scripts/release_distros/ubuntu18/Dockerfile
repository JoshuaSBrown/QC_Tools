FROM ubuntu:bionic

RUN apt-get update && apt-get install make cmake gcc git file lsb-core -y
RUN git clone https://github.com/JoshuaSBrown/QC_Tools.git                          
RUN mkdir -p QC_Tools/build_test
# Build with tests and then test
RUN cd QC_Tools/build_test && cmake -DENABLE_TESTS=ON -DENABLE_INTEGRATION_TESTS=ON ../ && make && make test
RUN mkdir -p QC_Tools/build
# Build optimized release version
RUN cd QC_Tools/build && cmake -DCMAKE_BUILD_TYPE=Release ../ && make
# VERSION of calc_J                                                             
# 64 or 32 bit architecture                                                     
# Remove the comma from version 
RUN VERSION=$(/QC_Tools/build/calc_J --version | awk '{ print $4 }' |tr -d '\n') && FORMAT=$(file /QC_Tools/build/calc_J | awk '{ print $6}' ) && FORMAT=${FORMAT%?} && DISTRO=$(lsb_release -a | grep ID | awk '{print $3}') && DISTRO_VERSION=$(lsb_release -a | grep Release | awk '{print $2}')&& tar_file=$(echo "calc_J_${VERSION}_${DISTRO}${DISTRO_VERSION}_${FORMAT}.tar.gz") && echo "tar file name "${tar_file} && cd QC_Tools && tar -zcvf "${tar_file}" build/calc_J && mkdir TAR && mv *tar.gz TAR/
