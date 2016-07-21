/*
	htable.c
	(c) Alexandr A Alexeev 2011-2016 | http://eax.me/
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "htable.h"

#define MIN_HTABLE_SIZE (1 << 3)

static void
_htable_resize(HTable tbl)
{
	HTableNode* new_items;
	HTableNode curr_item, prev_item, temp_item;
	size_t item_idx, new_size = tbl->size;
	uint32_t new_mask;

	if(tbl->nitems < MIN_HTABLE_SIZE)
		return;

	if(tbl->nitems > tbl->size)
	{
		new_size = tbl->size * 2;
		new_mask = new_size - 1;
		new_items = tbl->allocfunc(sizeof(HTableNode) * new_size, tbl->arg);
		if(new_items == NULL)
			return;

		/* just copy first part of the `items` table */
		memcpy(new_items, tbl->items, tbl->size * sizeof(HTableNode));
		/* second part fill with zeros for now */
		memset(&new_items[tbl->size], 0, tbl->size * sizeof(HTableNode));

		for(item_idx = 0; item_idx < tbl->size; item_idx++)
		{
			prev_item = NULL;
			curr_item = new_items[item_idx];
			while(curr_item != NULL)
			{
				if((curr_item->hash & tbl->mask) != (curr_item->hash & new_mask))
				{
					if(prev_item == NULL)
						new_items[item_idx] = curr_item->next;
					else
						prev_item->next = curr_item->next;

					temp_item = curr_item->next;
					curr_item->next = new_items[curr_item->hash & new_mask];
					new_items[curr_item->hash & new_mask] = curr_item;
					curr_item = temp_item;
				}
				else
				{
					prev_item = curr_item;
					curr_item = curr_item->next;
				}
			} /* while */
		} /* for */
	}
	else if(tbl->nitems <= (tbl->size / 2))
	{
		new_size = tbl->size / 2;
		new_mask = new_size - 1;
		new_items = tbl->allocfunc(sizeof(HTableNode) * new_size, tbl->arg);
		if(new_items == NULL)
			return;

		memcpy(new_items, tbl->items, new_size * sizeof(HTableNode));
		for(item_idx = new_size; item_idx < tbl->size; item_idx++)
		{
			if(tbl->items[item_idx] == NULL)
				continue;

			if(new_items[item_idx - new_size] == NULL)
				new_items[item_idx - new_size] = tbl->items[item_idx];
			else
			{
				curr_item = new_items[item_idx - new_size];
				while(curr_item->next != NULL)
					curr_item = curr_item->next;
				curr_item->next = tbl->items[item_idx];
			}
		}
	}

	if(new_size != tbl->size)
	{
		tbl->freefunc(tbl->items, tbl->arg);
		tbl->items = new_items;
		tbl->size = new_size;
		tbl->mask = new_mask;
	}
}

/* http://en.wikipedia.org/wiki/Jenkins_hash_function */
uint32_t
htable_default_hash(const char *key, const size_t key_len)
{
	uint32_t hash, i;

	for(hash = i = 0; i < key_len; i++)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

void
htable_create(
	HTable tbl,
	size_t node_size,
	htable_hash_func hfunc,
	htable_keyeq_func eqfunc,
	htable_alloc_func allocfunc,
	htable_free_func freefunc,
	htable_before_node_free_func bnffunc,
	void* arg)
{
	tbl->nitems = 0;
	tbl->size = MIN_HTABLE_SIZE;
	tbl->mask = tbl->size - 1;
	tbl->node_size = node_size;
	tbl->hfunc = hfunc;
	tbl->eqfunc = eqfunc;
	tbl->allocfunc = allocfunc;
	tbl->freefunc = freefunc;
	tbl->bnffunc = bnffunc;
	tbl->arg = arg;

	tbl->items = (HTableNode*)tbl->allocfunc(sizeof(HTableNode) * tbl->size, tbl->arg);
	if(tbl->items == NULL)
		return;
	memset(tbl->items, 0, sizeof(void*) * tbl->size);
}

void
htable_free_items(HTable tbl)
{
	size_t item_idx;
	HTableNode curr_item, next_item;

	for(item_idx = 0; item_idx < tbl->size; item_idx++)
	{
		curr_item = tbl->items[item_idx];
		while(curr_item)
		{
			next_item = curr_item->next;
			tbl->bnffunc(curr_item, tbl->arg);
			tbl->freefunc(curr_item, tbl->arg);
			curr_item = next_item;
		}
	}

	tbl->freefunc(tbl->items, tbl->arg);
}

HTableNode
htable_get(HTable tbl, HTableNode query)
{
	uint32_t hash = tbl->hfunc(query, tbl->arg);
	HTableNode curr_item = tbl->items[hash & tbl->mask];

	while(curr_item)
	{
		if(tbl->eqfunc(curr_item, query, tbl->arg))
			return curr_item;
		curr_item = curr_item->next;
	}

	return NULL;
}

void
htable_put(HTable tbl, HTableNode node, bool* isNewNode)
{
	uint32_t hash = tbl->hfunc(node, tbl->arg);
	HTableNode item = tbl->items[hash & tbl->mask];

	/* if such a key is already used, update node */
	while(item)
	{
		if(tbl->eqfunc(item, node, tbl->arg))
		{
			HTableNode savedNext = item->next;
			tbl->bnffunc(item, tbl->arg);
			memcpy(item, node, tbl->node_size);
			item->next = savedNext;
			item->hash = hash;
			*isNewNode = false;
			return;
		}
		item = item->next;
	}
 
	/* key is not used yet, add new node */
	item = tbl->allocfunc(tbl->node_size, tbl->arg);
	if(item == NULL)
		return;
	
	memcpy(item, node, tbl->node_size);

	item->hash = hash;
	item->next = tbl->items[hash & tbl->mask];
	tbl->items[hash & tbl->mask] = item;
	tbl->nitems++;

	_htable_resize(tbl);
	*isNewNode = true;
}

bool
htable_delete(HTable tbl, HTableNode query)
{
	uint32_t hash = tbl->hfunc(query, tbl->arg);
	HTableNode item = tbl->items[hash & tbl->mask];
	HTableNode prev = NULL;

	while(item)
	{
		if(tbl->eqfunc(item, query, tbl->arg))
		{
			if(prev)
				prev->next = item->next;
			else
				tbl->items[hash & tbl->mask] = item->next;

			tbl->bnffunc(item, tbl->arg);
			tbl->freefunc(item, tbl->arg);
			tbl->nitems--;
			_htable_resize(tbl);
			return true;
		}
		prev = item;
		item = item->next;
	}

	return false;
}

