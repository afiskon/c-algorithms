#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
	char temp_buff[512];

	dlist_head head;
	dlist_init(&head);
	assert(dlist_is_empty(&head));

	strcpy(item1.data, "first item");
	strcpy(item2.data, "second item");
	strcpy(item3.data, "third item");

	dlist_push_tail(&head, (dlist_node*) &item1);
	assert(!dlist_is_empty(&head));

	dlist_push_tail(&head, (dlist_node*) &item2);
	dlist_push_tail(&head, (dlist_node*) &item3);

	temp_buff[0] = 0;
	dlist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		strcat(temp_buff, tmp->data);
		strcat(temp_buff, ",");
	}
	assert(strcmp(temp_buff, "first item,second item,third item,") == 0);

	tmp = (ListItem) dlist_pop_tail_node(&head);
	assert(strcmp(tmp->data, "third item") == 0);

	temp_buff[0] = 0;
	dlist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		strcat(temp_buff, tmp->data);
		strcat(temp_buff, ",");
	}
	assert(strcmp(temp_buff, "first item,second item,") == 0);

	return 0;
}
