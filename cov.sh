#!/bin/sh

set -e

cmake -DCMAKE_C_COMPILER=`which gcc` \
      -DCMAKE_CXX_COMPILER=`which g++` \
      -DCMAKE_C_FLAGS="-O0 -g -fprofile-arcs -ftest-coverage" \
      -DCMAKE_EXE_LINKER_FLAGS="-lgcov" ..
make -j4
make test
lcov --directory . --capture --output-file cov.info
mkdir cov-report
genhtml -o ./cov-report/ cov.info
x-www-browser cov-report/index.html
