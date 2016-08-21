#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include "rbtree.h"

/* #define INIFNITE_TEST */
#define UNIQUE_NUMBERS_ARRAY_SIZE 7777

typedef struct
{
	RBNode node;
	char data[128];
} TreeItemData;

typedef TreeItemData *TreeItem;

static int
tree_comparator(const RBNode* a, const RBNode* b, void* arg)
{
	return strcmp(((const TreeItem)a)->data, ((const TreeItem)b)->data);
}

static void
tree_combiner(RBNode* existing, const RBNode* newdata, void* arg)
{
	/* do nothing */
}

static RBNode*
tree_allocfunc(void* arg)
{
	return malloc(sizeof(TreeItemData));
}

static void
tree_freefunc(RBNode* node, void* arg)
{
	free(node);
}

static void
tree_sprintfunc(RBNode* node, char* buff)
{
	strcpy(buff, ((TreeItemData*)node)->data);
}

static void
general_test()
{
	RBTree tree;
	bool isNew;
	TreeItemData item;
	TreeItem tmp;
	RBTreeLeftRightWalk lrw;
	char temp_buff[512];

	rb_create(&tree, sizeof(TreeItemData), tree_comparator, tree_combiner, tree_allocfunc, tree_freefunc, NULL);

	assert(rb_leftmost(&tree) == NULL);

	strcpy(item.data, "Item A");
	rb_insert(&tree, (RBNode*)&item, &isNew);
	assert(isNew);
	assert(rb_leftmost(&tree) != NULL);

	strcpy(item.data, "Item B");
	rb_insert(&tree, (RBNode*)&item, &isNew);
	assert(isNew);

	strcpy(item.data, "Item C");
	rb_insert(&tree, (RBNode*)&item, &isNew);
	assert(isNew);

	temp_buff[0] = 0;
	rb_begin_left_right_walk(&tree, &lrw);
	while( (tmp = (TreeItem)rb_left_right_walk(&lrw)) )
	{
		strcat(temp_buff, tmp->data);
		strcat(temp_buff, ",");
	}
	assert(strcmp(temp_buff, "Item A,Item B,Item C,") == 0);

	strcpy(item.data, "No Such Item");
	tmp = (TreeItem)rb_find(&tree, (RBNode*)&item);
	assert(tmp == NULL);

	strcpy(item.data, "Item A");
	tmp = (TreeItem)rb_find(&tree, (RBNode*)&item);
	assert(tmp != NULL);

	strcpy(item.data, "Item C");
	tmp = (TreeItem)rb_find(&tree, (RBNode*)&item);
	assert(tmp != NULL);

	strcpy(item.data, "Item B");
	tmp = (TreeItem)rb_find(&tree, (RBNode*)&item);
	assert(tmp != NULL);

	rb_delete(&tree, (RBNode*)tmp);

	/* rb_begin_iterate + rb_iterate doesn't work here! */
	temp_buff[0] = 0;
	while( (tmp = (TreeItem)rb_leftmost(&tree)) )
	{
		strcat(temp_buff, tmp->data);
		strcat(temp_buff, ",");
		rb_delete(&tree, (RBNode*)tmp);
	}

	assert(strcmp(temp_buff, "Item A,Item C,") == 0);
	assert(rb_leftmost(&tree) == NULL);
}

/*
 * Make sure left right walk enumirates all items, and any item is returned
 * only once. Also make sure left right walk returns item in sorted order.
 */
static void
left_right_walk_test()
{
	RBTree tree;
	bool isNew;
	TreeItemData item;
	TreeItem tmp;
	RBTreeLeftRightWalk lrw;
	bool number_returned[UNIQUE_NUMBERS_ARRAY_SIZE];
	int unique_numbers[UNIQUE_NUMBERS_ARRAY_SIZE];
	int i, j, temp, nitems, last_item;
	int array_size = UNIQUE_NUMBERS_ARRAY_SIZE;

#ifdef INIFNITE_TEST
	array_size = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
#endif

	for(i = 0; i < array_size; i++)
	{
		unique_numbers[i] = i;
		number_returned[i] = false;
	}

	/* shuffle numbers */
	for(i = 0; i < array_size; i++)
	{
		j = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
		assert((j >= 0) && (j < array_size));
		temp = unique_numbers[i];
		unique_numbers[i] = unique_numbers[j];
		unique_numbers[j] = temp;
	}

	/* create an empty tree */
	rb_create(&tree, sizeof(TreeItemData), tree_comparator, tree_combiner, tree_allocfunc, tree_freefunc, NULL);

	rb_begin_left_right_walk(&tree, &lrw);
	assert(rb_left_right_walk(&lrw) == NULL);

	/* fill a tree */
	for(i = 0; i < array_size; i++)
	{
		sprintf(item.data, "Item %08x", unique_numbers[i]);
		rb_insert(&tree, (RBNode*)&item, &isNew);
		assert(isNew);
	}

	/* check enumiration */
	nitems = 0;
	last_item = -1;
	rb_begin_left_right_walk(&tree, &lrw);
	while( (tmp = (TreeItem)rb_left_right_walk(&lrw)) )
	{
		sscanf(tmp->data, "Item %08x", &i);

		assert(number_returned[i] == false);
		number_returned[i] = true;

		assert(last_item < i);
		last_item = i;

		nitems++;
	}
	assert(nitems == array_size);
	assert(last_item == (array_size-1));

	/* free memory */
	nitems = 0;
	while( (tmp = (TreeItem)rb_leftmost(&tree)) )
	{
		rb_delete(&tree, (RBNode*)tmp);
		nitems++;
	}
	assert(nitems == array_size);
}

/*
 * Make sure right left walk enumirates all items, and any item is returned
 * only once. Also make sure right left walk returns item in revere sorted
 * order.
 */
static void
right_left_walk_test()
{
	RBTree tree;
	bool isNew;
	TreeItemData item;
	TreeItem tmp;
	RBTreeRightLeftWalk rlw;
	bool number_returned[UNIQUE_NUMBERS_ARRAY_SIZE];
	int unique_numbers[UNIQUE_NUMBERS_ARRAY_SIZE];
	int i, j, temp, nitems, last_item;
	int array_size = UNIQUE_NUMBERS_ARRAY_SIZE;

#ifdef INIFNITE_TEST
	array_size = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
#endif

	for(i = 0; i < array_size; i++)
	{
		unique_numbers[i] = i;
		number_returned[i] = false;
	}

	/* shuffle numbers */
	for(i = 0; i < array_size; i++)
	{
		j = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
		assert((j >= 0) && (j < array_size));
		temp = unique_numbers[i];
		unique_numbers[i] = unique_numbers[j];
		unique_numbers[j] = temp;
	}

	/* create an empty tree */
	rb_create(&tree, sizeof(TreeItemData), tree_comparator, tree_combiner, tree_allocfunc, tree_freefunc, NULL);

	rb_begin_right_left_walk(&tree, &rlw);
	assert(rb_right_left_walk(&rlw) == NULL);

	/* fill a tree */
	for(i = 0; i < array_size; i++)
	{
		sprintf(item.data, "Item %08x", unique_numbers[i]);
		rb_insert(&tree, (RBNode*)&item, &isNew);
		assert(isNew);
	}

	/* check enumiration */
	nitems = 0;
	last_item = array_size;
	rb_begin_right_left_walk(&tree, &rlw);
	while( (tmp = (TreeItem)rb_right_left_walk(&rlw)) )
	{
		sscanf(tmp->data, "Item %08x", &i);

		assert(number_returned[i] == false);
		number_returned[i] = true;

		assert(last_item > i);
		last_item = i;

		nitems++;
	}
	assert(nitems == array_size);
	assert(last_item == 0);

	/* free memory */
	nitems = 0;
	while( (tmp = (TreeItem)rb_leftmost(&tree)) )
	{
		rb_delete(&tree, (RBNode*)tmp);
		nitems++;
	}
	assert(nitems == array_size);
}

/*
 * Make sure direct walk enumirates all items, and any item is returned only
 * once
 */
static void
direct_walk_test()
{
	RBTree tree;
	bool isNew;
	TreeItemData item;
	TreeItem tmp;
	RBTreeDirectWalk dw;
	bool number_returned[UNIQUE_NUMBERS_ARRAY_SIZE];
	int unique_numbers[UNIQUE_NUMBERS_ARRAY_SIZE];
	int i, j, temp, nitems;
	int array_size = UNIQUE_NUMBERS_ARRAY_SIZE;

#ifdef INIFNITE_TEST
	array_size = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
#endif

	for(i = 0; i < array_size; i++)
	{
		unique_numbers[i] = i;
		number_returned[i] = false;
	}

	/* shuffle numbers */
	for(i = 0; i < array_size; i++)
	{
		j = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
		assert((j >= 0) && (j < array_size));
		temp = unique_numbers[i];
		unique_numbers[i] = unique_numbers[j];
		unique_numbers[j] = temp;
	}

	/* create an empty tree */
	rb_create(&tree, sizeof(TreeItemData), tree_comparator, tree_combiner, tree_allocfunc, tree_freefunc, NULL);

	rb_begin_direct_walk(&tree, &dw);
	assert(rb_direct_walk(&dw) == NULL);

	/* fill a tree */
	for(i = 0; i < array_size; i++)
	{
		sprintf(item.data, "Item %08x", unique_numbers[i]);
		rb_insert(&tree, (RBNode*)&item, &isNew);
		assert(isNew);
	}

	/* check enumiration */
	nitems = 0;
	rb_begin_direct_walk(&tree, &dw);
	while( (tmp = (TreeItem)rb_direct_walk(&dw)) )
	{
		sscanf(tmp->data, "Item %08x", &i);
		assert(number_returned[i] == false);
		number_returned[i] = true;
		nitems++;
	}
	assert(nitems == array_size);

	/* free memory */
	nitems = 0;
	while( (tmp = (TreeItem)rb_leftmost(&tree)) )
	{
		rb_delete(&tree, (RBNode*)tmp);
		nitems++;
	}
	assert(nitems == array_size);
}

/*
 * Make sure inverted walk enumirates all items, and any item is returned only
 * once
 */
static void
inverted_walk_test()
{
	RBTree tree;
	bool isNew;
	TreeItemData item;
	TreeItem tmp;
	RBTreeInvertedWalk iw;
	bool number_returned[UNIQUE_NUMBERS_ARRAY_SIZE];
	int unique_numbers[UNIQUE_NUMBERS_ARRAY_SIZE];
	int i, j, temp, nitems;
	int array_size = UNIQUE_NUMBERS_ARRAY_SIZE;

#ifdef INIFNITE_TEST
	array_size = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
#endif

	for(i = 0; i < array_size; i++)
	{
		unique_numbers[i] = i;
		number_returned[i] = false;
	}

	/* shuffle numbers */
	for(i = 0; i < array_size; i++)
	{
		j = (int)((((double)rand()) / ((double)(RAND_MAX+1))) * array_size);
		assert((j >= 0) && (j < array_size));
		temp = unique_numbers[i];
		unique_numbers[i] = unique_numbers[j];
		unique_numbers[j] = temp;
	}

	/* create an empty tree */
	rb_create(&tree, sizeof(TreeItemData), tree_comparator, tree_combiner, tree_allocfunc, tree_freefunc, NULL);

	rb_begin_inverted_walk(&tree, &iw);
	assert(rb_inverted_walk(&iw) == NULL);

	/* fill a tree */
	for(i = 0; i < array_size; i++)
	{
		sprintf(item.data, "Item %08x", unique_numbers[i]);
		rb_insert(&tree, (RBNode*)&item, &isNew);
		assert(isNew);
	}

	/* check enumiration */
	nitems = 0;
	rb_begin_inverted_walk(&tree, &iw);
	while( (tmp = (TreeItem)rb_inverted_walk(&iw)) )
	{
		sscanf(tmp->data, "Item %08x", &i);
		assert(number_returned[i] == false);
		number_returned[i] = true;
		nitems++;
	}
	assert(nitems == array_size);

	/* free memory */
	nitems = 0;
	while( (tmp = (TreeItem)rb_leftmost(&tree)) )
	{
		rb_delete(&tree, (RBNode*)tmp);
		nitems++;
	}
	assert(nitems == array_size);
}

static void
run_tests()
{
	general_test();
	left_right_walk_test();
	right_left_walk_test();
	direct_walk_test();
	inverted_walk_test();
}

int main()
{
	/*
	struct timeval  tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_sec ^ tv.tv_usec);
	*/

	srand(0);

#ifdef INIFNITE_TEST
	{
		int i = 0;
		for(;;) {
			run_tests();
			i++;
			printf("Tests passed: %d\n", i);
		}
	}
#else
	run_tests();
#endif

	return 0;
}


