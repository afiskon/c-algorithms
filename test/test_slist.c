#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
	char temp_buff[512];

	slist_head head;
	slist_init(&head);
	assert(slist_is_empty(&head));

	strcpy(item1.data, "first item");
	strcpy(item2.data, "second item");
	strcpy(item3.data, "third item");

	slist_push_head(&head, (slist_node*)&item1);
	assert(!slist_is_empty(&head));

	slist_push_head(&head, (slist_node*)&item2);
	slist_push_head(&head, (slist_node*)&item3);

	temp_buff[0] = 0;
	slist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		strcat(temp_buff, tmp->data);
		strcat(temp_buff, ",");
	}
	assert(strcmp(temp_buff, "third item,second item,first item,") == 0);

	tmp = (ListItem) slist_pop_head_node(&head);
	assert(strcmp(tmp->data, "third item") == 0);

	temp_buff[0] = 0;
	slist_foreach(iter, &head)
	{
		tmp = (ListItem) iter.cur;
		strcat(temp_buff, tmp->data);
		strcat(temp_buff, ",");
	}
	assert(strcmp(temp_buff, "second item,first item,") == 0);

	return 0;
}
