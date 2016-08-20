# c-algorithms

Usage:

```
mkdir build
cd build
cmake ..
make -j4
make test
```

How to make a coverage report:

```
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=`which gcc49` \
      -DCMAKE_CXX_COMPILER=`which g++49` \
      -DCMAKE_C_FLAGS="-O0 -g -fprofile-arcs -ftest-coverage" \
      -DCMAKE_EXE_LINKER_FLAGS="-lgcov" ..
make
make test
lcov --directory . --capture --output-file cov.info
mkdir cov-report
genhtml -o ./cov-report/ cov.info
chrome cov-report/index.html
```

See also: https://github.com/afiskon/c-algorithms-examples
