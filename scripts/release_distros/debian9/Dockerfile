FROM debian:stretch

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN apt-get update && apt-get install -y python curl bc zip unzip apt-utils ca-certificates make cmake gcc g++ git file lsb-release && apt-get clean && rm -rf /var/lib/apt/lists/*
RUN curl "https://bootstrap.pypa.io/get-pip.py" -o "get-pip.py"                 
RUN chmod +x get-pip.py                                                         
RUN ./get-pip.py                                                                
RUN pip install gdown  
RUN git clone https://github.com/JoshuaSBrown/QC_Tools.git                          
RUN mkdir -p QC_Tools/build_test
RUN mkdir -p QC_Tools/build

# Build with tests and then test
WORKDIR /QC_Tools/build
RUN cmake -DENABLE_TESTS=ON -DENABLE_INTEGRATION_TESTS=ON ../
RUN make
RUN make ARGS="-V" test
# Build optimized release version
RUN rm -rf *
RUN cmake -DCMAKE_BUILD_TYPE=Release ../
RUN make
# VERSION of calc_J                                                             
# 64 or 32 bit architecture                                                     
# Remove the comma from version 
WORKDIR /
COPY package_release.bash .
RUN chmod +x ./package_release.bash
RUN ./package_release.bash
