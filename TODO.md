# TODO

* Refactor rbtree
    * Get rid of rb_begin_iterate / rb_iterate, keep only new interface
    * Write property tests on left-rigth and right-left walks
    * Make corresponding changes to the blog post
* Add hash table implementation based on https://github.com/afiskon/c-hash-tables + write tests
* Add benchmarks. Figure out what rbtree iteration is fastest (on integer keys)
