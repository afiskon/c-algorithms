#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilist.h"

typedef struct
{
	slist_node node;
	char data[128];
} ListItemData;

typedef ListItemData *ListItem;

int main()
{
	slist_iter iter;
	ListItemData item1, item2, item3;
	ListItem tmp;

	slist_head head;
	slist_init(&head);

	strcpy(item1.data, "first item");
	strcpy(item2.data, "second item");
	strcpy(item3.data, "third item");

	printf("is empty before: %d\n", slist_is_empty(&head));

	slist_push_head(&head, (slist_node*)&item1);
	slist_push_head(&head, (slist_node*)&item2);
	slist_push_head(&head, (slist_node*)&item3);

	printf("is empty after: %d\n", slist_is_empty(&head));

	slist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		printf("tmp->data = %s\n", tmp->data);
	}

	tmp = (ListItem) slist_pop_head_node(&head);
	printf("After slist_pop_head_node call tmp->data = %s\n", tmp->data);

	slist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		printf("tmp->data = %s\n", tmp->data);
	}

	return 0;
}
