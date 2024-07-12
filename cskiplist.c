#include "cskiplist.h"
#include "clib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_SKIP_LIST_MAX_LEVEL	5
#define	C_SKIP_LIST_P	0.5

static int random_level()
{
	static int first = 1;

	if (first)
	{
		srand((unsigned)time(NULL));
		first = 0;
	}

	int result = 1;

	while (result < C_SKIP_LIST_MAX_LEVEL && (rand() > RAND_MAX / 2))
	{
		++result;
	}

	return result;
}

inline static void init_node(c_skip_node* pnode)
{
	pnode->prev = (void**)malloc(sizeof(void*) * pnode->level);
	pnode->next = (void**)malloc(sizeof(void*) * pnode->level);

	for (int i = 0, j = pnode->level; i < j; i++)
	{
		pnode->prev[i] = NULL;
		pnode->next[i] = NULL;
	}
}

c_skip_node* c_head_node_new()
{
	int len = sizeof(c_skip_node);
	c_skip_node* pnode = (c_skip_node*)malloc(len);
	memset(pnode, 0x00, len);

	pnode->key = NULL;
	pnode->level = C_SKIP_LIST_MAX_LEVEL;

	init_node(pnode);
	return pnode;
}

c_skip_node* c_skip_node_new(void* key, int level)
{
	int len = sizeof(c_skip_node);
	c_skip_node* pnode = (c_skip_node*)malloc(len);
	memset(pnode, 0x00, len);

	pnode->key = key;
	pnode->level = level;

	init_node(pnode);
	return pnode;
}

void c_skip_node_destroy(c_skip_node* pnode)
{
	if (pnode->prev != NULL)
	{
		free(pnode->prev);
		pnode->prev = NULL;
	}

	if (pnode->next != NULL)
	{
		free(pnode->next);
		pnode->next = NULL;
	}

	if (pnode != NULL)
	{
		free(pnode);
		pnode = NULL;
	}
}

c_skip_list* c_skip_list_new(int value_free, int (*compareTo)(const void*, const void*))
{
	int len = sizeof(c_skip_list);
	c_skip_list* plist = (c_skip_list*)malloc(len);
	memset(plist, 0x00, len);

	c_skip_list_init(plist, value_free, compareTo);
	return plist;
}

void c_skip_list_init(c_skip_list* plist, int value_free, int (*compareTo)(const void*, const void*))
{
	plist->head = c_head_node_new();
	plist->count = 0;
	plist->value_free = value_free;
	plist->compareTo = compareTo;
}

void c_skip_list_destroy(c_skip_list* plist)
{
	if (plist == NULL)
	{
		return;
	}

	c_skip_list_clear(plist);

	if (plist->head != NULL)
	{
		c_skip_node_destroy(plist->head);
	}

	free(plist);
	plist = NULL;
}

void c_skip_list_swap(c_skip_list* dst, c_skip_list* src)
{
	c_skip_node* head = dst->head;

	for (int i = 0, j = C_SKIP_LIST_MAX_LEVEL; i < j; i++)
	{
		head->prev[i] = NULL;
		head->next[i] = NULL;
	}

	dst->head = src->head;
	dst->count = src->count;
	dst->value_free = src->value_free;
	dst->compareTo = src->compareTo;

	src->head = head;
	src->count = 0;
}

static inline c_skip_node* insert_level(c_skip_list* plist, c_skip_node* previous, c_skip_node* pnode, int level)
{
	c_skip_node* current = previous->next[level];

	if (current == NULL)
	{
		previous->next[level] = pnode;
		pnode->prev[level] = previous;
		return previous;
	}

	if (plist->compareTo(pnode->key, current->key) < 1)
	{
		previous->next[level] = pnode;
		pnode->next[level] = current;
		pnode->prev[level] = previous;
		current->prev[level] = pnode;
		return previous;
	}

	while (current->next[level] != NULL &&
		plist->compareTo(current->key, pnode->key) < 1 &&
		plist->compareTo(((c_skip_node*)current->next[level])->key, pnode->key) < 1)
	{
		current = current->next[level];
	}

	c_skip_node* successor = current->next[level];
	current->next[level] = pnode;
	pnode->prev[level] = current;
	pnode->next[level] = successor;

	if (successor != NULL)
	{
		successor->prev[level] = pnode;
	}

	return current;
}

void c_skip_list_insert(c_skip_list* plist, void* key)
{
	int level = random_level();

	c_skip_node* pnode = c_skip_node_new(key, level);
	c_skip_node* previous = NULL;

	for (int i = level - 1; i >= 0; i--)
	{
		if (previous != NULL)
		{
			previous = insert_level(plist, previous, pnode, i);
		}
		else
		{
			previous = insert_level(plist, plist->head, pnode, i);
		}
	}

	plist->count++;
}

c_skip_node* c_skip_list_find(c_skip_list* plist, void* key)
{
	int level = C_SKIP_LIST_MAX_LEVEL - 1;
	c_skip_node* current = plist->head->next[level];
	c_skip_node* previous = NULL;

	while (level > 0 && current == NULL)
	{
		current = plist->head->next[--level];
	}

	while (current != NULL)
	{
		int result = plist->compareTo(current->key, key);
		//printf("%d, %d %d\n", level, current->key, key);

		if (result == 0)
		{
			return current;
		}
		else if (result < 0)
		{
			//printf("%d < %d\n", current->key, key);
			previous = current;
			current = current->next[level];

			if (current == NULL)
			{
				while (level > 0 && current == NULL)
				{
					current = previous->next[--level];
				}
			}
		}
		else
		{
			//printf("%d > %d\n", current->key, key);
			current = NULL;

			if (previous != NULL)
			{
				while (level > 0 && current == NULL)
				{
					current = previous->next[--level];
				}
			}
			else
			{
				while (level > 0 && current == NULL)
				{
					current = plist->head->next[--level];
				}
			}
		}
	}

	return NULL;
}

int c_skip_list_remove(c_skip_list* plist, void* key)
{
	c_skip_node* pnode = c_skip_list_find(plist, key);

	if (pnode == NULL)
	{
		return 0;
	}

	for (int i = pnode->level - 1; i >= 0; i--)
	{
		//printf("level:%d\n", i);
		c_skip_node* prev = pnode->prev[i];
		c_skip_node* next = pnode->next[i];

		if (next != NULL)
		{
			//printf("next:%d\n", next->key);
			next->prev[i] = prev;
		}

		prev->next[i] = next;
	}

	c_skip_node_destroy(pnode);
	plist->count--;
	return 1;
}

void c_skip_list_clear(c_skip_list* plist)
{
	c_skip_node* current = plist->head->next[0];

	if (plist->value_free)
	{
		while (current != NULL)
		{
			c_skip_node* pnode = current->next[0];

			free(current->key);

			c_skip_node_destroy(current);

			current = pnode;
		}
	} else
	{
		while (current != NULL)
		{
			c_skip_node* pnode = current->next[0];

			c_skip_node_destroy(current);

			current = pnode;
		}
	}

	plist->count = 0;
}

void c_skip_list_print(c_skip_list* plist)
{
	for (int i = C_SKIP_LIST_MAX_LEVEL - 1; i >= 0; i--)
	{
		c_skip_node* current = plist->head->next[i];

		while (current != NULL)
		{
			c_skip_node* pnode = current->next[i];

			printf("%d ", current->key);

			current = pnode;
		}

		printf("\n");
	}
}
