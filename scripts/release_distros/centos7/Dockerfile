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
WORKDIR /
COPY package_release.bash .
RUN chmod +x ./package_release.bash
RUN ./package_release.bash
#RUN bash -c "export VERSION=$(/QC_Tools/build/calc_J --version | grep "VERSION" | awk '{ print $4 }' | tr -d '\n')"
#RUN bash -c "export FORMAT=$(file /QC_Tools/build/calc_J | awk '{ print $6}' )"
#RUN bash -c "export FORMAT=${FORMAT%?}"
#RUN bash -c "export DISTRO=$(lsb_release -a | grep ID | awk '{print $3}')"
#RUN bash -c "export DISTRO_VERSION=$(lsb_release -a | grep Release | awk '{print $2}')"
#RUN bash -c "export tar_file=$(echo calc_J_${VERSION}_${DISTRO}${DISTRO_VERSION}_${FORMAT}.tar.gz)"
#RUN bash -c "echo 'tar file name '${tar_file}"
#WORKDIR /QC_Tools
#RUN tar -zcvf "${tar_file}" build/calc_J
#RUN mkdir TAR
#RUN mv *tar.gz TAR/
