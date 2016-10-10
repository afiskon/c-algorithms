cmake_minimum_required(VERSION 2.8)

project(c-algorithms-crypto-tests)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

add_executable(test_md5 test_md5.c)
target_link_libraries(test_md5 CAlgorithms)
add_test(test_md5 "./test_md5")

add_executable(test_sha1 test_sha1.c)
target_link_libraries(test_sha1 CAlgorithms)
add_test(test_sha1 "./test_sha1")

add_executable(test_sha256 test_sha256.c)
target_link_libraries(test_sha256 CAlgorithms)
add_test(test_sha256 "./test_sha256")

add_executable(test_sha384 test_sha384.c)
target_link_libraries(test_sha384 CAlgorithms)
add_test(test_sha384 "./test_sha384")

add_executable(test_sha512 test_sha512.c)
target_link_libraries(test_sha512 CAlgorithms)
add_test(test_sha512 "./test_sha512")

add_executable(test_rijndael test_rijndael.c)
target_link_libraries(test_rijndael CAlgorithms)
add_test(test_rijndael "./test_rijndael")

