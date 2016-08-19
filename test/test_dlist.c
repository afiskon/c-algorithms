#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilist.h"

typedef struct
{
	dlist_node node;
	char data[128];
} ListItemData;

typedef ListItemData *ListItem;

int main()
{
	dlist_iter iter;
	ListItemData item1, item2, item3;
	ListItem tmp;

	dlist_head head;
	dlist_init(&head);

	strcpy(item1.data, "first item");
	strcpy(item2.data, "second item");
	strcpy(item3.data, "third item");

	printf("is empty before: %d\n", dlist_is_empty(&head));

	dlist_push_tail(&head, (dlist_node*) &item1);
	dlist_push_tail(&head, (dlist_node*) &item2);
	dlist_push_tail(&head, (dlist_node*) &item3);

	printf("is empty after: %d\n", dlist_is_empty(&head));

	dlist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		printf("tmp->data = %s\n", tmp->data);
	}

	tmp = (ListItem) dlist_pop_tail_node(&head);
	printf("After dlist_pop_tail_node call tmp->data = %s\n", tmp->data);

	dlist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		printf("tmp->data = %s\n", tmp->data);
	}

	return 0;
}
