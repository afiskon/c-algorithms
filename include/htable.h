/*
  htable.h
  (c) Alexandr A Alexeev 2011-2016 | http://eax.me/
*/

#include <stdint.h>
#include <stdbool.h>

typedef struct HTableNodeData
{
	struct HTableNodeData* next;	/* next node in a chain, if any */
	uint32_t hash;					/* hash function value */
} HTableNodeData;

typedef HTableNodeData* HTableNode;

typedef bool (*htable_keyeq_func) (const HTableNode a, const HTableNode b, void *arg);
typedef uint32_t (*htable_hash_func) (const HTableNode a, void *arg);
typedef void* (*htable_alloc_func) (size_t size, void *arg);
typedef void (*htable_free_func) (void* mem, void *arg);
typedef void (*htable_before_node_free_func) (HTableNode node, void *arg);

/* Hash table representation */
typedef struct {
	HTableNode* items;	/* table items */
	size_t nitems; 		/* how many items are stored in hash table */
	uint32_t mask; 		/* mask, aplied to hash function */
	size_t size; 		/* current hash table size */

	/* user-specified arguments */

	size_t node_size;
	htable_hash_func hfunc;
	htable_keyeq_func eqfunc;
	htable_alloc_func allocfunc;
	htable_free_func freefunc;
	htable_before_node_free_func bnffunc;
	void* arg;
} HTableData;

typedef HTableData* HTable;

extern uint32_t htable_default_hash(const char *key, const size_t key_len);
extern void htable_create(
		HTable tbl,
		size_t node_size,
		htable_hash_func hfunc,
		htable_keyeq_func eqfunc,
		htable_alloc_func allocfunc,
		htable_free_func freefunc,
		htable_before_node_free_func bnffunc,
		void* arg
	);
extern void htable_free_items(HTable tbl);
extern HTableNode htable_get(HTable tbl, HTableNode query);
extern void htable_put(HTable tbl, HTableNode node, bool* isNewNode);
extern bool htable_delete(HTable tbl, HTableNode query);
