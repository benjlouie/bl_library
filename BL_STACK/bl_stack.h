/* Stack Functions */
/*/////////////////*/
#ifndef BL_STACK_H
#define BL_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct bl_stack_t bl_stack;

bl_stack *bl_stack_new(void);
void bl_stack_push(bl_stack *stack, void *data);
void *bl_stack_pop(bl_stack *stack);
void *bl_stack_peek(bl_stack *stack);
void bl_stack_free(bl_stack **stack, void (*free_func)(void *));
void bl_stack_free_nodes(bl_stack *stack, void (*free_func)(void *));
size_t bl_stack_size(bl_stack *stack);

#endif
