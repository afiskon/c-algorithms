# TODO

* Refactor rbtree
    * Get rid of rb_begin_iterate / rb_iterate, keep only new interface
    * Write property tests
		* (Only left-right and right-left walk) Nodes are returned in sorted (reverse-sorted) order
		* If all nodes are unique, every node is returned only once
    * Make corresponding changes to the blog post
* Add hash table implementation based on https://github.com/afiskon/c-hash-tables + write tests
* Add benchmarks. Figure out what rbtree iteration is fastest (on integer keys)
