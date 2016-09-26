cmake_minimum_required(VERSION 2.8)

project(c-algorithms-crypto-tests)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

add_executable(test_md5 test_md5.c)
target_link_libraries(test_md5 CAlgorithms)

add_test(test_md5 "./test_md5")
