/* BL_SLIST functions */
/*////////////////////*/

#include "bl_slist.h"


struct bl_slist_t {
	size_t size;
	
	struct bl_slist_elm {
		void *data;
		struct bl_slist_elm *next;
	} *head;
};

bl_slist *bl_slist_new(void)
{
	bl_slist *slist = malloc(sizeof(bl_slist)); /* might need to be "sizeof(struct bl_slist_t)" */
	slist->size = 0;
	slist->head = NULL;
	
	return slist;
}

void bl_slist_push(bl_slist *slist, void *data)
{
	struct bl_slist_elm *elm = malloc(sizeof(struct bl_slist_elm));
	assert(slist);
	elm->data = data;
	elm->next = slist->head;
	slist->head = elm;
	slist->size++;
}

void bl_slist_add_sorted(bl_slist *slist, void *data, int (*cmp_func)(void *, void *))
{
	struct bl_slist_elm *cur = NULL;
	struct bl_slist_elm *elm = malloc(sizeof(struct bl_slist_elm));
	int check = 1;
	
	assert(slist);
	elm->data = data;
	elm->next = NULL;
	cur = slist->head;
	
	if(cur == NULL || (cmp_func)(elm->data, cur->data) < 0) {
		elm->next = cur;
		slist->head = elm;
	} else {
		for( ; cur->next != NULL; cur = cur->next) {
			if((cmp_func)(elm->data, cur->next->data) < 0) {
				elm->next = cur->next;
				cur->next = elm;
				check = 0;
				break;
			}
		}
		if(check == 1) {
			cur->next = elm;
		}
	}
	slist->size++;
}

void *bl_slist_pop(bl_slist *slist)
{
	struct bl_slist_elm *tmp = NULL;
	void *data = NULL;
	
	assert(slist);
	assert(slist->head);
	
	tmp = slist->head;
	slist->head = tmp->next;
	data = tmp->data;
	free(tmp);
	slist->size--;
	return data;
}

void *bl_slist_del(bl_slist *slist, void *data, int (*cmp_func)(void *, void *))
{
	struct bl_slist_elm *cur = NULL;
	struct bl_slist_elm *del = NULL;
	void *del_data = NULL;
	
	assert(slist);
	assert(slist->head);
	
	if((cmp_func)(data, slist->head->data) == 0) {
		del = slist->head;
		del_data = del->data;
		slist->head = slist->head->next;
		free(del);
		slist->size--;
		return del_data;
	} else {
		for(cur = slist->head; cur->next != NULL; cur = cur->next) {
			if((cmp_func)(data, cur->next->data) == 0) {
				del = cur->next;
				del_data = cur->next->data;
				cur->next = cur->next->next;
				free(del);
				slist->size--;
				return del_data;
			}
		}
	}
	return NULL;
}

void **bl_slist_del_mult(bl_slist *slist, void *data, int (*cmp_func)(void *, void *))
{
	void **arr = NULL;
	void **tmp = NULL;
	struct bl_slist_elm *cur = NULL;
	struct bl_slist_elm *del = NULL;
	size_t i = 0;
	unsigned char head_check = 0;
	
	assert(slist);
	assert(slist->head);
	arr = malloc((slist->size + 1) * sizeof(void *));
	cur = slist->head;
	
	if((cmp_func)(data, cur->data) == 0) {
		arr[i] = cur->data;
		head_check = 1;
		i++;
	}
	while(cur->next != NULL) {
		if((cmp_func)(data, cur->next->data) == 0) {
			del = cur->next;
			arr[i] = del->data;
			i++;
			cur->next = cur->next->next;
			free(del);
			slist->size--;
		} else {
			cur = cur->next;
		}
	}
	if(head_check == 1) {
		del = slist->head;
		slist->head = slist->head->next;
		free(del);
		slist->size--;
	}
	
	arr[i] = NULL;
	i++;
	if((i + 1) <= slist->size / 4) { /* maybe turn the 1/4 into a changable #define */
		tmp = malloc((i + 1) * sizeof(void *));
		memcpy(tmp, arr, (i + 1) * sizeof(void *));
		free(arr);
		return tmp;
	}
	return arr;
}

void *bl_slist_peek(bl_slist *slist)
{
	assert(slist);
	assert(slist->head);
	return slist->head->data;
}

void *bl_slist_find(bl_slist *slist, void *data, int (*cmp_func)(void *, void *))
{
	struct bl_slist_elm *cur = NULL;
	
	for(cur = slist->head; cur != NULL; cur = cur->next) {
		if((cmp_func)(data, cur->data) == 0) {
			return cur->data;
		}
	}
	return NULL;
}

size_t bl_slist_size(bl_slist *slist)
{
	assert(slist);
	return slist->size;
}

void bl_slist_sort(bl_slist *slist, int (*cmp_func)(void *, void *)) /* use better algorithm (heap, merge) when possible */
{
	struct bl_slist_elm *cur = NULL;
	struct bl_slist_elm *sort_head = NULL;
	struct bl_slist_elm *sort_cur = NULL;
	int check = 1;
	
	if(slist->head != NULL) {
		sort_head = slist->head;
		slist->head = slist->head->next;
		sort_head->next = NULL;
	}
	
	while(slist->head != NULL) {
		/* isolate a node */
		cur = slist->head;
		slist->head = slist->head->next;
		cur->next = NULL;
		
		if((cmp_func)(cur->data, sort_head->data) < 0) {
			cur->next = sort_head;
			sort_head = cur; 
		} else {
			check = 1;
			for(sort_cur = sort_head; sort_cur->next != NULL; sort_cur = sort_cur->next) {
				if((cmp_func)(cur->data, sort_cur->next->data) < 0) {
					cur->next = sort_cur->next;
					sort_cur->next = cur;
					check = 0;
					break;
				}
			}
			if(check == 1) {
				sort_cur->next = cur;
			}
		}
	}
	slist->head = sort_head;
}

void bl_slist_free(bl_slist **slist, void (*free_func)(void *))
{
	struct bl_slist_elm *cur = NULL, *next = NULL;
	assert(*slist);
	
	if((*slist)->head != NULL) {
		for(cur = (*slist)->head; cur; cur = next) {
			next = cur->next;
			(free_func)(cur->data);
			free(cur);
		}
	}
	free(*slist);
	*slist = NULL;
}

void bl_slist_free_nodes(bl_slist *slist, void (*free_func)(void *))
{
	struct bl_slist_elm *cur = NULL, *next = NULL;
	assert(slist);
	if(slist->head != NULL) {
		for(cur = slist->head; cur; cur = next) {
			next = cur->next;
			(free_func)(cur->data);
			free(cur);
		}
	}
	slist->head = NULL;
	slist->size = 0;
}

void bl_slist_print(bl_slist *slist, void (print_func)(void *))
{
	struct bl_slist_elm *cur = NULL;

	assert(slist);
	if(slist->head == NULL) {
		printf("List Empty\n");
	} else {
		for(cur = slist->head; cur; cur = cur->next) {
			print_func(cur->data);
		}
	}
}
