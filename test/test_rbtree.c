#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include "rbtree.h"

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

/* Make sure left right walk returns item in sorted order */
static void
left_right_walk_sorting_test()
{
	/* TODO */
}

/* Make sure right left walk returns item in revere sorted order */
static void
right_left_walk_sorting_test()
{
	/* TODO */
}

/*
 * Make sure left right walk enumirates all items, and any item is returned
 * only once
 */
static void
left_right_walk_test()
{
	/* TODO */
}

/*
 * Make sure right left walk enumirates all items, and any item is returned
 * only once
 */
static void
right_left_walk_test()
{
	/* TODO */
}

/*
 * Make sure direct walk enumirates all items, and any item is returned only
 * once
 */
static void
direct_walk_test()
{
	/* TODO */
}

/*
 * Make sure inverted walk enumirates all items, and any item is returned only
 * once
 */
static void
inverted_walk_test()
{
	/* TODO */
}

int main()
{
/*
	struct timeval  tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_sec ^ tv.tv_usec);
*/
	srand(0);

	general_test();

	left_right_walk_sorting_test();
	right_left_walk_sorting_test();

	left_right_walk_test();
	right_left_walk_test();
	direct_walk_test();
	inverted_walk_test();

	return 0;
}

