/* BL_STACK functions */
/*////////////////////*/

#include "bl_stack.h"


struct bl_stack_t {
	size_t size;
	
	struct bl_stack_elm {
		void *data;
		struct bl_stack_elm *next;
	} *head;
};

bl_stack *bl_stack_new(void)
{
	bl_stack *stack = malloc(sizeof(bl_stack)); /* might need to be "sizeof(struct bl_stack_t)" */
	stack->size = 0;
	stack->head = NULL;
	
	return stack;
}

void bl_stack_push(bl_stack *stack, void *data)
{
	struct bl_stack_elm *elm = malloc(sizeof(struct bl_stack_elm));
	assert(stack);
	
	elm->data = data;
	elm->next = stack->head;
	stack->head = elm;
	stack->size++;
}

void *bl_stack_pop(bl_stack *stack)
{
	struct bl_stack_elm *tmp = NULL;
	void *data = NULL;
	
	assert(stack);
	assert(stack->head);
	
	tmp = stack->head;
	stack->head = tmp->next;
	data = tmp->data;
	free(tmp);
	stack->size--;
	return data;
}

void *bl_stack_peek(bl_stack *stack)
{
	assert(stack);
	assert(stack->head);
	return stack->head->data;
}

void bl_stack_free(bl_stack **stack, void (*free_func)(void *))
{
	struct bl_stack_elm *cur = NULL, *next = NULL;
	assert(*stack);
	
	if((*stack)->head != NULL) {
		for(cur = (*stack)->head; cur; cur = next) {
			next = cur->next;
			(free_func)(cur->data);
			free(cur);
		}
	}
	free(*stack);
	*stack = NULL;
}

void bl_stack_free_nodes(bl_stack *stack, void (*free_func)(void *))
{
	struct bl_stack_elm *cur = NULL, *next = NULL;
	assert(stack);
	
	if(stack->head != NULL) {
		for(cur = stack->head; cur; cur = next) {
			next = cur->next;
			(free_func)(cur->data);
			free(cur);
		}
	}
	stack->head = NULL;
	stack->size = 0;
}

size_t bl_stack_size(bl_stack *stack)
{
	assert(stack);
	return stack->size;
}
