cmake_minimum_required(VERSION 2.8)

project(c-algorithms)

add_library(CAlgorithms STATIC
	struct/ilist.c
	struct/rbtree.c
	struct/htable.c

	crypto/md5.c
	crypto/sha1.c
	crypto/sha2.c
	crypto/rijndael.c

	common/utils.c
)
