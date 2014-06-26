/*
 * llist.h - a linked list implementation
 *
 */

 #ifndef __LLIST_H
 #define __LLIST_H

 struct NODE {
 	void *data;
 	struct NODE *next;
 };

 typedef struct NODE NODE;

 typedef struct {
 	NODE *head;
 	NODE *tail;
 	unsigned count;	
 } LLIST;


LLIST *create_list(void);
void empty_list(LLIST *list);
void delete_list(LLIST *list);
void append_tolist(LLIST *list, void *data);



 #endif