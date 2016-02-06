/* Queue functions */
/*/////////////////*/
#pragma once
#ifndef BL_QUEUE_H
#define BL_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
//#include <errno.h>

typedef struct bl_queue_t bl_queue;

bl_queue *bl_queue_new(void);
void bl_queue_enqueue(bl_queue *queue, void *data);
void *bl_queue_dequeue(bl_queue *queue);
void *bl_queue_peak(bl_queue *queue);
void *bl_queue_delete(bl_queue *queue, void *data, int (*cmp_func)(const void *elmData, const void *data));
void *bl_queue_modify(bl_queue *queue, void *data, void *newData, int (*cmp_func)(const void *elmData, const void *newData));
void bl_queue_foreach(bl_queue *queue, void *userData, void (*func)(void *data, void *userData));
void bl_queue_foreach_delete(bl_queue *queue, void *userData, void (*func)(void *data, void *userData));
size_t bl_queue_size(bl_queue *queue);

#endif
