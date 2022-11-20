#!/bin/bash

#
# Create build directory
#

cmake -E make_directory build
cd build

#
# Run CMake and abort script on failure
#
cmake ..

if [ $? -ne 0 ]; then
    exit 0
fi

#
# Run make and abort on failure
#
make

if [ $? -ne 0 ]; then
    exit 0
fi

#
# Run tests
#

bin/unittest
 
