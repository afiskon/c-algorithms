cmake_minimum_required(VERSION 2.8)

project(c-algorithms)

include_directories(include)

add_library(CAlgorithms STATIC lib/ilist.c lib/rbtree.c)
