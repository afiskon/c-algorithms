/*-------------------------------------------------------------------------
 *
 * rbtree.h
 *	  interface for PostgreSQL generic Red-Black binary tree package
 *
 * Copyright (c) 2009-2016, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *		src/include/lib/rbtree.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * RBNode is intended to be used as the first field of a larger struct,
 * whose additional fields carry whatever payload data the caller needs
 * for a tree entry.  (The total size of that larger struct is passed to
 * rb_create.)	RBNode is declared here to support this usage, but
 * callers must treat it as an opaque struct.
 */
typedef struct RBNode
{
	char color;					/* node's current color, red or black */
	struct RBNode *left;		/* left child, or RBNIL if none */
	struct RBNode *right;		/* right child, or RBNIL if none */
	struct RBNode *parent;		/* parent, or NULL (not RBNIL!) if none */
} RBNode;

/* Support functions to be provided by caller */
typedef int (*rb_comparator) (const RBNode *a, const RBNode *b, void *arg);
typedef void (*rb_combiner) (RBNode *existing, const RBNode *newdata, void *arg);
typedef RBNode *(*rb_allocfunc) (void *arg);
typedef void (*rb_freefunc) (RBNode *x, void *arg);
typedef void (*rb_sprintfunc) (RBNode *x, char* buff);

typedef struct RBTree RBTree;

/*
 * RBTree control structure
 */
struct RBTree
{
	RBNode	   *root;			/* root node, or RBNIL if tree is empty */

	/* Remaining fields are constant after rb_create */

	size_t		node_size;		/* actual size of tree nodes */
	/* The caller-supplied manipulation functions */
	rb_comparator comparator;
	rb_combiner combiner;
	rb_allocfunc allocfunc;
	rb_freefunc freefunc;
	/* Passthrough arg passed to all manipulation functions */
	void	   *arg;
};

extern void rb_create(RBTree* tree,
		  size_t node_size,
		  rb_comparator comparator,
		  rb_combiner combiner,
		  rb_allocfunc allocfunc,
		  rb_freefunc freefunc,
		  void *arg);

extern RBNode *rb_find(RBTree *rb, const RBNode *data);
extern RBNode *rb_leftmost(RBTree *rb);
extern RBNode *rb_rightmost(RBTree *rb);

extern RBNode *rb_insert(RBTree *rb, const RBNode *data, bool *isNew);
extern void rb_delete(RBTree *rb, RBNode *node);

extern void rb_tree_debug_print(RBTree* rb, rb_sprintfunc sprintfunc);

typedef enum RBTreeNextStep
{
	NextStepNone,
	NextStepUp,
	NextStepLeft,
	NextStepRight
} RBTreeNextStep;

typedef struct
{
	RBTree* rb;
	RBNode* last_visited;
	bool is_over;
} RBTreeLeftRightWalk;

extern void rb_begin_left_right_walk(RBTree *rb, RBTreeLeftRightWalk* lrw);
extern RBNode* rb_left_right_walk(RBTreeLeftRightWalk* lrw);

typedef struct
{
	RBTree* rb;
	RBNode* last_visited;
	bool is_over;
} RBTreeRightLeftWalk;

extern void rb_begin_right_left_walk(RBTree *rb, RBTreeRightLeftWalk* rlw);
extern RBNode* rb_right_left_walk(RBTreeRightLeftWalk* rlw);

typedef struct
{
	RBTree* rb;
	RBNode* last_visited;
	bool is_over;
} RBTreeDirectWalk;

extern void rb_begin_direct_walk(RBTree *rb, RBTreeDirectWalk* dw);
extern RBNode* rb_direct_walk(RBTreeDirectWalk* dw);

typedef struct
{
	RBTree* rb;
	RBNode* last_visited;
	RBTreeNextStep next_step;
	bool is_over;
} RBTreeInvertedWalk;

extern void rb_begin_inverted_walk(RBTree *rb, RBTreeInvertedWalk* dw);
extern RBNode* rb_inverted_walk(RBTreeInvertedWalk* dw);

#endif   /* RBTREE_H */
