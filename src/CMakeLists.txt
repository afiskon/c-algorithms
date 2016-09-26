cmake_minimum_required(VERSION 2.8)

project(c-algorithms)

add_library(CAlgorithms STATIC ilist.c rbtree.c htable.c crypto/md5.c)
