#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LIST_LENGTH 4096UL

// Make an ENUM for return types, errors and such

typedef struct Node
{
	int64_t elem;
	struct Node *next, *prev;
} Node;

typedef struct List
{
	Node *head, *tail;
	size_t length;
} List;

int8_t list_init(List** list)
{
	List* temp = (List*)calloc(1, sizeof(List));
	if(temp)
	{
		temp->head = temp->tail = NULL;
		temp->length = 0ULL;
		*list = temp;
		return 0;
	}

	return 2;
}

Node* list_createNode(int64_t num)
{
	Node* temp = (Node*)calloc(1, sizeof(Node));
	if(temp)
	{
		temp->elem = num;
		temp->next = temp->prev = NULL;
		return temp;
	}

	return NULL;
}

int8_t list_destroy(List* list)
{
	// Check if list actually exists, list != NULL and every item in the list is != NULL /
	// 0.
	if(!list)
		return -1;
	if(list->head)
	{
		Node* curr = list->head;
		while(curr->next)
		{
			curr->prev = NULL;
			curr = curr->next;
			free(curr->prev);
		}
		curr->prev = NULL;
		free(curr);
		list->head = list->tail = NULL;
		free(list);
		return 0;
	}
	return 1;
}

int8_t list_isEmpty(List* list)
{
	return (list->length == 0);
}

int8_t list_push(List* list, int64_t num)
{
	// Maybe check for bounds, no MAX or -MAX?
	if(!list)	 // If list is NULL
		return -1;
	else if(list->length >= MAX_LIST_LENGTH)	// If no more space left
		return 1;
	else if(list->length == 0UL)	// If list is empty
	{
		Node* temp = list_createNode(num);
		list->head = list->tail = temp;
		++(list->length);
	}
	else	// If list is non-empty
	{
		Node* temp = list_createNode(num);
		temp->prev = list->tail;
		list->tail->next = temp;
		list->tail = temp;
		++(list->length);
	}

	return 0;
}

int64_t list_pop(List* list)
{
	if(!list)
		return -1;
	else if(!list->length)
		return 1;
	else if(list->length == 1)
	{
		int64_t ret = list->head->elem;
		list->head->next = list->head->prev = NULL;
		--(list->length);
		free(list->head);
		return ret;
	}

	Node* prev_node = list->tail->prev;
	int64_t ret = list->tail->elem;
	list->tail->prev->next = NULL;
	list->tail->prev = NULL;
	--(list->length);
	free(list->tail);
	list->tail = prev_node;

	return ret;
}

int8_t list_insert(List* list, int64_t num, size_t index)
{
	if(!list)
		return -1;
	else if(list->length > MAX_LIST_LENGTH || index > list->length)
		return 1;
	Node* temp = list_createNode(num);
	if(!temp)
		return -1;

	if(index == 0UL)
	{
		list->head->prev = temp;
		temp->next = list->head;
		list->head = temp;
		++(list->length);
		return 0;
	}
	else if(index == list->length)
	{
		list->tail->next = temp;
		temp->prev = list->tail;
		list->tail = temp;
		++(list->length);

		return 0;
	}

	Node* curr = list->head;
	size_t i = 0UL;
	while(i++ < index && curr)
	{
		curr = curr->next;
	}
	curr->prev->next = temp;
	temp->prev = curr->prev;
	curr->prev = temp;
	temp->next = curr;
	++(list->length);
	return 0;
}

int8_t list_print(List* list)
{
	if(!list)
		return -1;
	else if(!list->length)
		return 1;

	Node* curr = list->head;
	size_t i = 0ULL;
	while(curr)
	{
		printf("%lu) %ld\n", i++, curr->elem);
		curr = curr->next;
	}
	printf("Length: %lu\n", list->length);
	return 0;
}

int64_t list_remove(List* list, size_t index)
{
	// Be smart about return errors, maybe write to an integer as an argument
	if(!list)
		return -1;
	else if(!list->length)
		return 1;

	int64_t ret;
	size_t i = 0ULL;
	Node* curr = list->head;
	while(i++ < index && curr)
	{
		curr = curr->next;
	}
	ret = curr->elem;
	return ret;
}

