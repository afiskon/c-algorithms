cmake_minimum_required(VERSION 2.8)

project(c-algorithms-struct-tests)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

add_executable(test_slist test_slist.c)
target_link_libraries(test_slist CAlgorithms)

add_executable(test_dlist test_dlist.c)
target_link_libraries(test_dlist CAlgorithms)

add_executable(test_htable test_htable.c)
target_link_libraries(test_htable CAlgorithms)

add_executable(test_rbtree test_rbtree.c)
target_link_libraries(test_rbtree CAlgorithms)

add_test(test_slist "./test_slist")
add_test(test_dlist "./test_dlist")
add_test(test_htable "./test_htable")
add_test(test_rbtree "./test_rbtree")
