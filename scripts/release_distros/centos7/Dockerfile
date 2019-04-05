FROM centos:7

RUN yum install make cmake gcc-c++ git file redhat-lsb-core -y                      
RUN git clone https://github.com/JoshuaSBrown/QC_Tools.git                          
RUN mkdir -p QC_Tools/build_test
RUN mkdir -p QC_Tools/build
# Build with tests and then test
WORKDIR /QC_Tools/build_test
RUN cmake -DENABLE_TESTS=ON -DENABLE_INTEGRATION_TESTS=ON ../ 
RUN make
RUN make test

# Build optimized release version
WORKDIR /QC_Tools/build
RUN cmake -DCMAKE_BUILD_TYPE=Release ../
RUN make
# VERSION of calc_J                                                             
# 64 or 32 bit architecture                                                     
# Remove the comma from version 
RUN export VERSION=$(/QC_Tools/build/calc_J --version | grep "VERSION" | awk '{ print $4 }' | tr -d '\n')
RUN export FORMAT=$(file /QC_Tools/build/calc_J | awk '{ print $6}' )
RUN export FORMAT=${FORMAT%?}
RUN export DISTRO=$(lsb_release -a | grep ID | awk '{print $3}')
RUN export DISTRO_VERSION=$(lsb_release -a | grep Release | awk '{print $2}')
RUN export tar_file=$(echo "calc_J_${VERSION}_${DISTRO}${DISTRO_VERSION}_${FORMAT}.tar.gz")
RUN echo "tar file name "${tar_file}
WORKDIR /QC_Tools
RUN tar -zcvf "${tar_file}" build/calc_J
RUN mkdir TAR
RUN mv *tar.gz TAR/
