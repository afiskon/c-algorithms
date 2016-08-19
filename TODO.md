# TODO

* Refactor rbtree
	* Never override existing RB-tree nodes (see rbtree.c:593)
	* After removing old interface: Make corresponding changes to the blog post
	* Make sure that changing walk direction from left-right to right-left works well
	* Implement search by part of the key: find\_by\_partial\_key\_march(rb, rb\_comparator, void\* arg)
* Add LRU/LFU cache examples to https://github.com/afiskon/c-algorithms-examples
* Add array-bases queue implementation (fifo)
* Add in-memory B-tree implementation
* Add scapegoat tree implementation https://github.com/delamonpansie/octopus/blob/master/third_party/sptree.h#L331
* Add benchmarks. Figure out what rbtree iteration is fastest (on integer keys)
* Make code _at_ _least_ 97% test covered
* Check using Clang Static Analyzer and CppCheck
* Check using MemorySanitizer and probably other sanitizers
