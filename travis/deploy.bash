#!/bin/bash

# Make codecoverage build script executable
chmod +x scripts/build.bash
mkdir build
cd build
cmake -DENABLE_INTEGRATION_TESTS=ON -DENABLE_TESTS=ON -DCODE_COVERAGE=ON ../
make 
ctest --verbose
lcov --capture --directory . --output-file coverage.info
lcov --list coverage.info
bash <(curl -s https://codecov.io/bash) -t "e0045700-7ee0-41a6-a907-70f6a530f431"
