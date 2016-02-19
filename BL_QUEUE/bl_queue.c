/* BL_QUEUE functions */
/*////////////////////*/

#include "bl_queue.h"

struct bl_queue_t {
    size_t size;
    struct bl_queue_elm {
        void *data;
        struct bl_queue_elm *prev;
        struct bl_queue_elm *next;
    } *head, *tail;
};


bl_queue *bl_queue_new(void);
void bl_queue_enqueue(bl_queue *queue, void *data);
void *bl_queue_dequeue(bl_queue *queue);
void *bl_queue_peek(bl_queue *queue);
void *bl_queue_delete(bl_queue *queue, void *data, int (*cmp_func)(const void *elmData, const void *data));
void *bl_queue_modify(bl_queue *queue, void *data, void *newData, int (*cmp_func)(const void *elmData, const void *newData));
void bl_queue_foreach(bl_queue *queue, void *userData, void (*func)(void *data, void *userData));
void bl_queue_foreach_remove(bl_queue *queue, void *userData, void (*func)(void *data, void *userData));
void bl_queue_free(bl_queue *queue);
size_t bl_queue_size(bl_queue *queue);


/**
 * creates a new queue
 * @return the new queue
 */
bl_queue *bl_queue_new(void)
{
    bl_queue *queue = malloc(sizeof(bl_queue));
    *queue = (bl_queue){0, NULL, NULL};
    return queue;
}

/**
 * adds the given data to the queue (FIFO)
 * @param queue the queue
 * @param data the data to add
 */
void bl_queue_enqueue(bl_queue *queue, void *data)
{
    struct bl_queue_elm *elm = malloc(sizeof(struct bl_queue_elm));
    if(queue->size == 0) { // queue is empty
        *elm = (struct bl_queue_elm){data, NULL, NULL};
        queue->head = elm;
        queue->tail = elm;
    } else {
        *elm = (struct bl_queue_elm){data, NULL, queue->head};
        queue->head->prev = elm;
        queue->head = elm;
    }
    queue->size++;
}

/**
 * removes the last element from the queue (FIFO)
 * @param queue the queue
 * @return the data in the removed element
 */
void *bl_queue_dequeue(bl_queue *queue)
{
    if(queue->size == 0) {
        return NULL;
    }
    
    struct bl_queue_elm *elm = queue->tail;
    if(elm->prev) { // not head elm
        queue->tail = elm->prev;
        queue->tail->next = NULL;
    } else { // dequeue head elm
        queue->head = NULL;
        queue->tail = NULL;
    }
    queue->size--;
    
    void *data = elm->data;
    free(elm);
    return data;
}

/**
 * peak at the next queue element
 * @param queue the queue
 * @return the data in the next queue element
 */
void *bl_queue_peek(bl_queue *queue)
{
    if(queue->size == 0) {
        return NULL;
    }
    return queue->tail->data;
}

/**
 * removess the specified element from the queue
 * @param queue the queue
 * @param data to compare each element with
 * @param cmp_func() compares each element with the given data, should return 0 when elmData and data are equivalent
 * @return pointer to data from the removed element, NULL if no element was deleted
 */
void *bl_queue_remove(bl_queue *queue, void *data, int (*cmp_func)(const void *elmData, const void *data))
{
    struct bl_queue_elm *elm = queue->head;
    void *retData = NULL;
    while(elm) {
        if(cmp_func(elm->data, data) == 0) {
            retData = elm->data;
            queue->size--;
            break;
        }
        elm = elm->next;
    }
    
    if(elm) { // del found elm
        if(elm->prev) {
            elm->prev->next = elm->next;
        } else { // deleting head elm
            queue->head = elm->next;
        }
        if(elm->next) {
            elm->next->prev = elm->prev;
        } else { // deleting tail elm
            queue->tail = elm->prev;
        }
    }
    free(elm);
    
    return retData;
}

/**
 * changes the specified element to the newData, if no elm is found, newData is enqueued
 * @param queue the queue
 * @param data the specified elm to find
 * @param newData the new data to replace the old data with
 * @param cmp_func() compares each element to the provided data, should return 0 when elmData and data are equivalent
 * @return ptr to the old data that was modified, if the elm was not found, returns newData
 */
void *bl_queue_modify(bl_queue *queue, void *data, void *newData, int (*cmp_func)(const void *elmData, const void *data))
{
    struct bl_queue_elm *elm = queue->head;
    void *retData = NULL;
    while(elm) {
        if(cmp_func(elm->data, data) == 0) {
            retData = elm->data;
            break;
        }
        elm = elm->next;
    }
    
    if(elm) {
        elm->data = newData;
    } else {
        retData = newData;
        bl_queue_enqueue(queue, newData);
        queue->size++;
    }
    return retData;
}

/**
 * runs the provided function on every element of the queue
 * @param queue the queue
 * @param userData extra data to send to the function
 * @param func() function called on every elm of the queue, sent the elm data and the provided userData
 */
void bl_queue_foreach(bl_queue *queue, void *userData, void (*func)(void *data, void *userData))
{
    if(func) {
        struct bl_queue_elm *elm = queue->head;
        while(elm) {
            func(elm->data, userData);
            elm = elm->next;
        }
    }
}

/**
 * runs the provided function on every element of the queue and deletes all elements in the queue
 * @param queue the queue
 * @param userData extra data to send the function
 * @param func() function called on every elm of the queue, sent the elm data and the provided userData
 * @note call with func() = NULL to just delete every elm of the queue
 */
void bl_queue_foreach_remove(bl_queue *queue, void *userData, void (*func)(void *data, void *userData))
{
    struct bl_queue_elm *elm = queue->head;
    while(elm) {
        if(func) {
            func(elm->data, userData);
        }
        struct bl_queue_elm *tmp = elm;
        elm = elm->next;
        free(tmp);
    }
    *queue = (bl_queue){0, NULL, NULL};
}

/**
 * Frees all queue resources, does not touch the inserted data
 * @param queue the queue to free
 * @note to interact with the inserted data (free()?) use the provided foreach functions
 * @see bl_queue_foreach()
 * @see bl_queue_foreach_remove()
 */
void bl_queue_free(bl_queue *queue)
{
    bl_queue_foreach_remove(queue, NULL, NULL);
    free(queue);
}

/**
 * gives the number of elements in the queue
 * @param queue the queue
 * @return the number of elements in the queue
 */
size_t bl_queue_size(bl_queue *queue)
{
    return queue->size;
}
