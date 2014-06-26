/*
 * llist.c - a linked list implementation
 *
 */

#include <stdlib.h>

#include "llist.h"


static NODE *create_node(void *data)
{
	NODE *node = malloc(sizeof(NODE));
    	if (node) {
		node->data = data;
		node->next = NULL;
    	}
	return node;
}


LLIST *create_list(void)
{
	LLIST * list = malloc(sizeof(LLIST));
    	if (list) {
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
    	}
	return list;
}


void empty_list(LLIST *list)
{
    	NODE *node, *temp;
    	node = list->head;
    	while (node != NULL) {
		temp = node->next;
		free(node);
		node = temp;
   	}
}


void delete_list(LLIST *list)
{
    	if (list) {
		empty_list(list);
		free(list);
    	}
}


void append_tolist(LLIST *list, void *data)
{
    	NODE *node = create_node(data);
    	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
    	} else {
		list->tail->next = node;
		list->tail = node;
    	}
    	list->count++;
}













