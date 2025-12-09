/*
 * cskiplist - Skip List Implementation in C Language
 * Copyright (c) 2024 Eungsuk Jeon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __C_SKIP_LIST_H
#define __C_SKIP_LIST_H
#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct _tag_c_skip_node
	{
		void* key;
		int level;
		void** prev;
		void** next;
	} c_skip_node;

	typedef struct _tag_c_skip_list
	{
		c_skip_node* head;
		int count;
		int value_free;
		int (*compareTo)(const void*, const void*);
	} c_skip_list;

	c_skip_node* c_head_node_new();
	c_skip_node* c_skip_node_new(void* key, int level);
	void c_skip_node_destroy(c_skip_node* pnode);

	c_skip_list* c_skip_list_new(int value_free, int (*compareTo)(const void*, const void*));
	void c_skip_list_init(c_skip_list* plist, int value_free, int (*compareTo)(const void*, const void*));
	void c_skip_list_destroy(c_skip_list* plist);
	void c_skip_list_swap(c_skip_list* dst, c_skip_list* src);
	void c_skip_list_swap_ptr(c_skip_list** dst, c_skip_list** src);

	void c_skip_list_insert(c_skip_list* plist, void* key);
	c_skip_node* c_skip_list_find(c_skip_list* plist, void* key);
	int c_skip_list_remove(c_skip_list* plist, void* key);
	void c_skip_list_clear(c_skip_list* plist);
	void c_skip_list_print(c_skip_list* plist);

#ifdef __cplusplus
}
#endif
#endif
