#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "htable.h"

#define N 100

typedef struct
{
	HTableNode node;
	char expression[128];
	int value;
} ExpressionTableNodeData;

typedef ExpressionTableNodeData *ExpressionTableNode;

bool keyeq_func(const HTableNode* a_, const HTableNode* b_, void *arg)
{
	ExpressionTableNode a = (ExpressionTableNode)a_;
	ExpressionTableNode b = (ExpressionTableNode)b_;
	return (strcmp(a->expression, b->expression) == 0);
}

uint32_t hash_func(const HTableNode* a_, void *arg)
{
	ExpressionTableNode a = (ExpressionTableNode)a_;
	return htable_default_hash(a->expression, strlen(a->expression));
}

void* alloc_func(size_t size, void *arg)
{
	return malloc(size);
}

void free_func(void* mem, void *arg)
{
	free(mem);
}

void
run_test(HTable* htable)
{
	int i, j;

	/* fill table */
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= N; j++)
		{
			bool isNewNode;
			ExpressionTableNodeData new_node_data;
			sprintf(new_node_data.expression, "%d + %d", i, j);
			new_node_data.value = (i + j);
			htable_insert(htable, (HTableNode*)&new_node_data, &isNewNode);
			assert(isNewNode);
		}
	}

	assert(htable_nitems(htable) == (N*N));

	/* check hash table is filled right */
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= N; j++)
		{
			ExpressionTableNode found_node;
			ExpressionTableNodeData query;
			sprintf(query.expression, "%d + %d", i, j);
			found_node = (ExpressionTableNode)htable_find(htable, (HTableNode*)&query);
			assert(found_node != NULL);
			assert(found_node->value == (i + j));
		}
	}

	/* try to delete a non-existing node */
	{
		bool result;
		ExpressionTableNodeData query;
		sprintf(query.expression, "ololo trololo");
		result = htable_delete(htable, (HTableNode*)&query);
		assert(result == false);
	}

	/* clean table */
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= N; j++)
		{
			bool result;
			ExpressionTableNodeData query;
			sprintf(query.expression, "%d + %d", i, j);
			result = htable_delete(htable, (HTableNode*)&query);
			assert(result == true);
		}
	}

	assert(htable_nitems(htable) == 0);
}

int main()
{
	int i;
	HTable htable_data;

	htable_create(
			&htable_data,
			sizeof(ExpressionTableNodeData),
			hash_func,
			keyeq_func,
			alloc_func,
			free_func,
			NULL,
			NULL
		);

	for(i = 0; i < 100; i++)
		run_test(&htable_data);

	printf("OK!\n");

	return 0;
}
