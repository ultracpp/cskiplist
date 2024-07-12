# Skip List Data Structure in C

This repository contains an implementation of a skip list data structure in C. Skip lists are probabilistic data structures similar to balanced trees but are simpler to implement and can offer faster performance for certain operations like search and insertion.

## Features

- **Random Level Generation**: Nodes are probabilistically assigned levels, ensuring a balanced structure.
- **Efficient Search**: Utilizes skip pointers to skip over nodes that are unlikely to contain the target key.
- **Memory Management**: Proper allocation and deallocation of memory resources.
- **Modularity**: Implemented with clear separation of functions for easy understanding and maintenance.

## Usage

To use the skip list data structure in your C projects, follow these steps:

1. **Include `cskiplist.h`**: Include the header file in your project.
2. **Initialization**: Initialize a skip list using `c_skip_list_new()` function.
3. **Insertion**: Insert elements into the skip list using `c_skip_list_insert()`.
4. **Search**: Search for elements using `c_skip_list_find()`.
5. **Removal**: Remove elements using `c_skip_list_remove()`.
6. **Cleanup**: Destroy the skip list and free resources using `c_skip_list_destroy()`.

### Example

```c
#include <stdio.h>
#include <time.h>
#include "clib.h"
#include "cskiplist.h"

static inline int compare_int(const void* left, const void* right)
{
	int x = POINTER_TO_INT(left);
	int y = POINTER_TO_INT(right);

	if (x < y)
	{
		return -1;
	}
	else if (x == y)
	{
		return 0;
	}
	else
	{
		return 1;
	}

	return 1;
}

int main() {
    int capacity = 100;
	int rand_max = capacity * 2;

	srand((unsigned)time(NULL));

	for (int a = 0, b = 10; a < b; a++)
	{
		c_skip_list* plist = c_skip_list_new(0, &compare_int);

		for (int i = 0; i < capacity; i++)
		{
			int key = rand() % rand_max;
			c_skip_list_insert(plist, INT_TO_POINTER(key));
			printf("%d\n", key);
		}

		c_skip_list_print(plist);
		printf("====iterate, size:%d\n", plist->count);

		for (int i = 0, j = capacity * 2; i < j; i++)
		{
			int key = rand() % rand_max;
			c_skip_node* result = c_skip_list_find(plist, INT_TO_POINTER(key));

			if (result != NULL)
			{
				printf("contains, %d\n", POINTER_TO_INT(result->key));
			}
			else
			{
				printf("====not found, %d\n", key);
			}

			c_skip_list_remove(plist, INT_TO_POINTER(key));
		}

		c_skip_list_print(plist);
		printf("====remove, size:%d\n", plist->count);

		c_skip_list* plist2 = c_skip_list_new(0, &compare_int);
		c_skip_list_swap(plist2, plist);
		printf("====swap\n");

		c_skip_list_insert(plist, INT_TO_POINTER(777));

		c_skip_list_print(plist);
		printf("====plist, size:%d\n", plist->count);

		c_skip_list_print(plist2);
		printf("====plist2, size:%d\n", plist2->count);

		c_skip_list_destroy(plist);
		c_skip_list_destroy(plist2);

		printf("================\n");
  }
	
  return 0;
}
