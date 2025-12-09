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
}

void c_skip_list_swap(c_skip_list* dst, c_skip_list* src)
{
	/*c_skip_node* head = dst->head;

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
	src->count = 0;*/

	c_skip_list tmp = *dst;
	*dst = *src;
	*src = tmp;
}

void c_skip_list_swap_ptr(c_skip_list** dst, c_skip_list** src)
{
	c_skip_list* temp = *dst;
	*dst = *src;
	*src = temp;
}

void c_skip_list_insert(c_skip_list* plist, void* key)
{
	c_skip_node* update[C_SKIP_LIST_MAX_LEVEL];
	c_skip_node* current = plist->head;
	int level = random_level();

	for (int i = C_SKIP_LIST_MAX_LEVEL - 1; i >= 0; i--)
	{
		while (current->next[i] != NULL &&
			plist->compareTo(((c_skip_node*)current->next[i])->key, key) < 0)
		{
			current = (c_skip_node*)current->next[i];
		}
		update[i] = current;
	}

	c_skip_node* pnode = c_skip_node_new(key, level);

	for (int i = 0; i < level; i++)
	{
		pnode->next[i] = update[i]->next[i];
		pnode->prev[i] = update[i];

		if (update[i]->next[i] != NULL)
		{
			((c_skip_node*)update[i]->next[i])->prev[i] = pnode;
		}

		update[i]->next[i] = pnode;
	}

	plist->count++;
}

c_skip_node* c_skip_list_find(c_skip_list* plist, void* key)
{
	c_skip_node* current = plist->head;

	for (int i = C_SKIP_LIST_MAX_LEVEL - 1; i >= 0; i--)
	{
		while (current->next[i] != NULL &&
			plist->compareTo(((c_skip_node*)current->next[i])->key, key) < 0)
		{
			current = (c_skip_node*)current->next[i];
		}
	}

	current = (c_skip_node*)current->next[0];

	if (current != NULL && plist->compareTo(current->key, key) == 0)
	{
		return current;
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
		c_skip_node* prev = (c_skip_node*)pnode->prev[i];
		c_skip_node* next = (c_skip_node*)pnode->next[i];

		if (next != NULL)
		{
			next->prev[i] = prev;
		}

		prev->next[i] = next;
	}

	if (plist->value_free)
	{
		free(pnode->key);
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
			c_skip_node* pnode = (c_skip_node*)current->next[0];
			free(current->key);
			c_skip_node_destroy(current);
			current = pnode;
		}
	}
	else
	{
		while (current != NULL)
		{
			c_skip_node* pnode = (c_skip_node*)current->next[0];
			c_skip_node_destroy(current);
			current = pnode;
		}
	}

	for (int i = 0; i < C_SKIP_LIST_MAX_LEVEL; i++)
	{
		plist->head->next[i] = NULL;
		plist->head->prev[i] = NULL;
	}

	plist->count = 0;
}

void c_skip_list_print(c_skip_list* plist)
{
	for (int i = C_SKIP_LIST_MAX_LEVEL - 1; i >= 0; i--)
	{
		c_skip_node* current = (c_skip_node*)plist->head->next[i];

		while (current != NULL)
		{
			c_skip_node* pnode = (c_skip_node*)current->next[i];

			printf("%d ", current->key);

			current = pnode;
		}

		printf("\n");
	}
}
