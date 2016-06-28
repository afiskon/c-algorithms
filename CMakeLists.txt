cmake_minimum_required(VERSION 2.8)

project(c-algorithms)

include_directories(include)

add_library(CAlgorithms STATIC lib/ilist.c lib/rbtree.c)

add_executable(slist_example examples/slist_example.c)
target_link_libraries(slist_example CAlgorithms)
