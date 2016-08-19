# TODO

* Refactor rbtree
	* Never override existing RB-tree nodes (see rbtree.c:593)
	* Get rid of rb\_begin\_iterate / rb\_iterate, keep only new interface
	* Write property tests
		* (Only left-right and right-left walk) Nodes are returned in sorted (reverse-sorted) order
		* If all nodes are unique, every node is returned only once
	* Make corresponding changes to the blog post
	* Join LeftRightWalk and RightLeftWalk to the single structure
	* Make sure that changing walk direction from left-right to right-left works well
	* Implement search by part of the key: find\_by\_partial\_key\_march(rb, rb\_comparator, void\* arg)
* Add array-bases queue implementation (fifo)
* Add in-memory B-tree implementation
* Add scapegoat tree implementation https://github.com/delamonpansie/octopus/blob/master/third_party/sptree.h#L331
* Add benchmarks. Figure out what rbtree iteration is fastest (on integer keys)
* Check using Clang Static Analyzer and CppCheck
* Check using MemorySanitizer and probably other sanitizers
